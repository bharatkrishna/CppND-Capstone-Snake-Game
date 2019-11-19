#include <iostream>
#include "text.h"

Text::Text(const std::string &font_path) : font_path(font_path) {}

void Text::display(int x, int y, SDL_Renderer *renderer, int font_size, const std::string &message_text, const SDL_Color &color) {
    text_texture = loadFont(renderer, font_path, font_size, message_text, color);
    SDL_QueryTexture(text_texture, nullptr, nullptr, &text_rect.w, &text_rect.h);

    text_rect.x = x;
    text_rect.y = y;
    SDL_RenderCopy(renderer, text_texture, nullptr, &text_rect);
}

SDL_Texture *Text::loadFont(SDL_Renderer *renderer, const std::string &font_path, int font_size, const std::string &message_text, const SDL_Color &color) {
    TTF_Font *font = TTF_OpenFont(font_path.c_str(), font_size);
    if (!font) {
        std::cerr << "Failed to load font\n";
    }

    auto text_surface = TTF_RenderText_Solid(font, message_text.c_str(), color);
    if (!text_surface) {
        std::cerr << "Failed to create text surface\n";
    }

    auto text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
    if (!text_texture) {
        std::cerr << "Failed to create text texture\n";
    }
    SDL_FreeSurface(text_surface);
    return text_texture;
}
