// Copyright A-dizzle 11/28/2018


#include <iostream>
#include <sstream>
#include <string>
#include "gtest/gtest.h"
#include "mia/mia_app.h"
#include "imagetools/pixel_buffer.h"
#include "mia/command_line_processor.h"

using image_tools::MiaApp;

class MiaAppTest : public ::testing::Test {
    void SetUp() override {
        app_ = new MiaApp(1024, 780, image_tools::ColorData(1, 1, 1));
    }

    void TearDown() override {
        delete app_;
    }

 protected:
        MiaApp *app_;
};

TEST_F(MiaAppTest, ValidCommands) {
// fails because the strings get newline chars appended in random places
    std::ostringstream oss1;
    std::ostringstream oss2;
    std::streambuf *csb1 = std::cout.rdbuf();
    std::streambuf *csb2 = std::cout.rdbuf();
    bool correct = true;
    int argc = 4;
    char infile[] = {'/','h','o','m','e','/','s','m','i','t','8','7','9','6',
                     '/','c','s','c','i','3','0','8','1','/','r','e','p','o',
                     '-','s','m','i','t','8','7','9','6','/','P','R','O','J',
                     '/','r','e','s','o','u','r','c','e','s','/','f','r','u',
                     'i','t','2','.','p','n','g','\0'};
    char outfile[] = {'o','.','p','n','g','\0'};
    char input1[] = {'2','\0'};
    char dot[] = {'h', 'i','\0'};
    char *argv[] = {dot, infile, input1, outfile};
    image_tools::CommandLineProcessor cmd_processor;

    std::cout.rdbuf(oss1.rdbuf());
    cmd_processor.ProcessCommandLine(argc, argv);
    std::cout.rdbuf(csb1);

    std::cout.rdbuf(oss2.rdbuf());
    cmd_processor.printHelp();
    std::cout.rdbuf(csb2);

    std::string out1 = oss1.str();
    std::string out2 = oss2.str();
//    std::cout << out1 << out2;
    if (out1 != out2) {
    //            std::cout << "i have a dream\n";
        correct = false;
    }
    EXPECT_TRUE(correct);
}

//TEST_F(MiaAppTest, ValidCommands2) {
//// fails because the strings get newline chars appended in random places
//    std::ostringstream oss1;
//    std::streambuf *csb1 = std::cout.rdbuf();
//    bool correct = true;
//    int argc = 4;
//    image_tools::CommandLineProcessor cmd_processor;
//
//    char dot[] = {'h', 'i'};
//    char input1[] = {'2'};
//    char in[] = {'a','r','g','a','b','l','a','r','g','b','l','a','r','g'};
//    char *hello[] = {dot, in, input1};
//
//            std::cout << "a\n";
//    std::cout.rdbuf(oss1.rdbuf());
//            std::cout << "b\n";
//    cmd_processor.ProcessCommandLine(argc, hello);
//            std::cout << "c\n";
//    std::cout.rdbuf(csb1);
//            std::cout << "d\n";
//
//    std::string out1 = oss1.str();
//
//    if ("invalid input file\n\n" != out1) {
//    //            std::cout << out1 << std::endl;
//    //            std::cout << "a dreamy dream\n";
//    correct = false;
//    }
//    EXPECT_TRUE(correct);
//}

TEST_F(MiaAppTest, ValidCommands3) {
    std::ostringstream oss1;
    std::ostringstream oss2;
    std::streambuf *csb1 = std::cout.rdbuf();
    std::streambuf *csb2 = std::cout.rdbuf();
    bool correct = true;
    int argc = 4;
    char dot[] = {'h', 'i','\0'};
    char input1[] = {'2','\0'};
    char infile[] = {'/','h','o','m','e','/','s','m','i','t','8','7','9','6','/',
                     'c','s','c','i','3','0','8','1','/','r','e','p','o','-','s',
                     'm','i','t','8','7','9','6','/','P','R','O','J','/','r','e',
                     's','o','u','r','c','e','s','/','f','r','u','i','t','2','.',
                     'p','n','g','\0'};

    image_tools::CommandLineProcessor cmd_processor;

    argc = 4;
    char input[] = {'-','b','l','u','r','\0'};
    char *argd[] = {dot, infile, input, input1};

    std::cout.rdbuf(oss1.rdbuf());
    cmd_processor.ProcessCommandLine(argc, argd);
    std::cout.rdbuf(csb1);


    std::cout.rdbuf(oss2.rdbuf());
    cmd_processor.printHelp();
    std::cout.rdbuf(csb2);

    std::string out1 = oss1.str();
    std::string out2 = oss2.str();

    if (out1 != out2) {
    //            std::cout << out1 << out2 << std::endl;
    //            std::cout << "that one day, i\n";
        correct = false;
    }
    EXPECT_TRUE(correct);
}

TEST_F(MiaAppTest, ValidCommands4) {
    std::ostringstream oss1;
    std::ostringstream oss2;
    std::streambuf *csb1 = std::cout.rdbuf();
    std::streambuf *csb2 = std::cout.rdbuf();
    bool correct = true;
    int argc = 4;
    char infile[] = {'/','h','o','m','e','/','s','m','i','t','8','7','9','6','/',
                     'c','s','c','i','3','0','8','1','/','r','e','p','o','-','s',
                     'm','i','t','8','7','9','6','/','P','R','O','J','/','r','e',
                     's','o','u','r','c','e','s','/','f','r','u','i','t','2','.',
                     'p','n','g','\0'};
    char input[] = {'-','b','l','u','r','\0'};
    char dot[] = {'h', 'i','\0'};
    char input1[] = {'2','\0'};
    image_tools::CommandLineProcessor cmd_processor;

    argc = 4;
    char *argq[] = {dot, infile, input, input1};

    std::cout.rdbuf(oss1.rdbuf());
    cmd_processor.ProcessCommandLine(argc, argq);
    std::cout.rdbuf(csb1);


    std::cout.rdbuf(oss2.rdbuf());
    cmd_processor.printHelp();
    std::cout.rdbuf(csb2);


    std::string out1 = oss1.str();
    std::string out2 = oss2.str();

    if (out1 != out2) {
    //            std::cout << out1 << out2 << std::endl;
    //            std::cout << "can eat a ham\n";
        correct = false;
    }
    EXPECT_TRUE(correct);
}

