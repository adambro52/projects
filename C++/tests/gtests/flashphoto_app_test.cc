// Copyright (c) 2015-2018 Daniel Keefe, TAs, & Regents of the University of
// Minnesota.

#include <iostream>
#include "gtest/gtest.h"
#include "flashphoto/flashphoto_app.h"
#include "imagetools/pixel_buffer.h"
#include "mia/image_editor_commands.h"

using image_tools::FlashPhotoApp;

class FlashPhotoAppTest : public ::testing::Test {
    void SetUp() override {
        app_ = new FlashPhotoApp(1024, 780, image_tools::ColorData(1, 1, 1));
    }

    void TearDown() override {
        delete app_;
    }

 protected:
    FlashPhotoApp *app_;
};

TEST_F(FlashPhotoAppTest, GetToolTest) {
image_tools::Tool* pen = app_->GetToolByName("Pen");
EXPECT_TRUE(pen);
image_tools::Tool* unknownTool = app_->GetToolByName("Unknown");
EXPECT_FALSE(unknownTool);
}

TEST_F(FlashPhotoAppTest, BlurRegressionTest) {
    bool same = true;

    image_tools::ImageEditor *i_editor = new image_tools::ImageEditor();
    image_tools::PixelBuffer test =
            image_tools::PixelBuffer(
  "/home/smit8796/csci3081/repo-smit8796/PROJ/resources/fruit2.png");
    i_editor->set_pixel_buffer(&test);

    i_editor->ApplyBlurFilter(2);
    test = image_tools::PixelBuffer(
            "/home/smit8796/csci3081/repo-smit8796/PROJ/resources/blur2.png");
    if (*i_editor->pixel_buffer() != test) {
        same = false;
    }
    EXPECT_TRUE(same);
}

TEST_F(FlashPhotoAppTest, MotionBlurRegressionTest) {
    bool same = true;

    image_tools::ImageEditor* i_editor = new image_tools::ImageEditor();
    image_tools::PixelBuffer test =
            image_tools::PixelBuffer(
         "/home/smit8796/csci3081/repo-smit8796/PROJ/resources/fruit2.png");
    i_editor->set_pixel_buffer(&test);

    i_editor->ApplyMotionBlurFilter(2,
    image_tools::ImageEditor::MBLUR_DIR_N_S);
    test = image_tools::PixelBuffer(
"/home/smit8796/csci3081/repo-smit8796/PROJ/resources/motionblur-n-s2.png");
    if (*i_editor->pixel_buffer() != test) {
        same = false;
    }

    EXPECT_TRUE(same);
}

TEST_F(FlashPhotoAppTest, SharpenRegressionTest) {
    bool same = true;

    image_tools::ImageEditor* i_editor = new image_tools::ImageEditor();
    image_tools::PixelBuffer test =
            image_tools::PixelBuffer(
      "/home/smit8796/csci3081/repo-smit8796/PROJ/resources/fruit2.png");
    i_editor->set_pixel_buffer(&test);

    i_editor->ApplySharpenFilter(2);
    test = image_tools::PixelBuffer(
      "/home/smit8796/csci3081/repo-smit8796/PROJ/resources/sharpen2.png");
    if (*i_editor->pixel_buffer() != test) {
        same = false;
    }
    EXPECT_TRUE(same);
}

TEST_F(FlashPhotoAppTest, EdgeRegressionTest) {
    bool same = true;

    image_tools::ImageEditor* i_editor = new image_tools::ImageEditor();
    image_tools::PixelBuffer test =
            image_tools::PixelBuffer(
        "/home/smit8796/csci3081/repo-smit8796/PROJ/resources/fruit2.png");
    i_editor->set_pixel_buffer(&test);

    i_editor->ApplyEdgeDetectFilter();

    test = image_tools::PixelBuffer(
            "/home/smit8796/csci3081/repo-smit8796/PROJ/resources/edge.png");
    if (*i_editor->pixel_buffer() != test) {
        same = false;
    }
    EXPECT_TRUE(same);
}

