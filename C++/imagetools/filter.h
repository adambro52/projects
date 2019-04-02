//
// Created by A-dizzle on 10/31/2018.
// Copyright 2018 Adam Smith
//

#ifndef IMAGETOOLS_FILTER_H_
#define IMAGETOOLS_FILTER_H_

#include "imagetools/pixel_buffer.h"
#include "imagetools/color_data.h"
#include "imagetools/float_matrix.h"

namespace image_tools {

class Filter{
 public:
    Filter();
    virtual ~Filter();

    /** this function is used to iterate through the pixel buffer object
     * and for each pixel, it calls the CalculateFilteredPixel() function
     * to change the pixel. CalculateFilteredPixel() is overridden by
     * the subclasses of Filter */
    void ApplyToBuffer(PixelBuffer* buf);

    virtual void SetUpFilter();

    /** this function is overridden by all base classes and is used to correctly
     * calculate a pixel to implement the filter, it is used in the
     * ApplyToBuffer method and called at every position in teh image to set
     * each pixel to make the correct filter */
    virtual ColorData CalculateFilteredPixel(PixelBuffer* buf,
            int x, int y) = 0;

    virtual void CleanUpFilter();

    /** this indicates whether or not a copy needs to be passed into the
     * CalculateFilteredPixel() method as to ensure that the convolution
     * filters apply correctly*/
    virtual bool can_calculate_in_place();
};
}  // namespace image_tools

#endif  // IMAGETOOLS_FILTER_H_
