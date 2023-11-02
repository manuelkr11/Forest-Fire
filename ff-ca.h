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
    Status& operator=(const Status& other);
};

class ForestFireAutomata {
private:
    int height;
    int width;
    Status** status;
    Neighborhood neighbors;
    float probGrowth;
    float probCatchFire;

public:
    ForestFireAutomata(int height, int width, int** input);
    ForestFireAutomata(int height, int weight, bool trees);
    ~ForestFireAutomata();
    int getHeight() const;
    int getWidth() const;
    void setProbGrowth(float probability);
    void setProbCatchFire(float probability);
    void simulate();
    int getStatus(int height, int width) const;
};