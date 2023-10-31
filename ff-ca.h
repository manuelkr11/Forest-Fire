#pragma once

#include "neighbors.h"

class Status {
private:
    bool tree;
    bool fire;
public:
    Status();
    void set_fire();
    void set_tree();
    void reset_tree();
    void reset_fire();
    bool get_tree() const;
    bool get_fire() const;
};

class ForestFireAutomata {
private:
    int height;
    int width;
    Status** status;
    Neighborhood neighbors;

public:
    ForestFireAutomata(int** input, int height, int width);
    int getHeight() const;
    int getWidth() const;
    void simulate();
    void getStatusMatrix() const;
};