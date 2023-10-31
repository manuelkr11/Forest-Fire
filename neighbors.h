#pragma once

#include <vector>

class Neighborhood {
public:
    Neighborhood();
    void addNeighbor(int dx, int dy);
    int getNeighborCount() const;
    std::pair<int, int> getNeighbor(int index);
protected:
    std::vector<std::pair<int, int>> neighbors;
};

class vonMoore : public Neighborhood {
public:
    vonMoore();
};