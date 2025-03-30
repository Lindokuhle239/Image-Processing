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

void PGMimageProcessor::BFS(int startX, int startY, unsigned char threshold, unsigned char* image, std::unique_ptr<ConnectedComponent>& component){
    std::queue<std::pair<int, int>> queue;
    queue.emplace(startX, startY);
    image[startY*width + startX] = 0; //mark as visited

    const int dx[] = {-1, 1, 0, 0};
    const int dy[] = {0, 0, -1, 1};

    while (!queue.empty()){
        auto [x, y] = queue.front();
        queue.pop();
        component->addPixel(x, y);

        for (int i = 0; i < 4; ++i){
            int nx = x + dx[i];
            int ny = y + dy[i];

            if ((nx >= 0) && (nx < width) && (ny >= 0) && (ny < height) && (image[ny * width +nx] >= threshold){
                image[ny * width + nx] = 0;
                queue.emplace(nx, ny);
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
    if (!file)
        return false;
    
    file << "P5\n" << width << " " << height << "\n255\n";
    unsigned char* outputImage = new unsigned char[width*height]();

    for (const auto& comp: components){
        for (const auto& [x, y] : comp->getPixels()){
            outputImage[y*width + x] = 255;
        }
    }
    file.write(reinterpret_cast<char*>(outputImage), width, height);
    delete[] outputImage;
    return file.good();
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