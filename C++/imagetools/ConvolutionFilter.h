//
// Created by A-dizzle on 10/31/2018.
// Copyright 2018 Adam Smith
//

#ifndef IMAGETOOLS_CONVOLUTIONFILTER_H_
#define IMAGETOOLS_CONVOLUTIONFILTER_H_

#include "imagetools/filter.h"
#include "imagetools/mask_factory.h"

namespace image_tools {

class ConvolutionFilter : public Filter {
 private:
    /** this is what is used to calculate each pixel and is defined
     * differently in each sub-class. It will be iterated through in
     * CalculateFilteredPixel() to calculate what the filtered pixel will be
     * for each position in the image. */
    FloatMatrix* kernel_;
 public:
    /** This class inherets from the Filter class and implements each filter
     * using a Kernel which is defined in its sub-classes. */
    ConvolutionFilter();
    virtual ~ConvolutionFilter();

    /** this function is overridden by the sub classes to create different kinds
     * of kernels which this class implements in order to apply different kinds
     * of filters. */
    virtual FloatMatrix* CreateKernel() = 0;
    void SetUpFilter() override;

    /** this functrion is used to calculate what the new pixel will be based
     * off kernel_ variable which is defined in all of the subclasses. It
     * iterates through the kernel matrix and adds each value gotten by
     * multiplying the value in the matrix with the pixel in its respective
     * location to a running sum and then returns a pixel with its RGB
     * channels set to what is contained in the running sum. */
    ColorData CalculateFilteredPixel(PixelBuffer* buf,
       int x, int y) override;

    void CleanUpFilter() override;
    /** this function returns an indication that tells the instance whether
     * or not it needs to create a buffer copy. */
    bool can_calculate_in_place() override;
};
}  // namespace image_tools

#endif  // IMAGETOOLS_CONVOLUTIONFILTER_H_
