#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <cmath>
#include "dpfpdd.h"
#include "bmp.h"

#define IMAGE_WIDTH  273
#define IMAGE_HEIGHT 344
#define IMAGE_SIZE (IMAGE_WIDTH * IMAGE_HEIGHT)

void SaveBitmap(const std::string& filename, const unsigned char* data, int width, int height) {
    BMP bmp(width, height);
    int row_stride = (width + 3) & ~3;
    for (int y = 0; y < height; ++y) {
        std::memcpy(bmp.data.data() + y * row_stride, data + y * width, width);
    }
    bmp.write(filename);
}

void SaveBitmapScaled(const std::string& filename, const unsigned char* raw_data, int raw_width, int raw_height, int target_width, int target_height) {
    BMP bmp(target_width, target_height);
    int dest_row_stride = (target_width + 3) & ~3;
    
    float zoom = 0.75f; 
    float shift_x = -30.0f;
    float shift_y = -30.0f;
    
    float src_w = target_width / zoom;
    float src_h = target_height / zoom;
    
    float cx = raw_width / 2.0f + shift_x;
    float cy = raw_height / 2.0f + shift_y;
    
    float sx0 = cx - src_w / 2.0f;
    float sy0 = cy - src_h / 2.0f;
    
    int hist[256] = {0};
    int total_pixels = 0;
    for (int y = 0; y < target_height; ++y) {
        for (int x = 0; x < target_width; ++x) {
            float sx = sx0 + x / zoom;
            float sy = sy0 + y / zoom;
            int x0 = (int)sx;
            int y0 = (int)sy;
            if (x0 >= 0 && x0 < raw_width && y0 >= 0 && y0 < raw_height) {
                hist[raw_data[y0 * raw_width + x0]]++;
                total_pixels++;
            }
        }
    }
    
    int min_val = 0;
    int max_val = 255;
    if (total_pixels > 0) {
        int sum = 0;
        for (int i = 0; i < 256; ++i) {
            sum += hist[i];
            if (sum > total_pixels * 0.25f) {
                min_val = i;
                break;
            }
        }
        sum = 0;
        for (int i = 255; i >= 0; --i) {
            sum += hist[i];
            if (sum > total_pixels * 0.05f) {
                max_val = i;
                break;
            }
        }
    }
    
    for (int y = 0; y < target_height; ++y) {
        unsigned char* dest_row = bmp.data.data() + (y * dest_row_stride);
        for (int x = 0; x < target_width; ++x) {
            float sx = sx0 + x / zoom;
            float sy = sy0 + y / zoom;
            
            int x0 = (int)sx;
            int y0 = (int)sy;
            
            if (x0 < 0 || x0 >= raw_width - 1 || y0 < 0 || y0 >= raw_height - 1) {
                dest_row[x] = 255;
                continue;
            }
            
            float dx = sx - x0;
            float dy = sy - y0;
            
            int x1 = x0 + 1;
            int y1 = y0 + 1;
            
            float p00 = raw_data[y0 * raw_width + x0];
            float p10 = raw_data[y0 * raw_width + x1];
            float p01 = raw_data[y1 * raw_width + x0];
            float p11 = raw_data[y1 * raw_width + x1];
            
            float val = (1.0f - dx) * (1.0f - dy) * p00 +
                        dx * (1.0f - dy) * p10 +
                        (1.0f - dx) * dy * p01 +
                        dx * dy * p11;
            
            if (max_val > min_val) {
                val = (val - min_val) * 255.0f / (max_val - min_val);
            }
            
            if (val < 0.0f) val = 0.0f;
            if (val > 255.0f) val = 255.0f;
            
            float t = val / 255.0f;
            t = t < 0.5f ? 2.0f * t * t : 1.0f - pow(-2.0f * t + 2.0f, 2.0f) / 2.0f;
            t = t < 0.5f ? 2.0f * t * t : 1.0f - pow(-2.0f * t + 2.0f, 2.0f) / 2.0f;
            val = t * 255.0f;
            
            dest_row[x] = (unsigned char)(val + 0.5f);
        }
    }
    
    bmp.write(filename);
}

