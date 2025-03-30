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

PGMimageProcessor::PGMimageProcessor(PGMimageProcessor&& other) noexcept : components(std::move(other.components)),
    imageData(other.imageData),
    width(other.width),
    height(other.height){
        other.imageData = nullptr;
        other.width = 0;
        other.height = 0;
    }

PGMimageProcessor& PGMimageProcessor::operator=(PGMimageProcessor&& other) noexcept{
    if (this != &other){
        delete[] imageData;
        components = std::move(other.components);
        imageData = other.imageData;
        width = other.width;
        height = other.height;
        other.imageData = nullptr;
        other.width = 0;
        other.height = 0;
    }
    return *this;
}