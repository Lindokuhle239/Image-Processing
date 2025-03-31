#ifndef CONNECTEDCOMPONENT_H
#define CONNECTEDCOMPONENT_H
#include <vector>
#include <utility>

class ConnectedComponent{
    private:
        int id;
        int pixelCount;
        std::vector<std::pair<int, int>> pixels;

    public:
        ConnectedComponent();
        explicit ConnectedComponent(int componentID);

        ~ConnectedComponent() = default;
        ConnectedComponent(const ConnectedComponent&) = default;
        ConnectedComponent(ConnectedComponent&&) = default;
        ConnectedComponent& operator=(const ConnectedComponent&) = default;
        ConnectedComponent& operator=(ConnectedComponent&&) = default;

        void addPixel(int x, int y);
        int getId() const;
        int getPixelCount() const;
        const std::vector<std::pair<int, int>>& getPixels() const;
};

#endif