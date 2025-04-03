#include "PGMimageProcessor.h"
#include <fstream>
#include <queue>
#include <algorithm>
#include <memory>
#include <iostream>
#include <cstring>

PGMimageProcessor::PGMimageProcessor(const std::string& filename) : imageData(nullptr), width(0), height(0){
    if (!readPGMFile(filename)){
        throw std::runtime_error("Failed to read PGM file.");
    }
}

PGMimageProcessor::~PGMimageProcessor(){
    delete[] imageData;
    //imageData = nullptr;
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
    if (!file){
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return false;
    }
    
    std::string magicNum;
    file >> magicNum;
    if (magicNum != "P5"){
        std::cerr << "Error: Not a binary PGM file (P5)" << std::endl;
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
    imageData = new unsigned char[width*height];
    file.read(reinterpret_cast<char*>(imageData), width*height);

    std::cout << "Image dimensions: " << width << "x" << height << " (" << width*height << " total pixels)" << std::endl;

    std::cout << "Loaded image: " << width << "x" << height << std::endl;

    return file.good();
}

int PGMimageProcessor::extractComponents(unsigned char threshold, int minValidSize){
    bool* visited = new bool[width*height];
    memset(visited, false. width*height);

    int componentID = 0;
    for (int y = 0; y < height; ++y){
        for (int x = 0; x < width; ++x){
            int index = y * width + x;
            if (imageData[index] >= threshold && !visited[index]){
            //if (tempImage[y * width+x] >= threshold){
                std::cout << "Found pixel above threshold at (" << x << "," << y << ")" << std::endl;
                auto component = std::make_unique<ConnectedComponent>(componentId++);
                //BFS(x, y, threshold, tempImage, component);
                BFS(x, y, threshold, visited, component);
                if (component->getPixelCount() >= minValidSize){
                    components.push_back(std::move(component));
                }
            }
        }
    }
    delete[] visited;
    return components.size();

    /**if (!imageData){
        std::cerr << "Error: No image data loaded!" << std::endl;
        return 0;
    }
    //Debug: Print first 25 pixel values
    std::cout << "First 25 pixels: ";
    for (int i = 0; i < 25; ++i) {
        std::cout << (int)imageData[i] << " ";
    }
    std::cout << std::endl;

    std::cout << "Thresholding at: " << (int)threshold << std::endl;
    
    components.clear();
    unsigned char* tempImage = new unsigned char[width*height];
    std::copy(imageData, imageData + width * height, tempImage);

    int componentId = 0;

    std::cout << "Scanning image for components..." << std::endl;
    for (int y = 0; y < height; ++y){
        for (int x = 0; x < width; ++x){
            if (tempImage[y * width+x] >= threshold){
                std::cout << "Found pixel above threshold at (" << x << "," << y << ")" << std::endl;
                auto component = std::make_unique<ConnectedComponent>(componentId++);
                BFS(x, y, threshold, tempImage, component);

                if (component->getPixelCount() >= minValidSize){
                    components.push_back(std::move(component));
                }
            }
        }
    }
    delete[] tempImage;
    return components.size();*/
}

//void PGMimageProcessor::BFS(int startX, int startY, unsigned char threshold, unsigned char* image, std::unique_ptr<ConnectedComponent>& component){
void PGMimageProcessor::BFS(int startX, int startY, unsigned char threshold, bool* visited, std::unique_ptr<ConnectedComponent>& component){
    /*if (startX >= width || startY >= height){
        std::cerr << "Invalid BFS start position: (" << startX << "," << startY << ")" << std::endl;
        return;
    }
    std::cout << "Starting BFS at (" << startX << "," << startY << ") value: " << (int)image[startY*width + startX] << std::endl;*/
    std::queue<std::pair<int, int>> queue;
    queue.emplace(startX, startY);
    visited[startY * width + startX] =true;
    //image[startY*width + startX] = 0; //mark as visited

    //const int dx[] = {-1, 1, 0, 0};
    //const int dy[] = {0, 0, -1, 1};

    while (!queue.empty()){
        auto [x, y] = queue.front();
        queue.pop();
        component->addPixel(x, y);

        static const int dx[] = {-1, 1, 0, 0};
        static const int dy[] = {0, 0, -1, 1}; 

        for (int i = 0; i < 4; ++i){
            int nx = x + dx[i];
            int ny = y + dy[i];
            int nIndex = ny * width + nx;
            //if (nx >= 0 && nx < width && ny >= 0 && ny < height && image[ny*width + nx] >= threshold){
            if (nx >= 0 && nx < width && ny >= 0 && ny < height && imageData[nIndex] >= threshold && !visited[nIndex]){
                //image[ny*width + nx] = 0;
                queue.emplace(nx, ny);
                visited[nIndex] = true;
            }
        }
    }
}

int PGMimageProcessor::filterComponentsBySize(int minSize, int maxSize){
    auto it = std::remove_if(components.begin(), components.end(), [minSize, maxSize](const std::unique_ptr<ConnectedComponent>& comp){
        int size = comp->getPixelCount();
        return size < minSize || size > maxSize;
    });
    components.erase(it, components.end());
    return components.size();
}

bool PGMimageProcessor::writeComponents(const std::string& outFilename){
    std::ofstream file(outFilename, std::ios::binary);
    if (!file) {
        std::cerr << "Error: Could not create file " << outFilename << " (Error: " << strerror(errno) << ")" << std::endl;
        return false;
    }
    /*if (!file.is_open()){
        std::cerr << "Error: Failed to create the output file " << outFilename << " (Error: " << strerror(errno) << ")" << std::endl;
        return false;
    }*/

    //std::cout << "Writing output to " << outFilename << " with " << components.size() << " components" << std::endl;
    
    file << "P5\n" << width << " " << height << "\n255\n";
    if (!file) {
        std::cerr << "Error: Failed writing header" << std::endl;
        return false;
    }
    unsigned char* outputImage = new unsigned char[width*height]();

    for (const auto& comp: components){
        //std::cout << "Writing component " << comp->getId() << " with " << comp->getPixelCount() << " pixels\n";
        for (const auto& [x, y] : comp->getPixels()){
            if (x >= 0 && x < width && y >= 0 && y < height){
                outputImage[y*width + x] = 255;
            }
        }
    }
    file.write(reinterpret_cast<const char*>(outputImage), width * height);
    delete[] outputImage;

    /*if (file.good()){
        std::cerr << "Error: Failed during writing to " << outFilename << std::endl;
        return false;
    }*/
    if (!file) {
        std::cerr << "Error: Failed writing pixel data (bytes written: " << file.tellp() << ")" << std::endl;
        return false;
    }

    //std::cout << "Successfully wrote " << outFilename << std::endl;
    file.close();
    return true;
}

int PGMimageProcessor::getComponentCount() const{
    return components.size();
}

int PGMimageProcessor::getLargestSize() const{
    if (components.empty())
        return 0;
    return std::max_element(components.begin(), components.end(), [](const auto& a, const auto& b){
        return a->getPixelCount() < b->getPixelCount();
    })->get()->getPixelCount();
}

int PGMimageProcessor::getSmallestSize() const{
    if (components.empty())
        return 0;
    return std::min_element(components.begin(), components.end(), [](const auto& a, const auto& b){
        return a->getPixelCount() < b->getPixelCount();
    })->get()->getPixelCount();
}

void PGMimageProcessor::printComponentData(const ConnectedComponent& comp) const{
    std::cout << "Component ID: " << comp.getId() << ", Number of pixels: " << comp.getPixelCount() << "\n";
}