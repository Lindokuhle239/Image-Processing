#include "PGMimageProcessor.h"
#include <iostream>
#include <string>

int main(int argc, char* argv[]){
    if (argc < 2){
        std::cerr << "Usage: " << argv[0] << " [options] <inputPGMfile>\n";
        return 1;
    }

    std::string inputFile;
    std::string outputFile;
    int threshold = 128;
    int minValidSize = 1;
    int filterMin = -1;
    int filterMax = -1;
    bool printData = false;
    bool writeOutput = false;

    //parse command line arguments
    for (int i = 1; i < argc; ++i){
        std::string arg = argv[i];
        if (arg == "-t" && i + 1 < argc){
            threshold = std::stoi(std::string(argv[++i]));
        }
        else if (arg == "-m" && i + 1 < argc){
            minValidSize = std::stoi(std::string(argv[++i]));
        }
        else if (arg == "-f" && i + 2 < argc){
            filterMin = std::stoi(std::string(argv[++i]));
            filterMax = std::stoi(std::string(argv[++i]));
        }
        else if (arg == "-p"){
            printData = true;
        }
        else if (arg == "w" && i + 1 < argc){
            writeOutput = true;
            outputFile = argv[++i];
        }
        else{
            inputFile = arg;
        }
    }
    try{
        PGMimageProcessor processor(inputFile);
        int initialCount = processor.extractComponents(threshold, minValidSize);

        if (filterMin != -1 && filterMax != -1){
            processor.filterComponentsBySize(filterMin, filterMax);
        }
        if (printData){
            std::cout << "Total components: " << processor.getComponentCount() << std::endl;
            std::cout << "Smallest component: " << processor.getSmallestSize() << std::endl;
            std::cout << "Largest component: " << processor.getLargestSize() << std::endl;
        }

        if (writeOutput){
            processor.writeComponents(outputFile);
        }
    }
    catch(const std::exception& e){
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}