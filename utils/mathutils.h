/**
 * @file mathutils.h
 * @author Cristiano Sansò (cristiano.sanso.04@gmail.com)
 * @brief This file contains many functions to perform various math calculations
 * @copyright Copyright (c) 2023
 */


#include <cmath>


double Deg2Rad(double angle) {
    return angle * M_PI / 180;
}


double Rad2Deg(double angle) {
    return angle * 180 / M_PI;
}


double PointsDistance(int x1, int y1, int x2, int y2) {
    return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}


bool IsInside(int posx, int posy, int x1, int y1, int x2, int y2) {
    return posx >= x1 and posx <= x2 and posy >= y1 and posy <= y2;
}


bool IsInsideScreen(int posx, int posy, int addValue = 0) {
    return IsInside(posx, posy, -addValue, -addValue, WIDTH + addValue, HEIGHT + addValue);
}


bool IsMouseInside(int x1, int y1, int x2, int y2) {
    int mousex, mousey;

    SDL_GetMouseState(&mousex, &mousey);

    return IsInside(mousex * WIDTH / FULLWIDTH, mousey * HEIGHT / FULLHEIGHT, x1, y1, x2, y2);
}


bool IsMouseInsideRect(SDL_Rect rect) {
    return IsMouseInside(rect.x, rect.y, rect.x + rect.w, rect.y + rect.h);
}


int randint(int range) {
    for (int i = 0; i < 5; i++) srand(time(NULL) + rand());

    return rand() % range;
}


int weightedRandint(std::vector<int> weights) {
    int sumOfWeights = 0;
    int numChoices = (int) weights.size();

    for(int i = 0; i < numChoices; i++) {
        sumOfWeights += weights[i];
    }

    int result = randint(sumOfWeights);

    for(int i = 0; i < numChoices; i++) {
        if(result < weights[i]) return i;

        result -= weights[i];
    }

    return result;
}
