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
    int width;
    int height;
    Status** status;
    vonMoore neighbors;
    float probGrowth;
    float probCatchFire;

public:
    ForestFireAutomata(int width, int height, bool trees);
    ~ForestFireAutomata();
    int getHeight() const;
    int getWidth() const;
    void setProbGrowth(float probability);
    void setProbCatchFire(float probability);
    void setTree(int x, int y);
    void simulate(int nthreads);
    int getStatus(int x, int y) const;
};