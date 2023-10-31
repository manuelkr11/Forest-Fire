#include "ff-ca.h"
#include <iostream>


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

ForestFireAutomata::ForestFireAutomata(int** input, int height, int width) : height(height), width(width) {
    status = new Status*[height];
    for (int i = 0; i < height; i++){
        status[i] = new Status[width];
    }
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
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

int ForestFireAutomata::getHeight() const {
    return height;
}

int ForestFireAutomata::getWidth() const {
    return width;
}

void ForestFireAutomata::simulate() {
    //TODO
}

void ForestFireAutomata::getStatusMatrix() const {
    //TODO
}
