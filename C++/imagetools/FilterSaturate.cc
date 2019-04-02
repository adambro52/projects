//
// Created by A-dizzle on 10/31/2018.
// Copyright 2018 Adam Smith
//

#include "imagetools/FilterSaturate.h"
#include "imagetools/image_tools_math.h"

namespace image_tools {

    FilterSaturate::FilterSaturate(): scale_(.5) {}
    FilterSaturate::~FilterSaturate() {}

    ColorData FilterSaturate::CalculateFilteredPixel(PixelBuffer* buf,
       int x, int y) {
        ColorData pixel = buf->pixel(x, y);
        float lum = pixel.Luminance();

        pixel.set_red(ImageToolsMath::Lerp(lum, pixel.red(), scale_));
        pixel.set_green(ImageToolsMath::Lerp(lum, pixel.green(), scale_));
        pixel.set_blue(ImageToolsMath::Lerp(lum, pixel.blue(), scale_));

        pixel.Clamp();
        return pixel;
    }
}  // namespace image_tools
