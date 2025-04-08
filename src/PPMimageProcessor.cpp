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

m
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

int PGMimageProcessor::extractComponents(unsigned char threshold, int minValidSize){
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

void PGMimageProcessor::BFS(int startX, int startY, unsigned char threshold, bool* visited, std::unique_ptr<ConnectedComponent>& component){
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