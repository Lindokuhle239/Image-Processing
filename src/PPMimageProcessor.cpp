#include "PPMimageProcessor.h"
#include <fstream>
#include <queue>
#include <algorithm>
#include <iostream>
#include <cstring>

PPMimageProcessor::PPMimageProcessor(const std::string& filename) : imageData(nullptr), width(0), height(0) {
    if (!readPPMFile(filename)) {
        throw std::runtime_error("Failed to read PPM file");
    }
}

PPMimageProcessor::~PPMimageProcessor() {
    delete[] imageData;
}

PPMimageProcessor::PPMimageProcessor(PPMimageProcessor&& other) noexcept
: components(std::move(other.components)), imageData(other.imageData), width(other.width), height(other.height) {
    other.imageData = nullptr;
    other.width = 0;
    other.height = 0;
}

PPMimageProcessor& PPMimageProcessor::operator=(PPMimageProcessor&& other) noexcept {
    if (this != &other) {
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