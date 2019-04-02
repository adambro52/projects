//
// Created by A-dizzle on 10/31/2018.
// Copyright 2018 Adam Smith
//

#include "imagetools/FilterChannels.h"

namespace image_tools {

    FilterChannels::FilterChannels(): red_(1.0), green_(1.0), blue_(1.0) {}
    FilterChannels::~FilterChannels() {}

    ColorData FilterChannels::CalculateFilteredPixel(PixelBuffer* buf,
       int x, int y) {
        ColorData pixel = buf->pixel(x, y);

        pixel.set_red(pixel.red() * red_);
        pixel.set_red(pixel.green() * green_);
        pixel.set_red(pixel.blue() * blue_);

        pixel.Clamp();
        return pixel;
    }
}  // namespace image_tools
