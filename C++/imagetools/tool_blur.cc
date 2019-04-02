/**
This file is part of the CSCI-3081W Project Support Code, which was developed
at the University of Minnesota.

This code is to be used for student coursework.  It is not an open source
project.
Copyright (c) 2015-2018 Daniel Keefe, TAs, & Regents of the University of
Minnesota.

Original Author(s) of this File:
  Seth Johnson, 4/4/2015, University of Minnesota

Author(s) of Significant Updates/Modifications to the File:
  ...
*/

#include "imagetools/tool_blur.h"
#include "imagetools/mask_factory.h"

namespace image_tools {

ToolBlur::ToolBlur(): filterBlur(ConvolutionFilterBlur()) {
  // TODO(Students) setup your blur filter to use a radius of 5.0 here
  // ConvolutionFilterBlur filterBlur = ConvolutionFilterBlur();
  filterBlur.radius_ = 5.0;

  // the blur operation is not fast, so space the repeated applications of the
  // stamp out as far as we can get away with while still having it look good
  // 1/3 overlap is pretty good for this.
  stamp_overlap_ = 0.333;
}

ToolBlur::~ToolBlur() {
  // TODO(Students) cleanup your filter here if needed.
}

FloatMatrix* ToolBlur::CreateMask(float radius) {
  return MaskFactory::CreateLinearFalloffMask(radius);
}

ColorData ToolBlur::LookupPaintColor(int x, int y) {
  // TODO(Students) here's your hook to calculate a filtered version of the
  // pixel. Use your filter to compute the blurred version of the pixel at (x,y)
  // in *buffer_ and return the new color to make this tool work.
  ColorData pixel = filterBlur.CalculateFilteredPixel(buffer_, x, y);
  // Remove this:  As a placeholder, we're just returning the original pixel
  // color for now.
  return pixel;
}

} /* namespace image_tools */