TEST_F(FlashPhotoAppTest, ThresholdRegressionTest) {
    bool same = true;

    image_tools::ImageEditor* i_editor = new image_tools::ImageEditor();
    image_tools::PixelBuffer test =
            image_tools::PixelBuffer(
       "/home/smit8796/csci3081/repo-smit8796/PROJ/resources/fruit2.png");
    i_editor->set_pixel_buffer(&test);

    i_editor->ApplyThresholdFilter(.8);
    test = image_tools::PixelBuffer(
       "/home/smit8796/csci3081/repo-smit8796/PROJ/resources/threshold8.png");
    if (*i_editor->pixel_buffer() != test) {
        same = false;
    }
    EXPECT_TRUE(same);
}

TEST_F(FlashPhotoAppTest, SaturateRegressionTest) {
    bool same = true;

    image_tools::ImageEditor* i_editor = new image_tools::ImageEditor();
    image_tools::PixelBuffer test =
            image_tools::PixelBuffer(
            "/home/smit8796/csci3081/repo-smit8796/PROJ/resources/fruit2.png");
    i_editor->set_pixel_buffer(&test);

    i_editor->ApplySaturateFilter(2);
    test = image_tools::PixelBuffer(
       "/home/smit8796/csci3081/repo-smit8796/PROJ/resources/saturate2.png");
    if (*i_editor->pixel_buffer() != test) {
        same = false;
    }
    EXPECT_TRUE(same);
}

TEST_F(FlashPhotoAppTest, ChannelsRegressionTest) {
    bool same = true;

    image_tools::ImageEditor* i_editor = new image_tools::ImageEditor();
    image_tools::PixelBuffer test =
            image_tools::PixelBuffer(
            "/home/smit8796/csci3081/repo-smit8796/PROJ/resources/fruit2.png");
    i_editor->set_pixel_buffer(&test);

    i_editor->ApplyChannelsFilter(2, 1, 1);
    test = image_tools::PixelBuffer(
            "/home/smit8796/csci3081/repo-smit8796/PROJ/resources/red2.png");
    if (*i_editor->pixel_buffer() != test) {
        same = false;
    }
//    std::cout << "j" <<
//    std::endl;
//
//    i_editor->Undo();
//
//    i_editor->ApplyChannelsFilter(1, 2, 1);
//    test = image_tools::PixelBuffer(
//        "/home/smit8796/csci3081/repo-smit8796/PROJ/resources/green2.png");
//    if (*i_editor->pixel_buffer() != test){
//        same = false;
//    }
//    std::cout << "k" <<
//    std::endl;
//
//    i_editor->Undo();
//
//    i_editor->ApplyChannelsFilter(1, 1, 2);
//    test = image_tools::PixelBuffer(
//         "/home/smit8796/csci3081/repo-smit8796/PROJ/resources/blue2.png");
//    if (*i_editor->pixel_buffer() != test){
//        same = false;
//    }
//    std::cout << "l" <<
//    std::endl;
    EXPECT_TRUE(same);
}

TEST_F(FlashPhotoAppTest, QuantizeRegressionTest) {
    bool same = true;

    image_tools::ImageEditor *i_editor = new image_tools::ImageEditor();
    image_tools::PixelBuffer test =
            image_tools::PixelBuffer(
            "/home/smit8796/csci3081/repo-smit8796/PROJ/resources/fruit2.png");
    i_editor->set_pixel_buffer(&test);

    i_editor->ApplyQuantizeFilter(2);
    test = image_tools::PixelBuffer(
         "/home/smit8796/csci3081/repo-smit8796/PROJ/resources/quantize2.png");
    if (*i_editor->pixel_buffer() != test) {
        same = false;
    }
    EXPECT_TRUE(same);
}

TEST_F(FlashPhotoAppTest, BlurToolRegressionTest) {
    bool same = true;

    image_tools::ImageEditor *i_editor = new image_tools::ImageEditor();
    image_tools::PixelBuffer test =
            image_tools::PixelBuffer(
             "/home/smit8796/csci3081/repo-smit8796/PROJ/resources/fruit2.png");
    i_editor->set_pixel_buffer(&test);


    image_tools::AddToStrokeCommand add =
            image_tools::AddToStrokeCommand(i_editor, 200, 200);
    image_tools::EndStrokeCommand end =
            image_tools::EndStrokeCommand(i_editor, 200, 200);

    test = image_tools::PixelBuffer(
            "/home/smit8796/csci3081/repo-smit8796/PROJ/"
                    "resources/r10-xy100-200-blur-fruit.png");
    image_tools::StartStrokeCommand start =
            image_tools::StartStrokeCommand(i_editor, "Blur",
                              image_tools::ColorData(0, 0, 0), 10.0, 100, 100);
    start.Execute();

    add.Execute();

    end.Execute();

    if (*i_editor->pixel_buffer() != test) {
        same = false;
    }
    EXPECT_TRUE(same);
}

