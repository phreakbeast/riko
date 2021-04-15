#include "tek_image.h"

#include <string.h>
#include <stdlib.h>

#pragma pack(push, 1)
struct BMPFileHeader {
    u16 file_type;          // File type always BM which is 0x4D42
    u32 file_size;               // Size of the file (in bytes)
    u16 reserved1;               // Reserved, always 0
    u16 reserved2;               // Reserved, always 0
    u32 offset_data;             // Start position of pixel data (bytes from the beginning of the file)
};

struct BMPInfoHeader {
    u32 size;                      // Size of this header (in bytes)
    i32 width;                      // width of bitmap in pixels
    i32 height;                     // width of bitmap in pixels
    //       (if positive, bottom-up, with origin in lower left corner)
    //       (if negative, top-down, with origin in upper left corner)
    u16 planes;                    // No. of planes for the target device, this is always 1
    u16 bit_count;                 // No. of bits per pixel
    u32 compression;               // 0 or 3 - uncompressed. THIS PROGRAM CONSIDERS ONLY UNCOMPRESSED BMP images
    u32 size_image;                // 0 - for uncompressed images
    i32 x_pixels_per_meter;
    i32 y_pixels_per_meter;
    u32 colors_used;               // No. color indexes in the color table. Use 0 for the max number of colors allowed by bit_count
    u32 colors_important;          // No. of colors used for displaying the bitmap. If 0 all colors are required
};

struct BMPColorHeader {
    u32 red_mask;         // Bit mask for the red channel
    u32 green_mask;       // Bit mask for the green channel
    u32 blue_mask;        // Bit mask for the blue channel
    u32 alpha_mask;       // Bit mask for the alpha channel
    u32 color_space_type; // Default "sRGB" (0x73524742)
    u32 unused[16];                // Unused data for sRGB color space
};

#pragma pack(pop)

static void bmpfileheader_init(struct BMPFileHeader* hdr)
{
    hdr->file_type = 0x4D42;
    hdr->file_size = 0;
    hdr->reserved1 = 0;
    hdr->reserved2 = 0;
    hdr->offset_data = 0;
}

void bmpinfoheader_init(struct BMPInfoHeader* hdr)
{
    hdr->size = 0;
    hdr->width = 0;
    hdr->height = 0;
    hdr->planes = 1;
    hdr->bit_count = 0;
    hdr->compression = 0;
    hdr->size_image = 0;
    hdr->x_pixels_per_meter = 0;
    hdr->y_pixels_per_meter = 0;
    hdr->colors_used = 0;
    hdr->colors_important = 0;
}

void bmpcolorheader_init(struct BMPColorHeader* hdr)
{
    hdr->red_mask = 0x00ff0000;
    hdr->green_mask = 0x0000ff00;
    hdr->blue_mask = 0x000000ff;
    hdr->alpha_mask = 0xff000000;
    hdr->color_space_type = 0x73524742;
}

struct BMP{
    struct BMPFileHeader file_header;
    struct BMPInfoHeader bmp_info_header;
    struct BMPColorHeader bmp_color_header;
    u8 *pixels;
    u32 num_pixels;
    u32 row_stride;
};

bool check_color_header(struct BMPColorHeader bmp_color_header)
        {
    struct BMPColorHeader expected_color_header;
    bmpcolorheader_init(&expected_color_header);
    if (expected_color_header.red_mask != bmp_color_header.red_mask ||
        expected_color_header.blue_mask != bmp_color_header.blue_mask ||
        expected_color_header.green_mask != bmp_color_header.green_mask ||
        expected_color_header.alpha_mask != bmp_color_header.alpha_mask) {
        printf("Unexpected color mask format! The program expects the pixel data to be in the BGRA format\n");
        return false;
    }

    if (expected_color_header.color_space_type != bmp_color_header.color_space_type) {
        printf("Unexpected color space type! The program expects sRGB values\n");
        return false;
    }

    return true;
}

// Add 1 to the row_stride until it is divisible with align_stride
u32 make_stride_aligned(struct BMP bmp, u32 align_stride)
{
    uint32_t new_stride = bmp.row_stride;
    while (new_stride % align_stride != 0)
    {
        new_stride++;
    }
    return new_stride;
}

