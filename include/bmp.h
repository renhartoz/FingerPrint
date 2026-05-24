#ifndef BMP_H
#define BMP_H

#include <cstdint>
#include <vector>
#include <fstream>
#include <iostream>

#pragma pack(push, 1)

struct BMPFileHeader {
    uint16_t file_type{ 0x4D42 };   // "BM"
    uint32_t file_size{ 0 };        // File size in bytes
    uint16_t reserved1{ 0 };
    uint16_t reserved2{ 0 };
    uint32_t offset_data{ 54 };     // Start of pixel data
};

struct BMPInfoHeader {
    uint32_t size{ 40 };            // Header size
    int32_t width{ 0 };             // Image width
    int32_t height{ 0 };            // Image height
    uint16_t planes{ 1 };           // Always 1
    uint16_t bit_count{ 8 };        // Bits per pixel (grayscale)
    uint32_t compression{ 0 };      // No compression
    uint32_t size_image{ 0 };       // Image data size
    int32_t x_pixels_per_meter{ 2835 };
    int32_t y_pixels_per_meter{ 2835 };
    uint32_t colors_used{ 256 };    // 256 grayscale colors
    uint32_t colors_important{ 256 };
};

struct BMPColorTable {
    uint8_t r, g, b, reserved;
};

#pragma pack(pop)

class BMP {
public:
    BMPFileHeader file_header;
    BMPInfoHeader info_header;
    std::vector<BMPColorTable> color_table;
    std::vector<uint8_t> data;

    BMP(int32_t width, int32_t height) {
        int32_t row_stride = (width + 3) & ~3;
        info_header.width = width;
        info_header.height = -height; // Top-down image
        info_header.size_image = row_stride * height;
        file_header.file_size = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader) + 1024 + info_header.size_image;
        data.resize(info_header.size_image, 0);

        // Create grayscale color table
        color_table.resize(256);
        for (int i = 0; i < 256; ++i) {
            color_table[i] = { (uint8_t)i, (uint8_t)i, (uint8_t)i, 0 };
        }
    }

    void write(const std::string& filename) {
        std::ofstream file(filename, std::ios::binary);
        if (!file) {
            std::cerr << "Error opening file: " << filename << std::endl;
            return;
        }

        file.write(reinterpret_cast<const char*>(&file_header), sizeof(file_header));
        file.write(reinterpret_cast<const char*>(&info_header), sizeof(info_header));
        file.write(reinterpret_cast<const char*>(color_table.data()), color_table.size() * sizeof(BMPColorTable));
        file.write(reinterpret_cast<const char*>(data.data()), data.size());

        file.close();
    }
};

#endif
