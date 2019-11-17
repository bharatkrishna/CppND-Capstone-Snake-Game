#ifndef BONUS_H
#define BONUS_H

#include "SDL.h"

struct Bonus {
    bool place_bonus;
    SDL_Point loc;
    int interval;
};

#endif