TEST_F(MiaAppTest, ValidCommands5) {
std::ostringstream oss1;
std::ostringstream oss2;
std::streambuf *csb1 = std::cout.rdbuf();
std::streambuf *csb2 = std::cout.rdbuf();
bool correct = true;
int argc = 4;
char infile[] = {'/','h','o','m','e','/','s','m','i','t','8','7','9','6','/',
                 'c','s','c','i','3','0','8','1','/','r','e','p','o','-','s',
                 'm','i','t','8','7','9','6','/','P','R','O','J','/','r','e',
                 's','o','u','r','c','e','s','/','f','r','u','i','t','2','.',
                 'p','n','g','\0'};
char input[] = {'-','b','l','u','r','\0'};
char dot[] = {'h', 'i','\0'};
char input1[] = {'2','\0'};
char outfile[] = {'o','.','p','n','g','\0'};
image_tools::CommandLineProcessor cmd_processor;

        argc = 6;
        char *argw[] = {dot, infile, input, input, input1, outfile};

        std::cout.rdbuf(oss1.rdbuf());
        cmd_processor.ProcessCommandLine(argc, argw);
        std::cout.rdbuf(csb1);
//        std::cout << "and can go home\n";

        std::cout.rdbuf(oss2.rdbuf());
        cmd_processor.printHelp();
        std::cout.rdbuf(csb2);


std::string out1 = oss1.str();
std::string out2 = oss2.str();

        if (out1 != out2) {
//            std::cout << out1 << out2 << std::endl;
//            std::cout << "and can go home\n";
            correct = false;
        }

        EXPECT_TRUE(correct);
}

TEST_F(MiaAppTest, CorrectCommandSet) {
        std::ostringstream oss1;
        std::streambuf* csb1 = std::cout.rdbuf();
//        oss1.str("");
//        std::str(std::cout);
        bool correct = true;
        int argc = 5;
char infile[] = {'/','h','o','m','e','/','s','m','i','t','8','7','9','6','/',
                 'c','s','c','i','3','0','8','1','/','r','e','p','o','-','s',
                 'm','i','t','8','7','9','6','/','P','R','O','J','/','r','e',
                 's','o','u','r','c','e','s','/','f','r','u','i','t','2','.',
                 'p','n','g','\0'};
char input[] = {'-','b','l','u','r','\0'};
char dot[] = {'h', 'i','\0'};
char input1[] = {'2','\0'};
char outfile[] = {'o','.','p','n','g','\0'};
        char* argv[] = {dot, infile, input, input1, outfile};
        image_tools::CommandLineProcessor cmd_processor;
        // std::str(std::cout);

        std::cout.rdbuf(oss1.rdbuf());
        cmd_processor.ProcessCommandLine(argc, argv);
        std::cout.rdbuf(csb1);

        // std::str(std::cout);
        // std::cout << oss1.str() << std::endl;

        std::string out = oss1.str();
        int idx = static_cast<int>(out.find("added blur"));
         std::cout << out << std::endl;
        // std::cout << "blur is at index "<< idx << std::endl;
        // std::cout << "truth value: " << (idx == -1) << std::endl;
        if (idx == -1) {
             std::cout << "holy penstacciano!! ayyy" << std::endl;
            correct = false;
        }

//        oss1.str();
        // std::cout << "twas stred" << std::endl;
        // std::str(std::cout);

        char input2[] = {'-','b','l','u','e','\0'};
        char* argb[] = {dot, infile, input2, input1, outfile};
        // std::str(std::cout);

        std::cout.rdbuf(oss1.rdbuf());
        cmd_processor.ProcessCommandLine(argc, argb);
        std::cout.rdbuf(csb1);
        // std::str(std::cout);
        // std::cout << oss1.str() << std::endl;

        std::string out1 = oss1.str();
         std::cout << out1 << std::endl;
        idx = static_cast<int>(out1.find("added blue"));
        // std::cout << "blue is at index "<< idx << std::endl;
        if (idx == -1) {
             std::cout << "holy guac" << std::endl;
            correct = false;
        }

        oss1.str();
        // std::str(std::cout);

        char input6[] = {'-','e','d','g','e','d','e','t','e','c','t','\0'};
        char* argz[] = {dot, infile, input2, input1, input6, outfile};
//        std::str(std::cout);

        std::cout.rdbuf(oss1.rdbuf());
        cmd_processor.ProcessCommandLine(6, argz);
        std::cout.rdbuf(csb1);
        // std::str(std::cout);
        // std::cout << oss1.str() << std::endl;

        std::string out2 = oss1.str();
         std::cout << out2 << std::endl;
        idx = static_cast<int>(out2.find("added blue"));
        int idx1 = static_cast<int>(out2.find("added edgedetect"));
        // std::cout << "blue is at index " << idx << std::endl;
        // std::cout << "edgedetect is at index " << idx1 << std::endl;
        if (idx == -1 && idx1 == -1) {
                     std::cout << "holy friggimolie" << std::endl;
            correct = false;
        }

        EXPECT_TRUE(correct);
}
