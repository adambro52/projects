//
// Copyright by A-dizzle on 11/21/2018.
//

#include "mia/command_line_processor.h"
#include <mingfx.h>
#include <string>
#include <vector>

namespace image_tools {

CommandLineProcessor::CommandLineProcessor() {}
CommandLineProcessor::~CommandLineProcessor() {}

void CommandLineProcessor::ProcessCommandLine(int argc, char* argv[]) {
    // parse argv for the options provided and map them to their relevant
    // image_editor_command, if there are no bad commands in argv then
    // execute all of them otherwise print out help message

    std::string token = argv[1];
    std::size_t pos = token.find(".png");
    std::string token1 = argv[argc-1];
    std::size_t pos1 = token1.find(".png");
//    if (!mingfx::Platform::FileExists(token)) {
//       std::cout << "invalid input file\n" << std::endl;
//    } else
        if (token == "-h") {
      // std::cout << "-h command1: " << token << std::endl;
        printHelp();
    } else if (pos1 != token1.length() - 4) {
//       std::cout << "bad output file type: " << token1 << std::endl;
        printHelp();
    } else if (pos != token.length() - 4) {
//       std::cout << "bad input file type: " << token << std::endl;
            printHelp();  // checks to make sure that first and last
        // args are a file that ends in .png
    } else {
        if ( argc - 3 > 0 ) {
            std::vector<std::string> input;
            bool bad_input = false;
            for (int i = 2; i < argc - 1; ++i) {
                int count = 0;
                token = argv[i];
                for (int j = 0; j < static_cast<int>(token.length()); ++j) {
                    // check to make sure each arg is only digits or chars
                    if (isdigit(static_cast<int>(argv[i][j])) ||
                        argv[i][j] == '.') {
                        count++;
                    }
                }
                // if token only contains characters or ints
                if (count == 0 || count == static_cast<int>(token.length())) {
                    input.push_back(token);
                } else {
//           std::cout << "not only none or all digits: " << token << std::endl;
                    bad_input = true;
                }
            }

            if (bad_input) {
                printHelp();
//                std::cout << "Natasha\n";
            } else {
                std::string infile = argv[1];
                std::string outfile = argv[argc - 1];
                std::vector < ImageEditorCommand * > cmds;
                PixelBuffer *buf = new PixelBuffer(
                        300, 300, ColorData(1, 1, 1));
                ImageEditor *imageEditor = new ImageEditor(buf);
                bad_input = validCommands(input, argc - 1);
//                std::cout << "no segfault yet\n";
                if (bad_input) {
                    printHelp();
                } else {
                    ImageEditorCommand* cmd = new LoadCommand(
                            imageEditor, infile);
                    cmds.push_back(cmd);
                    cmds = getCommands(imageEditor, cmds, input, argc);
//                    std::cout << "still no segfault\n";
                    ImageEditorCommand* cmd1 = new SaveCommand(
                            imageEditor, outfile);
                    cmds.push_back(cmd1);
                    for (int i = 0; i < static_cast<int>(cmds.size()); i++) {
                        cmds[i]->Execute();
                        delete cmds[i];
                    }
                }
                delete imageEditor;
            }
        } else {
            std::string infile = argv[1];
            std::string outfile = argv[argc - 1];
            PixelBuffer *buf = new PixelBuffer(300, 300, ColorData(1, 1, 1));
            ImageEditor* imageEditor = new ImageEditor(buf);
            LoadCommand cmd = LoadCommand(imageEditor, infile);
            cmd.Execute();
            SaveCommand cmd1 = SaveCommand(imageEditor, outfile);
            cmd1.Execute();
            delete imageEditor;
        }
    }
}

std::vector<ImageEditorCommand*> CommandLineProcessor::getCommands(
        ImageEditor* i_editor, std::vector<ImageEditorCommand*> cmds,
        std::vector<std::string> input, int argc) {
    ImageEditorCommand *temp;
    float result = -1;
    for (int i = 0; i < argc-3 ; ++i) {
        if (!input[i].compare("-blur") &&
            String2Float(input[i+1], result)) {
            std::cout << "added blur with commands: " << std::endl;
            std::cout << input[i] << " " << input[i+1] << std::endl;
            temp = new BlurFilterCommand(i_editor, result);
            cmds.push_back(temp);
            i++;
        } else if (!input[i].compare("-sharpen") &&
                 String2Float(input[i+1], result)) {
            std::cout << "added sharpen with commands: " << std::endl;
            std::cout << input[i] << " " << input[i+1] << std::endl;
            temp = new SharpenFilterCommand(i_editor, result);
            cmds.push_back(temp);
      i++;
        } else if (!input[i].compare("-edgedetect")) {
            std::cout << "added edgedetect with commands: " << std::endl;
            std::cout << input[i] << std::endl;
            temp = new EdgeFilterCommand(i_editor);
            cmds.push_back(temp);
        } else if (!input[i].compare("-red") &&
                 String2Float(input[i+1], result)) {
            std::cout << "added red with commands: " << std::endl;
            std::cout << input[i] << " " << input[i+1] << std::endl;
            temp = new ChannelsFilterCommand(i_editor, result, 1, 1);
            cmds.push_back(temp);
      i++;
        } else if (!input[i].compare("-green") &&
                 String2Float(input[i+1], result)) {
            std::cout << "added green with commands: " << std::endl;
            std::cout << input[i] << " " << input[i+1] << std::endl;
            temp = new ChannelsFilterCommand(i_editor, 1, result, 1);
            cmds.push_back(temp);
      i++;
        } else if (!input[i].compare("-blue") &&
                 String2Float(input[i+1], result)) {
            std::cout << "added blue with commands: " << std::endl;
            std::cout << input[i] << " " << input[i+1] << std::endl;
            temp = new ChannelsFilterCommand(i_editor, 1, 1, result);
            cmds.push_back(temp);
      i++;
        } else if (!input[i].compare("-quantize") &&
                 String2Float(input[i+1], result)) {
            std::cout << "added quantize with commands: " << std::endl;
            std::cout << input[i] << " " << input[i+1] << std::endl;
            temp = new QuantizeFilterCommand(i_editor, result);
            cmds.push_back(temp);
      i++;
        } else if (!input[i].compare("-saturate") &&
                 String2Float(input[i+1], result)) {
            std::cout << "added saturate with commands: " << std::endl;
            std::cout << input[i] << " " << input[i+1] << std::endl;
            temp = new SaturateFilterCommand(i_editor, result);
            cmds.push_back(temp);
      i++;
        } else if (!input[i].compare("-threshold") &&
                 String2Float(input[i+1], result)) {
            std::cout << "added threshold with commands: " << std::endl;
            std::cout << input[i] << " " << input[i+1] << std::endl;
            temp = new ThresholdFilterCommand(i_editor, result);
            cmds.push_back(temp);
      i++;
        } else if (!input[i].compare("-motionblur-n-s") &&
                 String2Float(input[i+1], result)) {
            std::cout << "added motionblur-n-s with commands: " << std::endl;
            std::cout << input[i] << " " << input[i+1] << std::endl;
            temp = new MotionBlurFilterCommand(i_editor, result,
                            ImageEditor::MBLUR_DIR_N_S);
            cmds.push_back(temp);
      i++;
        } else if (!input[i].compare("-motionblur-e-w") &&
                 String2Float(input[i+1], result)) {
            std::cout << "added motionblur-e-w with commands: " << std::endl;
            std::cout << input[i] << " " << input[i+1] << std::endl;
            temp = new MotionBlurFilterCommand(i_editor, result,
                            ImageEditor::MBLUR_DIR_E_W);
            cmds.push_back(temp);
      i++;
        } else if (!input[i].compare("-motionblur-ne-sw") &&
                 String2Float(input[i+1], result)) {
            std::cout << "added motionblur-ne-sw with commands: " << std::endl;
            std::cout << input[i] << " " << input[i+1] << std::endl;
            temp = new MotionBlurFilterCommand(i_editor, result,
                            ImageEditor::MBLUR_DIR_NE_SW);
            cmds.push_back(temp);
      i++;
        } else if (!input[i].compare("-motionblur-nw-se") &&
                 String2Float(input[i+1], result)) {
            std::cout << "added motionblur-nw-se with commands: " << std::endl;
            std::cout << input[i] << " " << input[i+1] << std::endl;
            temp = new MotionBlurFilterCommand(i_editor, result,
                            ImageEditor::MBLUR_DIR_NW_SE);
            cmds.push_back(temp);
      i++;
        }
// else if (!input[i].compare("-tool-blur")) {
//     std::cout << "made tool blur stroke from (x, y) == (100, 100) " <<
//               "to (x, y) == (200, 200)" << std::endl;
//     temp = new StartStrokeCommand(i_editor, "Blur",
//                   ColorData(0, 0, 0), 10.0, 100, 100);
//     cmds.push_back(temp);
//     temp = new AddToStrokeCommand(i_editor, 200, 200);
//     cmds.push_back(temp);
//     temp = new EndStrokeCommand(i_editor, 200, 200);
//     cmds.push_back(temp);
//     i++;
// }
// else if (!input[i].compare("-tool-calligraphy-pen")) {
//     std::cout << "made tool blur stroke from (x, y) == (100, 100) " <<
//               "to (x, y) == (200, 200)" << std::endl;
//     temp = new StartStrokeCommand(i_editor, "Caligraphy Pen",
//                                   ColorData(0, 0, 0), 10.0, 100, 100);
//     cmds.push_back(temp);
//     temp = new AddToStrokeCommand(i_editor, 200, 200);
//     cmds.push_back(temp);
//     temp = new EndStrokeCommand(i_editor, 200, 200);
//     cmds.push_back(temp);
//     i++;
// }
// else if (!input[i].compare("-tool-chalk")) {
//     std::cout << "made tool blur stroke from (x, y) == (100, 100) " <<
//               "to (x, y) == (200, 200)" << std::endl;
//     temp = new StartStrokeCommand(i_editor, "Chalk",
//                                   ColorData(0, 0, 0), 10.0, 100, 100);
//     cmds.push_back(temp);
//     temp = new AddToStrokeCommand(i_editor, 200, 200);
//     cmds.push_back(temp);
//     temp = new EndStrokeCommand(i_editor, 200, 200);
//     cmds.push_back(temp);
//     i++;
// }
// else if (!input[i].compare("-tool-eraser")) {
//     std::cout << "made tool blur stroke from (x, y) == (100, 100) " <<
//               "to (x, y) == (200, 200)" << std::endl;
//     temp = new StartStrokeCommand(i_editor, "Eraser",
//                                   ColorData(0, 0, 0), 10.0, 100, 100);
//     cmds.push_back(temp);
//     temp = new AddToStrokeCommand(i_editor, 200, 200);
//     cmds.push_back(temp);
//     temp = new EndStrokeCommand(i_editor, 200, 200);
//     cmds.push_back(temp);
//     i++;
// }
// else if (!input[i].compare("-tool-highlighter")) {
//     std::cout << "made tool blur stroke from (x, y) == (100, 100) " <<
//               "to (x, y) == (200, 200)" << std::endl;
//     temp = new StartStrokeCommand(i_editor, "Highlighter",
//                                   ColorData(0, 0, 0), 10.0, 100, 100);
//     cmds.push_back(temp);
//     temp = new AddToStrokeCommand(i_editor, 200, 200);
//     cmds.push_back(temp);
//     temp = new EndStrokeCommand(i_editor, 200, 200);
//     cmds.push_back(temp);
//     i++;
// }
// else if (!input[i].compare("-tool-pen")) {
//     std::cout << "made tool blur stroke from (x, y) == (100, 100) " <<
//               "to (x, y) == (200, 200)" << std::endl;
//     temp = new StartStrokeCommand(i_editor, "Pen",
//                                   ColorData(0, 0, 0), 10.0, 100, 100);
//     cmds.push_back(temp);
//     temp = new AddToStrokeCommand(i_editor, 200, 200);
//     cmds.push_back(temp);
//     temp = new EndStrokeCommand(i_editor, 200, 200);
//     cmds.push_back(temp);
//     i++;
// }
// else if (!input[i].compare("-tool-spray-can")) {
//     std::cout << "made tool blur stroke from (x, y) == (100, 100) " <<
//               "to (x, y) == (200, 200)" << std::endl;
//     temp = new StartStrokeCommand(i_editor, "Spray Can",
//                                   ColorData(0, 0, 0), 10.0, 100, 100);
//     cmds.push_back(temp);
//     temp = new AddToStrokeCommand(i_editor, 200, 200);
//     cmds.push_back(temp);
//     temp = new EndStrokeCommand(i_editor, 200, 200);
//     cmds.push_back(temp);
//     i++;
// }
// else if (!input[i].compare("-tool-stamp")) {
//     std::cout << "made tool blur stroke from (x, y) == (100, 100) " <<
//               "to (x, y) == (200, 200)" << std::endl;
//     temp = new StartStrokeCommand(i_editor, "Stamp",
//                                   ColorData(0, 0, 0), 10.0, 100, 100);
//     cmds.push_back(temp);
//     temp = new AddToStrokeCommand(i_editor, 200, 200);
//     cmds.push_back(temp);
//     temp = new EndStrokeCommand(i_editor, 200, 200);
//     cmds.push_back(temp);
//     i++;
// }
    }
    return cmds;
}

bool CommandLineProcessor::validCommands(std::vector<std::string> input,
                                         int argc) {
    float result = -1;
    bool bad_input = false;
    // go thru and check to make sure that commands are valid
    int i = 0;
    while (i < argc-2) {
        // std::cout << input[i] << i << std::endl;
        if (!input[i].compare("-blur") &&
            String2Float(input[i+1], result)) { i++;
        } else if (!input[i].compare("-sharpen") &&
                 String2Float(input[i+1], result)) { i++;
        } else if (!input[i].compare("-edgedetect")) {
            // do nothing
        } else if (!input[i].compare("-red") &&
                 String2Float(input[i+1], result)) { i++;
        } else if (!input[i].compare("-green") &&
                 String2Float(input[i+1], result)) { i++;
        } else if (!input[i].compare("-blue") &&
                 String2Float(input[i+1], result)) { i++;
        } else if (!input[i].compare("-quantize") &&
                 String2Float(input[i+1], result)) { i++;
        } else if (!input[i].compare("-saturate") &&
                 String2Float(input[i+1], result)) { i++;
        } else if (!input[i].compare("-threshold") &&
                 String2Float(input[i+1], result)) { i++;
        } else if (!input[i].compare("-motionblur-n-s") &&
                 String2Float(input[i+1], result)) { i++;
        } else if (!input[i].compare("-motionblur-e-w") &&
                 String2Float(input[i+1], result)) { i++;
        } else if (!input[i].compare("-motionblur-ne-sw") &&
                 String2Float(input[i+1], result)) { i++;
        } else if (!input[i].compare("-motionblur-nw-se") &&
                 String2Float(input[i+1], result)) { i++;
        } else if (input[i] == "-h") { bad_input = true;
              // std::cout << "-h command: " << input[i] << std::endl;
        } else {
            // std::cout << "invlaid command: " << input[i] << std::endl;
            bad_input = true;
        }
        i++;
    }
    return bad_input;
    // else if (!input[i].compare("-tool-blur")) { i++; }
    // else if (!input[i].compare("-tool-calligraphy-pen")) { i++; }
    // else if (!input[i].compare("-tool-chalk")) { i++; }
    // else if (!input[i].compare("-tool-eraser")) { i++; }
    // else if (!input[i].compare("-tool-highlighter")) { i++; }
    // else if (!input[i].compare("-tool-pen")) { i++; }
    // else if (!input[i].compare("-tool-spray-can")) { i++; }
    // else if (!input[i].compare("-tool-stamp")) { i++; }
}

void CommandLineProcessor::printHelp() {
    std::cout <<
    "usage: mia infile.png [image processing commands ...] outfile.png\n" <<
    "infile.png: input image file in png format\n" <<
    "outfile.png: filename to use for saving the result\n" <<
    "image processing commands:\n" <<
    "-blur r: apply a gaussian blur of radius r\n" <<
    "-edgedetect: apply an edge detection filter\n" <<
    "-sharpen r: apply a sharpening filter of radius r\n" <<
    "-red s: scale the red channel by factor s\n" <<
    "-green s: scale the green channel by factor s\n" <<
    "-blue s: scale the blue channel by factor s\n" <<
    "-quantize n: quantize each color channel into n bins\n" <<
    "-saturate s: saturate colors by scale factor s\n" <<
    "-threshold c: threshold using cutoff value c\n" <<
    "-motionblur-n-s r: north-south motion blur with kernel radius r\n" <<
    "-motionblur-e-w r: east-west motion blur with kernel radius r\n" <<
    "-motionblur-ne-sw r: ne-sw motion blur with kernel radius r\n" <<
    "-motionblur-nw-se r: nw-se motion blur with kernel radius r\n";
}

bool CommandLineProcessor::String2Float(const std::string& str, float result) {
    try {
        result = std::stof(str);
        result = result;
        return true;
    }
    catch (std::invalid_argument&) {
        return false;
    }
    catch (std::out_of_range&) {
        return false;
    }
}
}  // namespace image_tools
