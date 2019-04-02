//
// Created by A-dizzle on 10/31/2018.
// Copyright 2018 Adam Smith
//

#ifndef IMAGETOOLS_FILTERSATURATE_H_
#define IMAGETOOLS_FILTERSATURATE_H_

#include "imagetools/filter.h"
#include "imagetools/color_data.h"
#include "imagetools/float_matrix.h"
#include "imagetools/pixel_buffer.h"

namespace image_tools {

class FilterSaturate : public Filter {
 public:

    /** This class's version of the inhereted ApplyToBuffer method filters the
     * image by changing the intensity of the colors in the image,
     * the amount of which is specified by the user */
    FilterSaturate();
    virtual ~FilterSaturate();

    /** This version of CalculateFilteredPixel does a linear interpolation on
     * each RGB value in the pixel with its human perceived luminance, its
     * current value, and a scale factor that is passed in from the
     * controller */
    ColorData CalculateFilteredPixel(PixelBuffer* buf, int x, int y) override;

    float scale_;
};
}  // namespace image_tools

#endif  // IMAGETOOLS_FILTERSATURATE_H_
