//
// Copyright by A-dizzle on 11/21/2018.
//

#ifndef MIA_COMMAND_LINE_PROCESSOR_H_
#define MIA_COMMAND_LINE_PROCESSOR_H_

#include <imagetools/pixel_buffer.h>
#include <string>
#include <vector>

#include "imagetools/color_data.h"
#include "imagetools/image_editor.h"
#include "mia/image_editor_commands.h"

namespace image_tools {

class CommandLineProcessor {
 public:
    CommandLineProcessor();
    ~CommandLineProcessor();

    /** this function is intended to process the command line arguments */
    void ProcessCommandLine(int argc, char *argv[]);

    /** intended to print out a help message if bad input is recieved */
    void printHelp();

    /** used for converting strings to integers when parsing the
     * input recieved from the command line*/
    bool String2Float(const std::string &str, float result);

    /** used to make sure that input args have the correct parameters by
     * iterating through each passed in argument and checking if it is
     * has a valid argument that was entered after it */
    bool validCommands(std::vector<std::string> input, int argc);

    /** used to put all commands into a container once they have been checked
     * for the correct parameters so that the ProcessCommandLine function can
     * execute every valid thing that the user input */
    std::vector<ImageEditorCommand*> getCommands(ImageEditor* i_editor,
            std::vector<ImageEditorCommand*> cmds,
            std::vector<std::string> input, int argc);
};
}  // namespace image_tools

#endif  // MIA_COMMAND_LINE_PROCESSOR_H_
