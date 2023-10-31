#include "neighbors.h"

Neighborhood::Neighborhood() {}

void Neighborhood::addNeighbor(int dx, int dy) {
    neighbors.push_back(std::make_pair(dx, dy));
}

int Neighborhood::getNeighborCount() const {
    return neighbors.size();
}

std::pair<int, int> Neighborhood::getNeighbor(int index) {
    if (index >= 0 && index < neighbors.size()) {
        return neighbors[index];
    }
    else {
        return std::make_pair(0,0);
    }
}

vonMoore::vonMoore() {
    addNeighbor(-1, -1);
    addNeighbor(-1, 0);
    addNeighbor(-1, 1);
    addNeighbor(0, -1);
    addNeighbor(0, 1);
    addNeighbor(1, -1);
    addNeighbor(1, 0);
    addNeighbor(1, 1);
}