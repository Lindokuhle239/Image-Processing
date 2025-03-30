#include "PGMimageProcessor.h"
#include <fstream>

PGMimageProcessor::PGMimageProcessor(const std::string& filename) : imageData(nullptr), wd(0), ht(0){
    if (!readPGMFile(filename)){
        throw std::runtime_error("Failed to read PGM file.");
    }
}

PGMimageProcessor::~PGMimageProcessor(){
    delete[] imageData;
}