DPFPDD_DEV g_reader = nullptr;

extern "C" __declspec(dllexport) void cancel_scan() {
    if (g_reader != nullptr) {
        dpfpdd_cancel(g_reader);
    }
}

extern "C" __declspec(dllexport) int scan_fingerprint(const char* output_filename) {
    int result;
    DPFPDD_DEV reader;
    DPFPDD_DEV_INFO readerInfo;
    if (dpfpdd_init() != DPFPDD_SUCCESS) {
        std::cerr << "Failed to initialize fingerprint device" << std::endl;
        return -1;
    }

    unsigned int dev_count = 1;
    if (dpfpdd_query_devices(&dev_count, &readerInfo) != DPFPDD_SUCCESS) {
        std::cerr << "No fingerprint reader found!" << std::endl;
        dpfpdd_exit();
        return -1;
    }

    if (dpfpdd_open(readerInfo.name, &reader) != DPFPDD_SUCCESS) {
        std::cerr << "Failed to open scanner: " << readerInfo.name << std::endl;
        dpfpdd_exit();
        return -1;
    }
    
    DPFPDD_DEV_CAPS caps;
    caps.size = sizeof(DPFPDD_DEV_CAPS);
    int caps_res = dpfpdd_get_device_capabilities(reader, &caps);
    if (caps_res != DPFPDD_SUCCESS) {
        std::cerr << "Failed to get scanner capabilities! Code: " << caps_res << std::endl;
        dpfpdd_close(reader);
        dpfpdd_exit();
        return -1;
    }
    
    // std::cout << "Device capabilities - Resolution count: " << caps.resolution_cnt << std::endl;
    // if (caps.resolution_cnt > 0) {
    //     std::cout << "Device capability Resolution [0]: " << caps.resolutions[0] << " DPI" << std::endl;
    // }
    
    DPFPDD_IMAGE_FMT img_fmt = DPFPDD_IMG_FMT_ANSI381;
    DPFPDD_IMAGE_PROC img_proc = DPFPDD_IMG_PROC_DEFAULT;
    unsigned int img_res = 700;
    DPFPDD_CAPTURE_PARAM capture_param = { sizeof(DPFPDD_CAPTURE_PARAM), img_fmt, img_proc, img_res };
    
    unsigned char* image_data = nullptr;
    unsigned int image_size = 0;
    unsigned int timeout = -1;
    DPFPDD_CAPTURE_RESULT capture_result = { sizeof(DPFPDD_CAPTURE_RESULT) };

    result = dpfpdd_capture(reader, &capture_param, timeout, &capture_result, &image_size, image_data);
    if (result != DPFPDD_E_MORE_DATA) {
        std::cerr << "Error getting required image size! Code: " << result << std::endl;
        dpfpdd_close(reader);
        dpfpdd_exit();
        return result;
    } else {
        image_data = new unsigned char[image_size];
        g_reader = reader;
        result = dpfpdd_capture(reader, &capture_param, timeout, &capture_result, &image_size, image_data);
        g_reader = nullptr;
        if (result == DPFPDD_SUCCESS && capture_result.success == 1) {
            int raw_width = capture_result.info.width;
            int raw_height = capture_result.info.height;
            int header_offset = image_size - (raw_width * raw_height);
            if (header_offset < 0) header_offset = 0;
            SaveBitmapScaled(output_filename, image_data + header_offset, raw_width, raw_height, IMAGE_WIDTH, IMAGE_HEIGHT);
            std::cout << "Fingerprint captured (" << raw_width << "x" << raw_height << ") with header offset " << header_offset << ", scaled to " << IMAGE_WIDTH << "x" << IMAGE_HEIGHT << " and saved as: " << output_filename << std::endl;
        } else {
            std::cerr << "Failed to capture fingerprint! Error code: " << result << std::endl;
        }
    
        delete[] image_data;
        dpfpdd_close(reader);
        dpfpdd_exit();
        return result;
    }
}
