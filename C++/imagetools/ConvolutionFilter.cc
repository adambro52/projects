//
// Created by A-dizzle on 10/31/2018.
// Copyright 2018 Adam Smith

#include <cmath>
#include "imagetools/ConvolutionFilter.h"
#include "imagetools/filter.h"
#include "imagetools/float_matrix.h"
#include "imagetools/color_data.h"

namespace image_tools {

    ConvolutionFilter::ConvolutionFilter() : kernel_(NULL) {}
    ConvolutionFilter::~ConvolutionFilter() {
        if (kernel_ != NULL) {
            delete kernel_;
        }
    }

    void ConvolutionFilter::SetUpFilter() {}

    ColorData ConvolutionFilter::CalculateFilteredPixel(PixelBuffer* buf,
        int x, int y) {
        kernel_ = CreateKernel();
        ColorData pixel = ColorData(0, 0, 0);
        ColorData new_pixel;

        int height = kernel_->height();
        int width = kernel_->width();
        int buf_w = buf->width();
        int buf_h = buf->height();
        int x_offset;
        int y_offset;

        float val = 0;

        // iterate through the matrix kernel and multiply its values with the
        // adjacent value in the pixel puffer then add each product to the
        // running sum 'pixel' defined above
        for (int h = 0; h < height; h++) {
            for (int w = 0; w < width; w++) {
                val = kernel_->value(w, h);

                // offset in order to access the correct values in the
                // pixel_buffer object
                x_offset = w - floor(width / 2);
                y_offset = h - floor(height / 2);

                if (x + x_offset >= 0 && x + x_offset < buf_w &&
                    y + y_offset >= 0 && y + y_offset < buf_h) {
                    new_pixel = buf->pixel(x + x_offset, y + y_offset);
                    new_pixel = new_pixel * val;
                    pixel = pixel + new_pixel;

                } else if (x + x_offset < 0 &&
                           y + y_offset >= 0 && y + y_offset < buf_h) {
                    new_pixel = buf->pixel(x + x_offset + buf_w, y + y_offset);
                    new_pixel = new_pixel * val;
                    pixel = new_pixel + pixel;

                } else if (x + x_offset > buf_w &&
                           y + y_offset >= 0 && y + y_offset < buf_h) {
                    new_pixel = buf->pixel(x + x_offset - buf_w, y + y_offset);
                    new_pixel = new_pixel * val;
                    pixel = new_pixel + pixel;

                } else if (x + x_offset >= 0 && x + x_offset < buf_w &&
                      y + y_offset < 0 ) {
                    new_pixel = buf->pixel(x + x_offset, y + y_offset + buf_h);
                    new_pixel = new_pixel * val;
                    pixel = new_pixel + pixel;

                } else if (x + x_offset >= 0 && x + x_offset < buf_w &&
                           y + y_offset > buf_h ) {
                    new_pixel = buf->pixel(x + x_offset, y + y_offset - buf_h);
                    new_pixel = new_pixel * val;
                    pixel = new_pixel + pixel;
                }
            }
        }
        pixel.Clamp();
        return pixel;
    }

    void ConvolutionFilter::CleanUpFilter() {}

    bool ConvolutionFilter::can_calculate_in_place() { return false; }
}  // namespace image_tools
