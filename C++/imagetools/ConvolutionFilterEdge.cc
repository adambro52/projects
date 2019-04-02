//
// Created by A-dizzle on 10/31/2018.
// Copyright 2018 Adam Smith
//

#include "imagetools/ConvolutionFilterEdge.h"
#include "imagetools/ConvolutionFilter.h"
#include "imagetools/float_matrix.h"

namespace image_tools {

    ConvolutionFilterEdge::ConvolutionFilterEdge() {}
    ConvolutionFilterEdge::~ConvolutionFilterEdge() {}

    FloatMatrix* ConvolutionFilterEdge::CreateKernel() {
        // create and return a float matrix that will create a Edge effect
        FloatMatrix* kernel = MaskFactory::CreateConstantMask(1.0);
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                kernel->set_value(i, j, -1);
            }
        }
        kernel->set_value(1, 1, 8);
        kernel->Normalize();
        return kernel;
    }
}  // namespace image_tools
