//
// Created by A-dizzle on 10/31/2018.
// Copyright 2018 Adam Smith
//

#ifndef IMAGETOOLS_FILTERTHRESHOLD_H_
#define IMAGETOOLS_FILTERTHRESHOLD_H_

#include "imagetools/filter.h"
#include "imagetools/color_data.h"
#include "imagetools/float_matrix.h"
#include "imagetools/pixel_buffer.h"

namespace image_tools {

class FilterThreshold : public Filter {
 public:
    /** This class's version of the inhereted ApplyToBuffer method filters the
     * image by changing turning each pixel either black or white, decided in
     * its CalculateFilteredPixel method */
    FilterThreshold();
    virtual ~FilterThreshold();

    /** This version of CalculateFilteredPixel looks at the average intensity
     * across the RGB channels of a pixel and if that average is less than a
     * specified threshold value, then the pixel is set to black. Otherwise,
     * the pixel is set to white */
    ColorData CalculateFilteredPixel(PixelBuffer* buf, int x, int y) override;

    float thresh_;
};
}  // namespace image_tools

#endif  // IMAGETOOLS_FILTERTHRESHOLD_H_
