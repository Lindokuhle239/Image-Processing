#ifndef PGMIMAGEPROCESSOR_H
#define PGMIMAGEPROCESSOR_H

#include <memory>
#include <vector>
#include <string>
#include "ConnectedComponent.h"

class PGMimageProcessor{
    private:
        std::vector<std::unique_ptr<ConnectedComponent>> components;
        unsigned char* imageData;
        int width;
        int height;

    public:
        explicit PGMimageProcessor(const std::string& filename);
        ~PGMimageProcessor();

        PGMimageProcessor(const PGMimageProcessor&) = delete; //no copying
        PGMimageProcessor(PGMimageProcessor&&) noexcept;
        PGMimageProcessor& operator=(const PGMimageProcessor&) = delete;
        PGMimageProcessor& operator=(PGMimageProcessor&&) noexcept;

        int extractComponents(unsigned char threshold, int minValidSize);
        int filterComponentsBySize(int minSize, int maxSize);
        bool writeComponents(const std::string& outFilename);
        int getComponentCount() const;
        int getLargestSize() const;
        int getSmallestSize() const;
        void printComponentData(const ConnectedComponent& component) const;

    private:
        //helper methods
        bool readPGMFile(const std::string& filename);
        void BFS(int x, int y, unsigned char threshold, std::unique_ptr<ConnectedComponent>& components);
};

#endif