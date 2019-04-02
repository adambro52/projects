//
// Created by A-dizzle on 10/31/2018.
// Copyright 2018 Adam Smith
//

#ifndef IMAGETOOLS_FILTERCHANNELS_H_
#define IMAGETOOLS_FILTERCHANNELS_H_

#include "imagetools/filter.h"
#include "imagetools/color_data.h"
#include "imagetools/float_matrix.h"
#include "imagetools/pixel_buffer.h"

namespace image_tools {

class FilterChannels : public Filter {
 public:
    /** This class's version of the inhereted ApplyToBuffer method filters the
     * image by changing its RGB channels, the amount of which is specified
     * by the user */
    FilterChannels();
    virtual ~FilterChannels();

    /** This version of CalculateFilteredPixel multiplies each RGB value in
     * the pixel by a specific value red, green, and blue scale factor that
     * is passed in from the controller */
    ColorData CalculateFilteredPixel(PixelBuffer* buf, int x, int y) override;

    float red_;
    float green_;
    float blue_;
};
}  // namespace image_tools

#endif  // IMAGETOOLS_FILTERCHANNELS_H_