TEST_F(FlashPhotoAppTest, CalligraphyPenRegressionTest) {
    bool same = true;

    image_tools::ImageEditor *i_editor = new image_tools::ImageEditor();
    image_tools::PixelBuffer test =
            image_tools::PixelBuffer(
            "/home/smit8796/csci3081/repo-smit8796/PROJ/resources/fruit2.png");
    i_editor->set_pixel_buffer(&test);


    image_tools::AddToStrokeCommand add =
            image_tools::AddToStrokeCommand(i_editor, 200, 200);
    image_tools::EndStrokeCommand end =
            image_tools::EndStrokeCommand(i_editor, 200, 200);
    test = image_tools::PixelBuffer(
            "/home/smit8796/csci3081/repo-smit8796/PROJ/"
                    "resources/r10-xy100-200-calligraphy-pen-fruit.png");
    image_tools::StartStrokeCommand start1 =
            image_tools::StartStrokeCommand(i_editor, "Calligraphy Pen",
                            image_tools::ColorData(0, 0, 0), 10.0, 100, 100);
    start1.Execute();

    add.Execute();

    end.Execute();

    if (*i_editor->pixel_buffer() != test) {
        same = false;
    }
    EXPECT_TRUE(same);
}

TEST_F(FlashPhotoAppTest, ChalkRegressionTest) {
    bool same = true;

    image_tools::ImageEditor *i_editor = new image_tools::ImageEditor();
    image_tools::PixelBuffer test =
            image_tools::PixelBuffer(
             "/home/smit8796/csci3081/repo-smit8796/PROJ/resources/fruit2.png");
    i_editor->set_pixel_buffer(&test);


    image_tools::AddToStrokeCommand add =
            image_tools::AddToStrokeCommand(i_editor, 200, 200);
    image_tools::EndStrokeCommand end =
            image_tools::EndStrokeCommand(i_editor, 200, 200);

    test = image_tools::PixelBuffer(
            "/home/smit8796/csci3081/repo-smit8796"
                    "/PROJ/resources/r10-xy100-200-chalk-fruit.png");
    image_tools::StartStrokeCommand start2 =
            image_tools::StartStrokeCommand(i_editor, "Chalk",
                              image_tools::ColorData(0, 0, 0), 10.0, 100, 100);
    start2.Execute();

    add.Execute();

    end.Execute();

    if (*i_editor->pixel_buffer() != test) {
        same = false;
    }
    EXPECT_TRUE(same);
}

TEST_F(FlashPhotoAppTest, EraserRegressionTest) {
    bool same = true;

    image_tools::ImageEditor *i_editor = new image_tools::ImageEditor();
    image_tools::PixelBuffer test =
            image_tools::PixelBuffer(
             "/home/smit8796/csci3081/repo-smit8796/PROJ/resources/fruit2.png");
    i_editor->set_pixel_buffer(&test);


    image_tools::AddToStrokeCommand add =
            image_tools::AddToStrokeCommand(i_editor, 200, 200);
    image_tools::EndStrokeCommand end =
            image_tools::EndStrokeCommand(i_editor, 200, 200);

    test = image_tools::PixelBuffer(
            "/home/smit8796/csci3081/repo-smit8796/"
                    "PROJ/resources/r10-xy100-200-eraser-fruit.png");
    image_tools::StartStrokeCommand start3 =
            image_tools::StartStrokeCommand(i_editor, "Eraser",
                           image_tools::ColorData(0, 0, 0), 10.0, 100, 100);
    start3.Execute();

    add.Execute();

    end.Execute();

    if (*i_editor->pixel_buffer() != test) {
        same = false;
    }
    EXPECT_TRUE(same);
}

