#include "PGMimageProcessor.h"
#include "PPMimageProcessor.h"
#include <iostream>
#include <string>
#include <memory>


enum ImageType {
    PGM,
    PPM,
    UNKNOWN
};
//function to determine the image type based on the file extension
ImageType getImageType(const std::string& filename) {
    size_t dotPos = filename.find_last_of('.');
    if (dotPos != std::string::npos) {
        std::string ext = filename.substr(dotPos + 1);
        if (ext == "pgm") {
            return PGM;
        } else if (ext == "ppm") {
            return PPM;
        }
    }
    return UNKNOWN;
}

int main(int argc, char* argv[]){
    if (argc < 2){
        std::cerr << "Usage: " << argv[0] << " [options] <inputPGMfile>\n";
        return 1;
    }

    std::string inputFile;
    std::string outputFile;
    std::string boxedOutputFile;
    int threshold = 128;
    int minValidSize = 1;
    int filterMin = -1;
    int filterMax = -1;
    bool printData = false;
    bool writeOutput = false;
    bool writeBoxed = false;
    unsigned char boxColor[3] = {255, 0, 0}; //red color for the box

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
        else if (arg == "-w" && i + 1 < argc){
            writeOutput = true;
            outputFile = argv[++i];
        }
        else if (arg == "-b" && i+1 <argc){
            writeBoxed = true;
            boxedOutputFile = argv[++i];
        }
        else{
            inputFile = arg;
        }
    }
    try{
        ImageType type = getImageType(inputFile);

        if (type == PPM){
            PPMimageProcessor processor(inputFile);
            int initialCount = processor.extractComponents(threshold, minValidSize);
            std::cout << "Initial components found: " << initialCount << std::endl;
            //std::cout << "Total components: " << processor.getComponentCount() << std::endl;
            //std::cout << "Smallest component: " << processor.getSmallestSize() << std::endl;
            //std::cout << "Largest component: " << processor.getLargestSize() << std::endl;

            if (filterMin != -1 && filterMax != -1){
                processor.filterComponentsBySize(filterMin, filterMax);
            }

            if (printData){
                std::cout << "Total components: " << processor.getComponentCount() << std::endl;
                std::cout << "Smallest component: " << processor.getSmallestSize() << std::endl;
                std::cout << "Largest component: " << processor.getLargestSize() << std::endl;
            }

            if (writeOutput){
                if (outputFile.empty()){
                    std::cerr << "Error: No output filename specified with -w" << std::endl;
                    return 1;
                }
                if (!processor.writeComponents(outputFile)){
                    std::cerr << "Error: Failed to write output file" << std::endl;
                    return 1;
                }
                std::cout << "Successfully wrote: " << outputFile << std::endl;
            }

            if (writeBoxed){
                if (!processor.writeBoxedComponents(boxedOutputFile, boxColor)){
                    std::cerr << "Error: Failed to write boxed output file" << std::endl;
                    return 1;
                }
            }
            else if (type == PGM){
                PGMimageProcessor processor(inputFile);
                int initialCount = processor.extractComponents(threshold, minValidSize);
                std::cout << "Initial components found: " << initialCount << std::endl;

                //std::cout << "Total components: " << processor.getComponentCount() << std::endl;
                //std::cout << "Smallest component: " << processor.getSmallestSize() << std::endl;
                //std::cout << "Largest component: " << processor.getLargestSize() << std::endl;

                if (filterMin != -1 && filterMax != -1){
                    processor.filterComponentsBySize(filterMin, filterMax);
                }
                if (printData){
                    std::cout << "Total components: " << processor.getComponentCount() << std::endl;
                    std::cout << "Smallest component: " << processor.getSmallestSize() << std::endl;
                    std::cout << "Largest component: " << processor.getLargestSize() << std::endl;
                }

                if (writeOutput){
                    if (outputFile.empty()){
                        std::cerr << "Error: No output filename specified with -w" << std::endl;
                        return 1;
                    }
                    if (!processor.writeComponents(outputFile)){
                        std::cerr << "Error: Failed to write output file" << std::endl;
                        return 1;
                    }
                    std::cout << "Successfully wrote: " << outputFile << std::endl;
                }
                if (writeBoxed){
                    std::cerr << "Warning: Bounding boxes are not supported for PGM images." << std::endl;
                }
            }
            else{
                std::cerr << "Error: Unsupported file type. Use .pgm or .ppm files" << std::endl;
                return 1;
            }
        }
    }
    catch(const std::exception& e){
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}