//
// Created by A-dizzle on 10/31/2018.
// Copyright 2018 Adam Smith
//

#ifndef IMAGETOOLS_FILTERQUANTIZE_H_
#define IMAGETOOLS_FILTERQUANTIZE_H_

#include "imagetools/filter.h"
#include "imagetools/color_data.h"
#include "imagetools/float_matrix.h"
#include "imagetools/pixel_buffer.h"

namespace image_tools {

class FilterQuantize : public Filter {
 public:

    /** This class's version of the inhereted ApplyToBuffer method filters the
     * image by changing the amount of unique colors in the image,
     * the amount of which is specified by the user */
    FilterQuantize();
    virtual ~FilterQuantize();

    /** This version of CalculateFilteredPixel multiplies each RGB value in
     * the pixel by a specific value red, green, and blue scale factor that
     * is passed in from the controller */
    ColorData CalculateFilteredPixel(PixelBuffer* buf, int x, int y) override;

    float num_;
};
}  // namespace image_tools

#endif  // IMAGETOOLS_FILTERQUANTIZE_H_
