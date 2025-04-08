#ifndef PPMIMAGEPROCESSOR_H
#define PPMIMAGEPROCESSOR_H

#include "ConnectedComponent.h"
#include <memory>
#include <vector>
#include <string>
#include <utility>

class PPMimageProcessor {
    private:
        std::vector<std::unique_ptr<ConnectedComponent>> components;
        unsigned char* imageData; //3 bytes per pixel (RGB)
        int width;
        int height;

    public:
        explicit PPMimageProcessor(const std::string& filename);
        ~PPMimageProcessor();

        //disallow copying
        PPMimageProcessor(const PPMimageProcessor&) = delete;
        PPMimageProcessor& operator=(const PPMimageProcessor&) = delete;

        //allow moving 
        PPMimageProcessor(PPMimageProcessor&& other) noexcept;
        PPMimageProcessor& operator=(PPMimageProcessor&& other) noexcept;

        //core functionality
        int extractComponents(unsigned char threshold, int minValidSize);
        int filterComponentsBySize(int minSize, int maxSize);
        bool writeComponents(const std::string& outFileName);
        bool writeBoxedComponents(const std::string& outFileName, unsigned char boxColor[3] = nullptr);

        //getters
        int getComponentCount() const;
        int getLargestSize() const;
        int getSmallestSize() const;
        void printComponentData(const ConnectedComponent& component) const;
        int getWidth() const { return width; };
        int getHeight() const { return height; };

    private:
        //helper methods
        bool readPPMFile(const std::string& filename);
        void BFS(int x, int y, unsigned char threshold, bool* visited, std::unique_ptr<ConnectedComponent>& component);
        unsigned char rgbToGrayScale(unsigned char r, unsigned char g, unsigned char b);
};
#endif