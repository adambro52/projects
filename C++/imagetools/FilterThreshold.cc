//
// Created by A-dizzle on 10/31/2018.
// Copyright 2018 Adam Smith
//

#include "imagetools/FilterThreshold.h"

namespace image_tools {

FilterThreshold::FilterThreshold(): thresh_(.5) {}
FilterThreshold::~FilterThreshold() {}

ColorData FilterThreshold::CalculateFilteredPixel(PixelBuffer *buf,
                                                  int x, int y) {
    ColorData pixel = buf->pixel(x, y);
    int avg = (pixel.blue() + pixel.red() + pixel.green()) / 3;
    if ( avg > thresh_ ) {
        pixel.set_blue(1);
        pixel.set_red(1);
        pixel.set_green(1);
    } else {
        pixel.set_blue(0);
        pixel.set_red(0);
        pixel.set_green(0);
    }

    pixel.Clamp();
    return pixel;
  }
}  // namespace image_tools
