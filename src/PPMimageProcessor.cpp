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

unsigned char PPMimageProcessor::rgbToGrayScale(unsigned char r, unsigned char g, unsigned char b) {
    return static_cast<unsigned char>(0.299 * r + 0.587 * g + 0.114 * b);
}

bool PPMimageProcessor::readPPMFile(const std::string& filename){
    std::ifstream file(filename, std::ios::binary);
    if (!file){
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return false;
    }
    
    std::string magicNum;
    file >> magicNum;
    if (magicNum != "P6"){
        std::cerr << "Error: Not a binary PPM file (P6)" << std::endl;
        return false;
    }

    file >> width >> height;
    int maxVal;
    file >> maxVal;
    file.get();

    if (maxVal != 255){
        std::cerr << "Warning: Unsupported max pixel value. Expected 255." << std::endl;
    }

    //delete[] imageData;
    imageData = new unsigned char[width*height*3];
    file.read(reinterpret_cast<char*>(imageData), width*height*3);

    return file.good();
}

int PPMimageProcessor::extractComponents(unsigned char threshold, int minValidSize){
    bool* visited = new bool[width*height];
    memset(visited, false, width*height);

    int componentID = 0;
    for (int y = 0; y < height; ++y){
        for (int x = 0; x < width; ++x){
            int index = (y * width + x) * 3; // RGB image, so 3 bytes per pixel
            unsigned char gray = rgbToGrayScale(imageData[index], imageData[index + 1], imageData[index + 2]); 

            if (gray >= threshold && !visited[y * width + x]) {
                auto component = std::make_unique<ConnectedComponent>(componentID++);
                BFS(x, y, threshold, visited, component);
                if (component->getPixelCount() >= minValidSize) {
                    components.push_back(std::move(component));
                }
            }
        }
    }
    delete[] visited;
    return components.size();
}

void PPMimageProcessor::BFS(int startX, int startY, unsigned char threshold, bool* visited, std::unique_ptr<ConnectedComponent>& component){
    std::queue<std::pair<int, int>> queue;
    queue.emplace(startX, startY);
    visited[startY * width + startX] =true;

    while (!queue.empty()){
        auto [x, y] = queue.front();
        queue.pop();
        component->addPixel(x, y);

        static const int dx[] = {-1, 1, 0, 0};
        static const int dy[] = {0, 0, -1, 1}; 

        for (int i = 0; i < 4; ++i){
            int nx = x + dx[i];
            int ny = y + dy[i];
            if (nx >= 0 && nx < width && ny >= 0 && ny < height && imageData[nIndex] >= threshold && !visited[nIndex]){
                int nIndex = (ny * width + nx) *3; // RGB image, so 3 bytes per pixel
                unsigned char gray = rgbToGrayScale(imageData[nIndex], imageData[nIndex + 1], imageData[nIndex + 2]);

                if (gray >= threshold){
                    queue.emplace(nx, ny);
                    visited[ny * width + nx] = true;
                }
            }
        }
    }
}

int PPMimageProcessor::filterComponentsBySize(int minSize, int maxSize){
    auto it = std::remove_if(components.begin(), components.end(), [minSize, maxSize](const std::unique_ptr<ConnectedComponent>& comp){
    //auto it = std::remove_if(components.begin(), components.end(), [&](const std::unique_ptr<ConnectedComponent>& comp)){
        int size = comp->getPixelCount();
        return size < minSize || size > maxSize;
    });
    components.erase(it, components.end());
    return components.size();
}

