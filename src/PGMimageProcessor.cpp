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

bool PGMimageProcessor::readPGMFile(const std::string& filename){
    std::ifstream file(filename, std::ios::binary);
    if (!file)
        return false;
    
    std::string magicNum;
    file >> magicNum;
    if (magicNum != "P5")
        return false;

    file >> width >> height;
    int maxVal;
    file >> maxVal;
    file.ignore(1); //skip single whitespace

    delete[] imageData;
    imageData = new unsigned char[width*height];
    file.read(reinterpret_cast<char*>(imageData), width*height);

    return file.good();
}

int PGMimageProcessor::extractComponents(unsigned char threshold, int minValidSize){
    if (!imageData)
        return false;
    
    components.clear();
    unsigned char* tempImage = new unsigned char[width*height];
    std::copy(imageData, imageData + width * height, tempImage);

    int componentId = 0;

    for (int y = 0; y < height; ++y){
        for (int x = 0; x < width; ++x){
            if (tempImage[y * width+x] >= threshold){
                auto component = std::make_unique<ConnectedComponent>(componentId++);
                BFS(x, y, threshold, tempImage, component);

                if (component->getPixelCount() >= minValidSize){
                    components.push_back(std::move(component));
                }
            }
        }
    }
    delete[] tempImage;
    return components.size();
}