TEST_F(FlashPhotoAppTest, HighlighterRegressionTest) {
    bool same = true;

    image_tools::ImageEditor *i_editor = new image_tools::ImageEditor();
    image_tools::PixelBuffer test =
            image_tools::PixelBuffer(
            "/home/smit8796/csci3081/repo-smit8796/PROJ/resources/fruit2.png");
    i_editor->set_pixel_buffer(&test);


    image_tools::AddToStrokeCommand add =
            image_tools::AddToStrokeCommand(i_editor, 200, 200);
    image_tools::EndStrokeCommand end =
            image_tools::EndStrokeCommand(i_editor, 200, 200);

    test = image_tools::PixelBuffer(
            "/home/smit8796/csci3081/repo-smit8796/"
                    "PROJ/resources/r10-xy100-200-highlighter-fruit.png");
    image_tools::StartStrokeCommand start4 =
            image_tools::StartStrokeCommand(i_editor, "Highlighter",
                            image_tools::ColorData(0, 0, 0), 10.0, 100, 100);
    start4.Execute();

    add.Execute();

    end.Execute();

    if (*i_editor->pixel_buffer() != test) {
        same = false;
    }
    EXPECT_TRUE(same);
}

TEST_F(FlashPhotoAppTest, PenRegressionTest) {
    bool same = true;

    image_tools::ImageEditor *i_editor = new image_tools::ImageEditor();
    image_tools::PixelBuffer test =
            image_tools::PixelBuffer(
         "/home/smit8796/csci3081/repo-smit8796/PROJ/resources/fruit2.png");
    i_editor->set_pixel_buffer(&test);


    image_tools::AddToStrokeCommand add =
            image_tools::AddToStrokeCommand(i_editor, 200, 200);
    image_tools::EndStrokeCommand end =
            image_tools::EndStrokeCommand(i_editor, 200, 200);

    test = image_tools::PixelBuffer(
            "/home/smit8796/csci3081/repo-smit8796/"
                    "PROJ/resources/r10-xy100-200-pen-fruit.png");
    image_tools::StartStrokeCommand start5 =
            image_tools::StartStrokeCommand(i_editor, "Pen",
                           image_tools::ColorData(0, 0, 0), 10.0, 100, 100);
    start5.Execute();

    add.Execute();

    end.Execute();

    if (*i_editor->pixel_buffer() != test) {
        same = false;
    }
    EXPECT_TRUE(same);
}

TEST_F(FlashPhotoAppTest, SprayCanRegressionTest) {
    bool same = true;

    image_tools::ImageEditor *i_editor = new image_tools::ImageEditor();
    image_tools::PixelBuffer test =
            image_tools::PixelBuffer(
          "/home/smit8796/csci3081/repo-smit8796/PROJ/resources/fruit2.png");
    i_editor->set_pixel_buffer(&test);


    image_tools::AddToStrokeCommand add =
            image_tools::AddToStrokeCommand(i_editor, 200, 200);
    image_tools::EndStrokeCommand end =
            image_tools::EndStrokeCommand(i_editor, 200, 200);

    test = image_tools::PixelBuffer(
            "/home/smit8796/csci3081/repo-smit8796/"
                    "PROJ/resources/r10-xy100-200-spray-can-fruit.png");
    image_tools::StartStrokeCommand start6 =
            image_tools::StartStrokeCommand(i_editor, "Spray Can",
                           image_tools::ColorData(0, 0, 0), 10.0, 100, 100);
    start6.Execute();

    add.Execute();

    end.Execute();

    if (*i_editor->pixel_buffer() != test) {
        same = false;
    }
    EXPECT_TRUE(same);
}

TEST_F(FlashPhotoAppTest, StampRegressionTest) {
    bool same = true;

    image_tools::ImageEditor *i_editor = new image_tools::ImageEditor();
    image_tools::PixelBuffer test =
            image_tools::PixelBuffer(
          "/home/smit8796/csci3081/repo-smit8796/PROJ/resources/fruit2.png");
    i_editor->set_pixel_buffer(&test);


    image_tools::AddToStrokeCommand add =
            image_tools::AddToStrokeCommand(i_editor, 200, 200);
    image_tools::EndStrokeCommand end =
            image_tools::EndStrokeCommand(i_editor, 200, 200);

    test = image_tools::PixelBuffer(
            "/home/smit8796/csci3081/repo-smit8796/"
                    "PROJ/resources/r10-xy100-200-stamp-fruit.png");
    image_tools::StartStrokeCommand start7 =
            image_tools::StartStrokeCommand(i_editor, "Stamp",
                          image_tools::ColorData(0, 0, 0), 10.0, 100, 100);
    start7.Execute();
    end.Execute();

    if (*i_editor->pixel_buffer() != test) {
        same = false;
    }

    EXPECT_TRUE(same);
}
