#ifndef BONUS_H
#define BONUS_H

#include "SDL.h"

struct Bonus {
    bool active;
    SDL_Point loc;
    int interval;
};

#endif