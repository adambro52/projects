//
// Created by A-dizzle on 10/31/2018.
// Copyright 2018 Adam Smith
//

#ifndef IMAGETOOLS_CONVOLUTIONFILTERBLUR_H_
#define IMAGETOOLS_CONVOLUTIONFILTERBLUR_H_

#include "ConvolutionFilter.h"

namespace image_tools {

class ConvolutionFilterBlur : public ConvolutionFilter {
 public:
    float radius_;

    /** This class inherits from the ConvolutionFilter class and creates the
     * kernel which will result in a Gaussian blur when its inhereted
     * ApplyToBuffer is called in the controller. */
    ConvolutionFilterBlur();
    virtual ~ConvolutionFilterBlur();

    /** this function overrides the function in ConvolutionFilter and produces
     * a blur kernel by having each entry in the kernel equal to the value of
     * the Gaussian function calculated with the pixel's Euclidean distance
     * from the center of the kernel*/
    FloatMatrix* CreateKernel() override;
};
}  // namespace image_tools

#endif  //  IMAGETOOLS_CONVOLUTIONFILTERBLUR_H_