bool image_load_bmp(Image *img, const char *filename)
{
    printf("loading image %s\n", filename);

    FILE *file;
    file = fopen(filename, "rb");
    if (!file)
    {
        printf("cannot open %s\n", filename);
        return false;
    }

    struct BMP bmp;
    bmpfileheader_init(&bmp.file_header);
    bmpinfoheader_init(&bmp.bmp_info_header);
    bmpcolorheader_init(&bmp.bmp_color_header);

    fread(&bmp.file_header, sizeof(struct BMPFileHeader), 1, file);
    fread(&bmp.bmp_info_header, sizeof(bmp.bmp_info_header), 1, file);

    // The BMPColorHeader is used only for transparent images
    if (bmp.bmp_info_header.bit_count == 32) {
        // Check if the file has bit mask color information
        if (bmp.bmp_info_header.size >= (sizeof(struct BMPInfoHeader) + sizeof(struct BMPColorHeader))) {
            //in_file.read((char *) &bmp.bmp_color_header, sizeof(bmp.bmp_color_header));
            fread(&bmp.bmp_color_header, sizeof(bmp.bmp_color_header), 1, file);
            // Check if the pixel data is stored as BGRA and if the color space type is sRGB
            if (!check_color_header(bmp.bmp_color_header))
            {
                fclose(file);
                return false;
            }
        } else {
            printf("Warning! The file %s does not seem to contain bit mask information\n", filename);
            fclose(file);
            return false;
        }
    }

    // Jump to the pixel data location
    fseek(file, bmp.file_header.offset_data, SEEK_SET); //maybe from beginning
    //in_file.seekg(in_file.beg);

    // Adjust the header fields for output.
    // Some editors will put extra info in the image file, we only save the headers and the data.
    if (bmp.bmp_info_header.bit_count == 32)
    {
        bmp.bmp_info_header.size = sizeof(struct BMPInfoHeader) + sizeof(struct BMPColorHeader);
        bmp.file_header.offset_data = sizeof(struct BMPFileHeader) + sizeof(struct BMPInfoHeader) + sizeof(struct BMPColorHeader);
    }
    else
    {
        bmp.bmp_info_header.size = sizeof(struct BMPInfoHeader);
        bmp.file_header.offset_data = sizeof(struct BMPFileHeader) + sizeof(struct BMPInfoHeader);
    }
    bmp.file_header.file_size = bmp.file_header.offset_data;

    if (bmp.bmp_info_header.height < 0)
    {
        printf("The program can treat only BMP images with the origin in the bottom left corner!\n");
        fclose(file);
        return false;
    }

    bmp.num_pixels = bmp.bmp_info_header.width * bmp.bmp_info_header.height * bmp.bmp_info_header.bit_count / 8;
    bmp.pixels = malloc(sizeof(u8) * bmp.num_pixels);

    // Here we check if we need to take into account row padding
    if (bmp.bmp_info_header.width % 4 == 0) {
        //in_file.read((char *) &bmp.pixels, sizeof(u8) * bmp.num_pixels);
        fread((char*)bmp.pixels, sizeof(u8) * bmp.num_pixels, 1, file);
        bmp.file_header.file_size += bmp.num_pixels;
    }
    else
    {
        bmp.row_stride = bmp.bmp_info_header.width * bmp.bmp_info_header.bit_count / 8;
        u32 new_stride = make_stride_aligned(bmp, 4);
        u32 padding_row_size = new_stride - bmp.row_stride;
        u8* padding_row = malloc(sizeof(u8) * padding_row_size);

        for (int y = 0; y < bmp.bmp_info_header.height; ++y)
        {
            fread((char*)(bmp.pixels + bmp.row_stride * y), bmp.row_stride, 1, file);
            fread((char*)padding_row, padding_row_size, 1, file);
            //in_file.read((char *) (bmp.pixels + bmp.row_stride * y), bmp.row_stride);
            //in_file.read((char *) padding_row.data(), padding_row.size());
        }
        bmp.file_header.file_size += bmp.num_pixels + bmp.bmp_info_header.height * padding_row_size;
    }

    fclose(file);
    //in_file.close();

    //TODO: handle 32 bit (4 byte) pixels
    u8 tmp_rgb = 0; // Swap buffer
    for (unsigned long i = 0; i < bmp.num_pixels; i += 3) 
    {
        tmp_rgb = bmp.pixels[i];
        bmp.pixels[i] = bmp.pixels[i + 2];
        bmp.pixels[i + 2] = tmp_rgb;
    }

    ImageFormat format = IMAGE_FORMAT_RGB;
    int internal_format = 3;
    if (bmp.bmp_info_header.bit_count == 32)
    {
        format = IMAGE_FORMAT_RGBA;
        internal_format = 4;
    }

    img->width = bmp.bmp_info_header.width;
    img->height = bmp.bmp_info_header.height;
    img->format = format;
    img->internal_format = internal_format;
    img->pixels = malloc(sizeof(u8) * img->width * img->height * img->internal_format);
    memcpy(img->pixels, bmp.pixels, sizeof(u8) * img->width * img->height * img->internal_format);

    return true;
}

