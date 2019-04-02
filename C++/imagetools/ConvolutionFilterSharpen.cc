//
// Created by A-dizzle on 10/31/2018.
// Copyright 2018 Adam Smith
//

#include <cmath>
#include "imagetools/ConvolutionFilterSharpen.h"
#include "imagetools/ConvolutionFilter.h"
#include "imagetools/float_matrix.h"

namespace image_tools {

    ConvolutionFilterSharpen::ConvolutionFilterSharpen(): rad_(1.0) {}
    ConvolutionFilterSharpen::~ConvolutionFilterSharpen() {}

    FloatMatrix* ConvolutionFilterSharpen::CreateKernel() {
        // create and return a float matrix that will create a Sharpen
        FloatMatrix* kernel = MaskFactory::CreateConstantMask(rad_);
        for (int i = 0; i < kernel->height(); i++) {
            for (int j = 0; j < kernel->width(); j++) {
                kernel->set_value(i, j, -1);
            }
        }
        float sum = kernel->Sum();
        kernel->set_value(floor(kernel->height() / 2),
                floor(kernel->width() / 2), -sum);
        kernel->Normalize();
        return kernel;
    }
}  // namespace image_tools
