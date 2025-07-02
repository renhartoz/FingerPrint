#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include "dpfpdd.h"
#include "bmp.h"

#define IMAGE_WIDTH  500
#define IMAGE_HEIGHT 550
#define IMAGE_SIZE (IMAGE_WIDTH * IMAGE_HEIGHT)

void SaveBitmap(const std::string& filename, const unsigned char* data, int width, int height) {
    BMP bmp(width, height);
    std::memcpy(bmp.data.data(), data, width * height);
    bmp.write(filename);
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
    if (dpfpdd_get_device_capabilities(reader, &caps) != DPFPDD_SUCCESS) {
        std::cerr << "Failed to get scanner capabilities!" << std::endl;
        dpfpdd_close(reader);
        dpfpdd_exit();
        return -1;
    }
    
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
        result = dpfpdd_capture(reader, &capture_param, timeout, &capture_result, &image_size, image_data);
        if (result == DPFPDD_SUCCESS) {
            SaveBitmap(output_filename, image_data, IMAGE_WIDTH, IMAGE_HEIGHT);
            std::cout << "Fingerprint captured and saved as: " << output_filename << std::endl;
        } else {
            std::cerr << "Failed to capture fingerprint! Error code: " << result << std::endl;
        }
    
        delete[] image_data;
        dpfpdd_close(reader);
        dpfpdd_exit();
        return result;
    }
}
