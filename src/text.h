/* Code based on https://www.youtube.com/watch?v=FIjj6UVXtXk */

#ifndef TEXT_H
#define TEXT_H

#include "SDL.h"
#include "SDL_ttf.h"
#include <string>

class Text {
public:
    Text(const std::string &font_path);
    void display(int x, int y, SDL_Renderer *renderer, int font_size, const std::string &message_text, const SDL_Color &color);
    static SDL_Texture *loadFont(SDL_Renderer *renderer, const std::string &font_path, int font_size, const std::string &message_text, const SDL_Color &color);

private:
    std::string font_path;
    SDL_Texture *text_texture{nullptr};
    SDL_Rect text_rect;
};

#endif