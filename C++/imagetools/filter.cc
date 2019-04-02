//
// Created by A-dizzle on 10/31/2018.
// Copyright 2018 Adam Smith
//

#include "imagetools/filter.h"
#include "imagetools/pixel_buffer.h"
#include "imagetools/color_data.h"

namespace image_tools {

    Filter::Filter() {}
    Filter::~Filter() {}

    void Filter::SetUpFilter() {}

    void Filter::CleanUpFilter() {}

    void Filter::ApplyToBuffer(PixelBuffer* buf) {
        SetUpFilter();

        if (!can_calculate_in_place()) {
            PixelBuffer buf_copy = PixelBuffer(*buf);
            ColorData pixel;
            for (int h = 0; h < buf->height(); h++) {
                for (int w = 0; w < buf->width(); w++) {
                    pixel = CalculateFilteredPixel(&buf_copy, w, h);
                    buf->set_pixel(w, h, pixel);
                }
            }
        } else {
            ColorData pixel;
            for (int h = 0; h < buf->height(); h++) {
                for (int w = 0; w < buf->width(); w++) {
                    pixel = CalculateFilteredPixel(buf, w, h);
                    buf->set_pixel(w, h, pixel);
                }
            }
        }

        CleanUpFilter();
    }

    bool Filter::can_calculate_in_place() { return true; }
}  // namespace image_tools
