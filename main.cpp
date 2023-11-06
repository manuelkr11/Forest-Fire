#include <iostream>
#include <fstream>
#include <SDL2/SDL.h>
#include <string>
#include <random>
#include <omp.h>
#include "ff-ca.h"
#include "Timing.h"

int main (int argc, char* argv[]) {

    bool measure = false;
    int nthreads = 1;
    int generations = 100;

    int screen_width = 1600;
    int screen_height = 1024;

    int width = 1024;
    int height = 1024;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--width") {
            if (i <= argc) {
				width = std::stoi(argv[++i]);
			}
        }
        else if (arg == "--height") {
            if (i <= argc) {
				height = std::stoi(argv[++i]);
			}
        }
        else if (arg == "--measure") {
            measure = true;
        }
        else if (arg == "--threads") {
            if (i <= argc) {
                nthreads = std::stoi(argv[++i]);
            }
        }
        else if (arg == "--generations") {
            if (i <= argc) {
                generations = std::stoi(argv[++i]);
            }
        }
    }

    ForestFireAutomata forest_fire(width, height, true);
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 gen(seed);
    std::uniform_real_distribution<float> dis(0.0, 1.0);
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            float probability = dis(gen);
            if (probability < 0.7 - ((std::sqrt((i - height / 2) * (i - height / 2) + (j - width / 2) * (j - width / 2))) / (std::max(width, height) / 2))*0.8) {
                forest_fire.setTree(i, j);
            }
        }
    }

    if (width <= screen_width && height <= screen_height && !measure){

        int tile_size = std::min(screen_width/width, screen_height/height);
        screen_width = width * tile_size;
        screen_height = height * tile_size;

        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
            return 1;
        }
        SDL_Window* window = SDL_CreateWindow("Solar System Physics Simulation", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_width, screen_height, SDL_WINDOW_SHOWN);
        if (!window) {
            std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
            return 1;
        }
        SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (!renderer) {
            std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
            SDL_DestroyWindow(window);
            SDL_Quit();
            return 1;
        }

        bool quit = false;
        SDL_Event e;

        while (!quit) {
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT) {
                    quit = true;            
                }
                else if (e.type == SDL_KEYDOWN) {
                    if (e.key.keysym.sym == SDLK_ESCAPE) {
                        quit = true;
                    }
                }
            }

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);

            for (int i = 0; i < width; i++) {
                for (int j = 0; j < height; j++) {
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                    if (forest_fire.getStatus(i, j)==1) {
                        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                        SDL_Rect tileRect = {tile_size*i, tile_size*j, tile_size, tile_size};
                        SDL_RenderFillRect(renderer, &tileRect);
                    } else if (forest_fire.getStatus(i, j)==2) {
                        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                        SDL_Rect tileRect = {tile_size*i, tile_size*j, tile_size, tile_size};
                        SDL_RenderFillRect(renderer, &tileRect);
                    }
                }
            }

            SDL_RenderPresent(renderer);

            forest_fire.simulate(nthreads);
        }

        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }
    else {

        Timing *timing = Timing::getInstance();
        timing->startComputation();
        for (int i = 0; i < generations; i++) {
            forest_fire.simulate(nthreads);
        }
        timing->stopComputation();
        std::string filename = "timing.txt";
        std::ofstream timefile (filename, std::ios::app);
        timefile << "Resolution " << width << "x" << height << ", " << generations << " Generations:" << std::endl;
		timefile << timing->getResults() << "  (" << nthreads << " threads)" << std::endl;
		timefile.close(); 
    }

    return 0;
}