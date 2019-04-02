//
// Copyright by A-dizzle on 11/21/2018.
//

#include "imagetools/image_editor.h"

namespace image_tools {

ImageEditor::ImageEditor(PixelBuffer *buffer) {
    current_buffer_ = buffer;
}

ImageEditor::ImageEditor() {
    current_buffer_ = NULL;
}

ImageEditor::~ImageEditor() {
    if (current_buffer_ != NULL) delete current_buffer_;
}

void ImageEditor::LoadFromFile(const std::string &filename) {
    current_buffer_->LoadFromFile(filename);
}

void ImageEditor::SaveToFile(const std::string &filename) {
    current_buffer_->SaveToFile(filename);
}

// TOOLS

Tool* ImageEditor::GetToolByName(const std::string &name) {
    if (name == t_blur_.name()) {
        return &t_blur_;
    } else if (name == t_calligraphy_pen_.name()) {
        return &t_calligraphy_pen_;
    } else if (name == t_chalk_.name()) {
        return &t_chalk_;
    } else if (name == t_eraser_.name()) {
        return &t_eraser_;
    } else if (name == t_highlighter_.name()) {
        return &t_highlighter_;
    } else if (name == t_pen_.name()) {
        return &t_pen_;
    } else if (name == t_spray_can_.name()) {
        return &t_spray_can_;
      } else if (name == t_stamp_.name()) {
        return &t_stamp_;
    } else {
        return NULL;
    }
}

void ImageEditor::SaveStateForPossibleUndo() {
    PixelBuffer *buffer_copy = new PixelBuffer(*current_buffer_);
    saved_states_.push_front(buffer_copy);

    // remove the oldest undos if we've over our limit
    while (saved_states_.size() > max_undos_) {
        delete saved_states_.back();
        saved_states_.pop_back();
    }

    // committing a new state invalidates the states saved in the redo stack,
    // so, we simply clear out this stack.
    while (!undone_states_.empty()) {
        delete undone_states_.back();
        undone_states_.pop_back();
    }
}

void ImageEditor::StartStroke(const std::string &tool_name,
        const ColorData &color, float radius, int x, int y) {
    current_tool_ = GetToolByName(tool_name);
    tool_color_ = color;
    tool_radius_ = radius;
    if ((current_tool_) && (current_buffer_)) {
        SaveStateForPossibleUndo();
        current_tool_->StartStroke(current_buffer_, x, y, tool_color_,
                                   tool_radius_);
    }
}

void ImageEditor::AddToStroke(int x, int y) {
    if ((current_tool_) && (current_buffer_)) {
        current_tool_->AddToStroke(x, y);
    }
}

void ImageEditor::EndStroke(int x, int y) {
    if ((current_tool_) && (current_buffer_)) {
        current_tool_->EndStroke(x, y);
    }
}

// FILTERS

void ImageEditor::ApplyBlurFilter(float radius) {
    SaveStateForPossibleUndo();
    f_blur_ = ConvolutionFilterBlur();
    f_blur_.radius_ = radius;
    f_blur_.ApplyToBuffer(current_buffer_);
}

void ImageEditor::ApplyMotionBlurFilter(float radius, MBlurDir dir) {
    SaveStateForPossibleUndo();
    f_motion_blur_ = ConvolutionFilterMotionBlur();
    f_motion_blur_.rad_ = radius;
    f_motion_blur_.dir_ = MotionBlurDirectionName(dir);
    f_motion_blur_.ApplyToBuffer(current_buffer_);
}

void ImageEditor::ApplySharpenFilter(float radius) {
    SaveStateForPossibleUndo();
    f_sharpen_ = ConvolutionFilterSharpen();
    f_sharpen_.rad_ = radius;
    f_sharpen_.ApplyToBuffer(current_buffer_);
}

void ImageEditor::ApplyEdgeDetectFilter() {
    SaveStateForPossibleUndo();
    f_edge_ = ConvolutionFilterEdge();
    f_edge_.ApplyToBuffer(current_buffer_);
}

void ImageEditor::ApplyThresholdFilter(float cutoff_value) {
    SaveStateForPossibleUndo();
    f_threshold_ = FilterThreshold();
    f_threshold_.thresh_ = cutoff_value;
    f_threshold_.ApplyToBuffer(current_buffer_);
}

void ImageEditor::ApplySaturateFilter(float scale_factor) {
    SaveStateForPossibleUndo();
    f_saturate_ = FilterSaturate();
    f_saturate_.scale_ = scale_factor;
    f_saturate_.ApplyToBuffer(current_buffer_);
}

void ImageEditor::ApplyChannelsFilter(float red_scale, float green_scale,
                         float blue_scale) {
                             SaveStateForPossibleUndo();
    f_channels_ = FilterChannels();
    f_channels_.red_ = red_scale;
    f_channels_.green_ = green_scale;
    f_channels_.blue_ = blue_scale;
    f_channels_.ApplyToBuffer(current_buffer_);
}

void ImageEditor::ApplyQuantizeFilter(int num_bins) {
    SaveStateForPossibleUndo();
    f_quantize_ = FilterQuantize();
    f_quantize_.num_ = num_bins;
    f_quantize_.ApplyToBuffer(current_buffer_);
}

void ImageEditor::Undo() {
    if (can_undo()) {
        // save state for a possilbe redo
        undone_states_.push_front(current_buffer_);

        // make the top state on the undo stack the current one
        current_buffer_ = saved_states_.front();
        saved_states_.pop_front();
    }
}

void ImageEditor::Redo() {
    if (can_redo()) {
        // save state for a possible undo
        saved_states_.push_front(current_buffer_);

        // make the top state on the redo stack the current one
        current_buffer_ = undone_states_.front();
        undone_states_.pop_front();
    }
}

bool ImageEditor::can_undo() { return saved_states_.size(); }

bool ImageEditor::can_redo() { return undone_states_.size(); }

PixelBuffer* ImageEditor::pixel_buffer() { return current_buffer_; }

void ImageEditor::set_pixel_buffer(PixelBuffer *buffer) {
    current_buffer_ = buffer;
}
const std::map<ImageEditor::MBlurDir, std::string>
        ImageEditor::mblur_dir_names_ = {
        {MBLUR_DIR_N_S, "North/South"},
        {MBLUR_DIR_E_W, "East/West"},
        {MBLUR_DIR_NE_SW, "Northeast/Southwest"},
        {MBLUR_DIR_NW_SE, "Northwest/Southeast"}};

}  // namespace image_tools
