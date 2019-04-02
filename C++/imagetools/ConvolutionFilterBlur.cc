//
// Created by A-dizzle on 10/31/2018.
// Copyright 2018 Adam Smith

#include <cmath>
#include "imagetools/ConvolutionFilterBlur.h"
#include "imagetools/ConvolutionFilter.h"
#include "imagetools/float_matrix.h"
#include "imagetools/mask_factory.h"
#include "imagetools/image_tools_math.h"

namespace image_tools {

    ConvolutionFilterBlur::ConvolutionFilterBlur(): radius_(1.0) {}
    ConvolutionFilterBlur::~ConvolutionFilterBlur() {}

    FloatMatrix* ConvolutionFilterBlur::CreateKernel() {
        FloatMatrix* kernel = MaskFactory::CreateConstantMask(radius_);

        int center = floor(kernel->height() / 2);
        for (int x = 0; x < kernel->height(); x++) {
            for (int y = 0; y < kernel->width(); y++) {
                float dist = sqrt(pow((center - x), 2) + pow((center - y), 2));
                kernel->set_value(x, y,
                      ImageToolsMath::Gaussian(dist, center));
            }
        }

        kernel->Normalize();
        return kernel;
    }
}  // namespace image_tools
