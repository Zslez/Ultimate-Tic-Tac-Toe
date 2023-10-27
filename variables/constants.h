/**
 * @file constants.h
 * @author Cristiano Sansò (cristiano.sanso.04@gmail.com)
 * @brief This file contains the definitions of the constant values used in the application
 * @copyright Copyright (c) 2023
 */


#define GAME_IS_PAID    1

#define NEXT_LEVEL      3
#define PREVIOUS_WIN    2
#define CLOSE_WINDOW    1
#define DO_NOTHING      0
#define RESTART        -1

#define FPS SECOND



// PALETTES

const std::vector<COLORTYPE> PALETTE_CLASSIC = {
    {224, 108, 117},
    {152, 195, 121},
    { 97, 175, 239},
    {229, 192, 123},
    {198, 120, 221}
};

const std::vector<COLORTYPE> PALETTE_HOT_COOL = {
    {208,  49,  97},
    {191, 216, 209},
    { 43,  55,  75},
    {238, 128, 128},
    { 23, 138, 148}
};

const std::vector<COLORTYPE> PALETTE_PASTEL_1 = {
    {225, 163, 111},
    {226, 216, 165},
    { 87, 126, 137},
    {222, 196, 132},
    {111, 159, 156}
};

const std::vector<COLORTYPE> PALETTE_PASTEL_2 = {
    {236, 230, 218},
    { 78,  97,  91},
    { 31,  37,  50},
    {202, 206, 181},
    { 47,  53,  67}
};



// TYPE DEFINITIONS

#define FLOWS std::vector<Flow>
#define CHUNKS std::vector< std::vector<int> >

#define INTPOS std::array<int, 2>
#define DOUBLEPOS std::array<double, 2>



// SCREEN CONSTANTS

Clock CLOCK;

const int SECOND = CLOCK.GetCurrentlySetFPS();


//const int FULLWIDTH = 1920;
//const int FULLHEIGHT = 1080;

const int WIDTH = 630;
const int HEIGHT = 630;

SDL_Window *TRUESCREEN = SDL_CreateWindow(
    "Ultimate Tic-Tac-Toe",
    SDL_WINDOWPOS_UNDEFINED,
    SDL_WINDOWPOS_UNDEFINED,
    WIDTH,
    HEIGHT,
    SDL_WINDOW_ALLOW_HIGHDPI //| SDL_WINDOW_FULLSCREEN_DESKTOP //| SDL_RENDERER_ACCELERATED
);

int FULLWIDTH;
int FULLHEIGHT;


int GetSize() {SDL_GetWindowSize(TRUESCREEN, &FULLWIDTH, &FULLHEIGHT); return 0;}
int a = GetSize();


SDL_Renderer *RENDERER = SDL_CreateRenderer(TRUESCREEN, -1, 0);




// TEXTURES

const std::vector<SDL_Texture*> textures = {
    SDL_CreateTextureFromSurface(RENDERER, IMG_Load("data/images/x.png")),
    SDL_CreateTextureFromSurface(RENDERER, IMG_Load("data/images/o.png"))
};




// MAPS

const std::vector<int> boardMap = {
    0, 1, 2, 9, 10, 11, 18, 19, 20,
    3, 4, 5, 12, 13, 14, 21, 22, 23,
    6, 7, 8, 15, 16, 17, 24, 25, 26,
    27, 28, 29, 36, 37, 38, 45, 46, 47,
    30, 31, 32, 39, 40, 41, 48, 49, 50,
    33, 34, 35, 42, 43, 44, 51, 52, 53,
    54, 55, 56, 63, 64, 65, 72, 73, 74,
    57, 58, 59, 66, 67, 68, 75, 76, 77,
    60, 61, 62, 69, 70, 71, 78, 79, 80
};



std::vector<int> pointsMap = {
    3, 2, 3, 2, 2, 2, 3, 2, 3,
    3, 2, 3, 2, 2, 2, 3, 2, 3,
    3, 2, 3, 2, 2, 2, 3, 2, 3,
    3, 2, 3, 2, 2, 2, 3, 2, 3,
    3, 2, 3, 2, 2, 2, 3, 2, 3,
    3, 2, 3, 2, 2, 2, 3, 2, 3,
    3, 2, 3, 2, 2, 2, 3, 2, 3,
    3, 2, 3, 2, 2, 2, 3, 2, 3,
    3, 2, 3, 2, 2, 2, 3, 2, 3
};



const std::vector< std::vector<int> > winMap = {
    // rows

    {0, 1, 2},
    {3, 4, 5},
    {6, 7, 8},

    // columns

    {0, 3, 6},
    {1, 4, 7},
    {2, 5, 8},

    // diagonals

    {0, 4, 8},
    {2, 4, 6},
};



const std::vector<int> smallBoardPointsBase = {
    20, 15, 20,
    15, 25, 15,
    20, 15, 20
};



const std::array<int, 3> XWIN = {-1, 0, 0};
const std::array<int, 3> OWIN = {-1, 1, 1};



// GAME CONSTANTS

const int INFINITYVALUE = 65536;
const int MAXSEARCHDEPTH = 6;
const int ENDGAMEMOVESCONST = 26;
int ENDGAMEMOVES = ENDGAMEMOVESCONST;
const int TABLEDEPTH = ENDGAMEMOVESCONST - 6;
const int WINVALUE = 1000;

const int SMALLWINCOMBVALUE = 2;
const int BIGWINCOMVALUE = 20;
