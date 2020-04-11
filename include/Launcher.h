//
// Created by robert on 04.04.2020.
//

#ifndef TKOM_LAUNCHER_H
#define TKOM_LAUNCHER_H

#include <memory>
#include <vector>
#include <algorithm>
#include <iostream>
#include "Scanner.h"

struct Configuration {

    std::string configPath {""};
    std::string outputPath {""};
    bool isVerbose {false};

};

class Launcher {

private:
    unsigned int minArgc {1};
    Configuration configuration;
    std::vector<std::string> filePathFlags = {"-f", "-o"};
    std::vector<std::string> nonFilePathFlags = {"-v"};

    Scanner scanner;

    bool isPathFlag(std::string potentialFlag) {
        return std::find(filePathFlags.begin(), filePathFlags.end(), potentialFlag.c_str()) != filePathFlags.end();
    }

    bool isNonPathFile(std::string potentialFlag) {
        return std::find(nonFilePathFlags.begin(), nonFilePathFlags.end(), potentialFlag.c_str()) != nonFilePathFlags.end();
    }

    bool isFilePathProper(std::string filepath){
        return true;
    }

    bool isPathToUpperDirProper(std::string filepath){
        return true;
    }

public:

    Launcher() = default;

    void readFlags(int argc, char* argv[]) {

        if((unsigned int)argc < minArgc) {
            throw std::runtime_error("Not enough arguments provided");
        }

        for(int i = 1; i < argc; i++) {

            std::string potentialFlag(argv[i]);
            if(isPathFlag(potentialFlag)) {
                //if next argument is provided
                if(i + 1 >= argc) {
                    throw std::runtime_error("File path not provided");
                }

                std::string filePath(argv[i+1]);

                if(potentialFlag == "-f") {
                    if(!isFilePathProper(filePath)) {
                        throw std::runtime_error("Wrong path to config file");
                    }
                    configuration.configPath = filePath;

                } else if(potentialFlag == "-o") {
                    if(!isPathToUpperDirProper(filePath)) {
                        throw std::runtime_error("Wrong path to output file");
                    }
                    configuration.outputPath = filePath;
                }

                //next argument can be ommited, because it is a proper filepath
                i++;

            } else if(isNonPathFile(potentialFlag)) {

                if(potentialFlag == "-v") {
                    configuration.isVerbose = true;
                }
            }
        }
    }

    void run() {
//        Interprenter(configuration);
//         std::cout << "running with"
//         << " config: " << configuration.configPath
//         << " output: " << configuration.outputPath
//         << " verbose: " << configuration.isVerbose << std::endl;

         scanner.scan();
    }
};


#endif //TKOM_LAUNCHER_H