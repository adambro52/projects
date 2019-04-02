//
// Created by A-dizzle on 10/31/2018.
// Copyright 2018 Adam Smith
//

#ifndef IMAGETOOLS_CONVOLUTIONFILTEREDGE_H_
#define IMAGETOOLS_CONVOLUTIONFILTEREDGE_H_

#include "ConvolutionFilter.h"

namespace image_tools {

class ConvolutionFilterEdge : public ConvolutionFilter {
 public:

    /** This class inherits from the ConvolutionFilter class and creates the
     * kernel which will result in all of the edges in the original images to
     * be highlighted when its inhereted ApplyToBuffer is called in the
     * controller. */
    ConvolutionFilterEdge();
    virtual ~ConvolutionFilterEdge();

    /** this function overrides the function in ConvolutionFilter and produces
     * a 3x3 kernel with the values on the edge all equal to -1 and the entry
     * in the center equal to 8 */
    FloatMatrix* CreateKernel() override;
};
}  // namespace image_tools

#endif  //  IMAGETOOLS_CONVOLUTIONFILTEREDGE_H_