void write_headers(struct BMP bmp, FILE *file)
{
    fwrite((const char *) &bmp.file_header, sizeof(bmp.file_header), 1, file);
    fwrite((const char *) &bmp.bmp_info_header, sizeof(bmp.bmp_info_header), 1, file);

    if (bmp.bmp_info_header.bit_count == 32)
    {
        //file.write((const char *) &bmp.bmp_color_header, sizeof(bmp.bmp_color_header));
        fwrite((const char *) &bmp.bmp_color_header, sizeof(bmp.bmp_color_header), 1, file);
    }
}

void write_headers_and_data(struct BMP bmp, FILE *file)
{
    write_headers(bmp, file);
    fwrite((const char *) &bmp.pixels, sizeof(u8) & bmp.num_pixels, 1, file);
}

bool image_save_bmp(Image *img, const char *filename) 
{
    printf("saving image %s\n", filename);

    FILE *file;

    file = fopen(filename, "wb");
    if (!file)
    {
        printf("cannot open %s\n", filename);
        return false;
    }

    struct BMP bmp;
    bmpfileheader_init(&bmp.file_header);
    bmpinfoheader_init(&bmp.bmp_info_header);
    bmpcolorheader_init(&bmp.bmp_color_header);

    bmp.bmp_info_header.width = img->width;
    bmp.bmp_info_header.height = img->height;
    if (img->format == IMAGE_FORMAT_RGBA)
    {
        bmp.bmp_info_header.size = sizeof(struct BMPInfoHeader) + sizeof(struct BMPColorHeader);
        bmp.file_header.offset_data = sizeof(struct BMPFileHeader) + sizeof(struct BMPInfoHeader) + sizeof(struct BMPColorHeader);
        bmp.bmp_info_header.bit_count = 32;
        bmp.bmp_info_header.compression = 3;
        bmp.row_stride = img->width * 4;
        bmp.num_pixels = bmp.row_stride * img->height;
        bmp.pixels = malloc(sizeof(u8) * bmp.num_pixels);
        bmp.file_header.file_size = bmp.file_header.offset_data + bmp.num_pixels;
    }
    else if (img->format == IMAGE_FORMAT_RGB)
    {
        bmp.bmp_info_header.size = sizeof(struct BMPInfoHeader);
        bmp.file_header.offset_data = sizeof(struct BMPFileHeader) + sizeof(struct BMPInfoHeader);

        bmp.bmp_info_header.bit_count = 24;
        bmp.bmp_info_header.compression = 0;
        bmp.row_stride = img->width * 3;
        bmp.num_pixels = bmp.row_stride * img->height;
        bmp.pixels = malloc(sizeof(u8) * bmp.num_pixels);

        uint32_t new_stride = make_stride_aligned(bmp, 4);
        bmp.file_header.file_size =
                bmp.file_header.offset_data + bmp.num_pixels +
                bmp.bmp_info_header.height * (new_stride - bmp.row_stride);
    }
    else
    {
        printf("wrong pixel format\n");
        fclose(file);
        return false;
    }

    if (bmp.bmp_info_header.bit_count == 32)
    {
        write_headers_and_data(bmp, file);
    }
    else if (bmp.bmp_info_header.bit_count == 24)
    {
        if (bmp.bmp_info_header.width % 4 == 0)
        {
            write_headers_and_data(bmp, file);
        }
        else
        {
            u32 new_stride = make_stride_aligned(bmp, 4);
            u32 padding_row_size = new_stride - bmp.row_stride;
            u8* padding_row = malloc(sizeof(u8) * padding_row_size);

            write_headers(bmp, file);

            for (int y = 0; y < bmp.bmp_info_header.height; ++y)
            {
                fwrite((const char *) &bmp.pixels + bmp.row_stride * y, bmp.row_stride, 1, file);
                fwrite((const char *) padding_row, padding_row_size, 1, file);
            }
        }
    }
    else
    {
        printf("The program can treat only 24 or 32 bits per pixel BMP files\n");
        fclose(file);
        return false;
    }

    fclose(file);

    return true;
}
