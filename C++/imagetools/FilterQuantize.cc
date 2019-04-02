//
// Created by A-dizzle on 10/31/2018.
// Copyright 2018 Adam Smith
//

#include "imagetools/FilterQuantize.h"
#include <cmath>

namespace image_tools {

    FilterQuantize::FilterQuantize(): num_(.5) {}
    FilterQuantize::~FilterQuantize() {}

    ColorData FilterQuantize::CalculateFilteredPixel(PixelBuffer* buf,
       int x, int y) {
        ColorData pixel = buf->pixel(x, y);

        float steps = num_ - 1;
        pixel.set_red(round(pixel.red() * steps) / steps);
        pixel.set_green(round(pixel.green() * steps) / steps);
        pixel.set_blue(round(pixel.blue() * steps) / steps);


        pixel.Clamp();
        return pixel;
    }
}  // namespace image_tools
