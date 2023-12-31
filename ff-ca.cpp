#include "ff-ca.h"
#include <omp.h>
#include <random>
#include <chrono>


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

ForestFireAutomata::ForestFireAutomata(int width, int height, bool trees) 
        : height(height), width(width), probGrowth(0.00005), probCatchFire(0.0001) {
    status = new Status*[width];
    for (int i = 0; i < width; i++){
        status[i] = new Status[height];
    }

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 gen(seed);
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

void ForestFireAutomata::setFire(int x, int y){
    if(y < height || x < width) {
        status[x][y].set_fire();
    }
}

void ForestFireAutomata::simulate(int nthreads) {
    Status** old_status = new Status*[width];
    for (int i = 0; i < width; i++){
        old_status[i] = new Status[height];
    }
    
    #pragma omp parallel num_threads(nthreads)
	#pragma omp for
    for (int i = 0; i < width; i++){
        for (int j = 0; j < height; j++){
            old_status[i][j] = status[i][j];
        }
    }

    #pragma omp parallel num_threads(nthreads)
    {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 gen(seed);
    std::uniform_real_distribution<float> dis(0.0, 1.0);

	#pragma omp for
    for (int i = 0; i < width; i++){
        for (int j = 0; j < height; j++){
            if (!old_status[i][j].get_tree()) {
                float prob = dis(gen);
                if (prob<=probGrowth) {
                    status[i][j].set_tree();
                }
            }
            else {
                if (old_status[i][j].get_fire()) {
                    status[i][j].reset_tree();
                }
                else {
                    int fire_neighbor_count = 0;
                    for (int k = 0; k < neighbors.getNeighborCount(); k++) {
                        int x_neighbor = i + neighbors.getNeighbor(k).first;
                        int y_neighbor = j + neighbors.getNeighbor(k).second;
                        if ((x_neighbor>=0) && (x_neighbor<width) && (y_neighbor>=0) && (y_neighbor<height)) {
                            if(old_status[x_neighbor][y_neighbor].get_fire()){
                                fire_neighbor_count++;
                                break;
                            }
                        }
                    }
                    if (fire_neighbor_count > 0) {
                        status[i][j].set_fire();
                    }
                    else {
                        float prob = dis(gen);
                        if (prob<=probCatchFire) {
                            status[i][j].set_fire();
                        }
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