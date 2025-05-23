#include "ConnectedComponent.h"
#include <utility>

ConnectedComponent::ConnectedComponent() : id(-1), pixelCount(0){}

ConnectedComponent::ConnectedComponent(int componentID) : id(componentID), pixelCount(0){}

void ConnectedComponent::addPixel(int x, int y){
    pixels.emplace_back(x, y);
    pixelCount++;
}

int ConnectedComponent::getId() const{
    return id;
}

int ConnectedComponent::getPixelCount() const{
    return pixelCount;
}

const std::vector<std::pair<int, int>>& ConnectedComponent::getPixels() const{
    return pixels;
}