bool PPMimageProcessor::writeComponents(const std::string& outFilename){
    std::ofstream file(outFilename, std::ios::binary);
    if (!file) {
        std::cerr << "Error: Cannot open output file " << std::endl;
        return false;
    }
    
    file << "P6\n" << width << " " << height << "\n255\n";
    if (!file) {
        std::cerr << "Error: Failed writing header" << std::endl;
        return false;
    }
    unsigned char* outputImage = new unsigned char[width*height]();

    for (const auto& comp: components){
        for(const auto& pixel: comp->getPixels()){
            int index = (pixel.second * width + pixel.first) * 3;
            outputImage[index] = 255; // R
            outputImage[index + 1] = 255; // G
            outputImage[index + 2] = 255; // B
        }
    }
    file.write(reinterpret_cast<const char*>(outputImage), width * height*3);
    delete[] outputImage;
    file.close();
}

bool PPMimageProcessor::writeBoxedComponents(const std::string& outFileName, unsigned char boxColor[3]){
    if (!boxColor){
        unsigned char red[3] = {255, 0, 0}; // Default to red
        return writeBoxedComponents(outFileName, red);
    }

    std::ofstream filenout(outFileName, std::ios::binary);
    if (!file){
        std::cerr << "Error: Cannot open output file " << outFileName << std::endl;
        return false;
    }

    file << "P6\n" << width << " " << height << "\n255\n";
    if (!file) {
        std::cerr << "Error: Failed writing header" << std::endl;
        return false;
    }

    //create output image (copy original)
    unsigned char* outputImage = new unsigned char[width*height*3];
    std::copy(imageData, imageData + width*height*3, outputImage);

    //draw bounding boxes
    for (const auto&comp : components){
        if (comp->getPixelCount() == 0){
            continue;
        }

        //finding bounding box coordinates
        int minX = width;
        int minY = height;
        int maxX = 0;
        int maxY = 0;

        for (const auto& pixel : comp->getPixels()){
            minX = std::min(minX, pixel.first);
            minY = std::min(minY, pixel.second);
            maxX = std::max(maxX, pixel.first);
            maxY = std::max(maxY, pixel.second);
        }

        //draw horizontal lines
        for (int x = minX; x <= maxX; ++x){
            //top line
            outputImage[(minY * width + x) * 3] = boxColor[0];
            outputImage[(minY * width + x) * 3 + 1] = boxColor[1];
            outputImage[(minY * width + x) * 3 + 2] = boxColor[2];
            //bottom line
            outputImage[(maxY * width + x) * 3] = boxColor[0];
            outputImage[(maxY * width + x) * 3 + 1] = boxColor[1];
            outputImage[(maxY * width + x) * 3 + 2] = boxColor[2];
        }

        //draw vertical lines
        for (int y = minY; y <= maxY; ++y){
            //left line
            outputImage[(y * width + minX) * 3] = boxColor[0];
            outputImage[(y * width + minX) * 3 + 1] = boxColor[1];
            outputImage[(y * width + minX) * 3 + 2] = boxColor[2];
            //right line
            outputImage[(y * width + maxX) * 3] = boxColor[0];
            outputImage[(y * width + maxX) * 3 + 1] = boxColor[1];
            outputImage[(y * width + maxX) * 3 + 2] = boxColor[2];
        }
    }
    file.write(reinterpret_cast<const char*>(outputImage), width * height * 3);
    delete[] outputImage;
    return file.good();
}

int PPMimageProcessor::getComponentCount() const{
    return components.size();
}

int PPMimageProcessor::getLargestSize() const{
    if (components.empty())
        return 0;
    return std::max_element(components.begin(), components.end(), [](const auto& a, const auto& b){
        return a->getPixelCount() < b->getPixelCount();
    })->get()->getPixelCount();
}

int PPMimageProcessor::getSmallestSize() const{
    if (components.empty())
        return 0;
    return std::min_element(components.begin(), components.end(), [](const auto& a, const auto& b){
        return a->getPixelCount() < b->getPixelCount();
    })->get()->getPixelCount();
}

void PPMimageProcessor::printComponentData(const ConnectedComponent& comp) const{
    std::cout << "Component ID: " << comp.getId() << ", Number of pixels: " << comp.getPixelCount() << "\n";
}