//
// Created by A-dizzle on 10/31/2018.
//

#include "filter.h"
#include "pixel_buffer.h"
#include "color_data.h"

namespace image_tools {

Filter::Filter();
virtual Filter::~Filter(){}

void Filter::ApplyToBuffer(PixelBuffer *buf) {
    // do the other member functions get called in here? yes
    SetUpFilter();
    ColorData pixel;
    for ( int h = 0; h < buf.height(); h++ ){
      for ( int w = 0; w < buf.width(); w++ ){
        pixel = CalculateFilteredPixel( buf, w, h );
        buf.set_pixel( &pixel );
      }
    }
    CleanUpFilter();
  }

virtual void SetUpFilter(){}

virtual ColorData CalculateFilteredPixel(const PixelBuffer &buf, int x, int y) = 0;

virtual void CleanUpFilter(){}

virtual bool Filter::can_calculate_in_place() { return true; }
}
