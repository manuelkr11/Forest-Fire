#include "ff-ca.h"
#include <iostream>
#include <omp.h>
#include <random>


Status::Status() : tree(false), fire(false) {}

void Status::set_fire(){
    if(tree){
        fire = true;
    }
}

void Status::set_tree(){
    tree = true;
}

void Status::reset_tree(){
    tree = false;
    fire = false;
}

void Status::reset_fire(){
    fire = false;
}

bool Status::get_tree() const {
    return tree;
}

bool Status::get_fire() const {
    return fire;
}

Status& Status::operator=(const Status& other) {
    if (this == &other) {
        return *this;
    }

    tree = other.tree;
    fire = other.fire;

    return *this;
}

ForestFireAutomata::ForestFireAutomata(int width, int height, int** input) 
        : height(height), width(width), probGrowth(0.9), probCatchFire(0.) {
    status = new Status*[this->width];
    for (int i = 0; i < this->width; i++){
        status[i] = new Status[this->height];
    }
    for (int i = 0; i < width; i++){
        for (int j = 0; j < height; j++){
            switch(input[i][j]){
                case 1:
                    status[i][j].set_tree();
                    break;
                case 2:
                    status[i][j].set_tree();
                    status[i][j].set_fire();
            }
        }
    }
}

ForestFireAutomata::ForestFireAutomata(int width, int height, bool trees) 
        : height(height), width(width), probGrowth(0.01), probCatchFire(0.2) {
    status = new Status*[width];
    for (int i = 0; i < width; i++){
        status[i] = new Status[height];
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.0, 1.0);

    for (int i = 0; i < width; i++){
        for (int j = 0; j < height; j++){
            if (trees) {
                float random = dis(gen);
                if(random <= probGrowth) {
                    status[i][j].set_tree();
                }
            }
        }
    }
}

ForestFireAutomata::~ForestFireAutomata() {
    for (int i = 0; i < width; i++) {
        delete[] status[i];
    }
    delete[] status;
}

int ForestFireAutomata::getHeight() const {
    return height;
}

int ForestFireAutomata::getWidth() const {
    return width;
}

void ForestFireAutomata::setProbGrowth(float probability) {
    probGrowth = probability;
}

void ForestFireAutomata::setProbCatchFire(float probability) {
    probCatchFire = probability;
}

void ForestFireAutomata::setTree(int x, int y){
    if(y < height || x < width) {
        status[x][y].set_tree();
    }
}

void ForestFireAutomata::simulate() {
    Status** old_status = new Status*[width];
    for (int i = 0; i < width; i++){
        old_status[i] = new Status[height];
    }
    for (int i = 0; i < width; i++){
        for (int j = 0; j < height; j++){
            old_status[i][j] = status[i][j];
        }
    }

    for (int i = 0; i < width; i++){
        for (int j = 0; j < height; j++){
            if (!old_status[i][j].get_tree()) {
                std::random_device rd;
                std::mt19937 gen(rd());
                std::uniform_real_distribution<float> dis(0.0, 1.0);
                float prob = dis(gen);
                if (prob<=probGrowth) {
                    status[i][j].set_tree();
                }
            }
            else {
                if (old_status[i][j].get_fire()) {
                    status[i][j].reset_tree();  //burn down
                }
                else {
                    int fire_neighbor_count = 0;
                    for (int k = 0; k < neighbors.getNeighborCount(); k++) { //TODO could work with breakk
                        int x_neighbor = i + neighbors.getNeighbor(k).first;
                        int y_neighbor = j + neighbors.getNeighbor(k).second;
                        if ((x_neighbor>=0) && (x_neighbor<width) && (y_neighbor>=0) && (y_neighbor<height)) {
                            if(old_status[x_neighbor][y_neighbor].get_fire()){
                                fire_neighbor_count++;
                            }
                        }
                    }
                    if (fire_neighbor_count > 0) {
                        status[i][j].set_fire();
                    }
                    else {
                        std::random_device rd;
                        std::mt19937 gen(rd());
                        std::uniform_real_distribution<float> dis(0.0, 1.0);
                        float prob = dis(gen);
                        if (prob<=probCatchFire) {
                            status[i][j].set_fire();
                        }
                    }
                }
            }
        }
    }

    for (int i = 0; i < width; i++) {
        delete[] old_status[i];
    }
    delete[] old_status;
}

int ForestFireAutomata::getStatus(int x, int y) const {
    if (y >= height || x >= width) {
        return 0;
    }
    else if (status[x][y].get_tree()) {
        if (status[x][y].get_fire()) {
            return 2;
        }
        else {
            return 1;
        }
    }
    else {
        return 0;
    }
}