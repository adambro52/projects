//
// Created by A-dizzle on 10/31/2018.
// Copyright 2018 Adam Smith
//

#ifndef IMAGETOOLS_CONVOLUTIONFILTERMOTIONBLUR_H_
#define IMAGETOOLS_CONVOLUTIONFILTERMOTIONBLUR_H_

#include <string>
#include "ConvolutionFilter.h"

namespace image_tools {

class ConvolutionFilterMotionBlur : public ConvolutionFilter {
 public:
    float rad_;
    std::string dir_;

    /** This class inherits from the ConvolutionFilter class and creates the
     * kernel which will result in all of the edges in the original images to
     * be sharpened when its inhereted ApplyToBuffer is called in the
     * controller. */
    ConvolutionFilterMotionBlur();
    virtual ~ConvolutionFilterMotionBlur();

    /** this function overrides the function in ConvolutionFilter and produces
     * a kernel with a size specified by a value passed in by the user from
     * the controller and sets the values everywhere but the center entry all
     * equal to -1 and the entry in the center equal to the negated sum of all
     * the entries besides itself incremented by one */
    FloatMatrix* CreateKernel() override;
};
}  // namespace image_tools

#endif  // IMAGETOOLS_CONVOLUTIONFILTERMOTIONBLUR_H_
