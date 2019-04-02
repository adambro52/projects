//
// Created by A-dizzle on 10/31/2018.
// Copyright 2018 Adam Smith
//

#include "imagetools/ConvolutionFilterMotionBlur.h"
#include "imagetools/ConvolutionFilter.h"
#include "imagetools/float_matrix.h"

namespace image_tools {

    ConvolutionFilterMotionBlur::ConvolutionFilterMotionBlur():
                rad_(1.0), dir_("North/South") {}
    ConvolutionFilterMotionBlur::~ConvolutionFilterMotionBlur() {}

    FloatMatrix* ConvolutionFilterMotionBlur::CreateKernel() {
        // create and return a float matrix that will create a Motionblur effect
        FloatMatrix* kernel = MaskFactory::CreateConstantMask(rad_);
        int height = kernel->height();
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < height; x++) {
                if (dir_ == "Northwest/Southeast" && x != y) {
                    kernel->set_value(x, y, 0.0);
                }
                if (dir_ == "North/South" && x != height / 2) {
                    kernel->set_value(x, y, 0.0);
                }
                if (dir_ == "East/West" && y != height / 2) {
                    kernel->set_value(x, y, 0.0);
                }
                if (dir_ == "Northeast/Southwest" && y != height - x) {
                        kernel->set_value(x, y, 0.0);
                }
            }
        }
        kernel->Normalize();
        return kernel;
    }
}  // namespace image_tools
