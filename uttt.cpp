/**
 * @file main.cpp
 * @author Cristiano Sansò (cristiano.sanso.04@gmail.com)
 * @brief 
 * @version 1.0
 * @date 2023-03-05
 * 
 * @copyright Copyright (c) 2023
 * 
 */


// include basic modules

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <filesystem>
#include <thread>
#include <chrono>

#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>

// include SDL

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

// include utils used to generate constants

#include "utils/clock.h"

// include variables

#include "variables/colors.h"
#include "variables/constants.h"
#include "variables/variables.h"

// include other utils used in the graphics

#include "utils/mathutils.h"

// include functions and classes

#include "utils/draw.h"
#include "utils/funcutils.h"

// include remaining game classes

#include "game/tempgame.h"
#include "game/game.h"
//#include "game/game 3x3.h"
#include "menus/main_menu.h"




// MAIN FUNCTION

int main(int argc, char *argv[]) {
    // setup

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        std::cout << "SDL could not initialize" << SDL_GetError() << std::endl;
    }

    TTF_Init();

    // run game's main menu

    MainMenu menu;
    menu.Run();

    // close everything and exit

    SDL_DestroyWindow(TRUESCREEN);
    SDL_Quit();

    return 0;
}
