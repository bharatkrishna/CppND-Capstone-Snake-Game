#include "tilemap.h"
#include <iostream>

Tilemap* Tilemap::_instance = nullptr;

void Tilemap::init(SDL_Renderer* renderer, int tile_w, int tile_h) {
  this->renderer = renderer;
  this->tile_w = tile_w;
  this->tile_h = tile_h;
}

bool Tilemap::addTile(std::string file_path, std::string id) {
  SDL_Surface* temp_surface = IMG_Load(file_path.c_str());

  if (temp_surface == 0) {
    std::cout << "Image load fail\n"; 
    return false;
  }

  SDL_Texture* texture = SDL_CreateTextureFromSurface(
      renderer, temp_surface);

  if (texture == 0) {
    std::cout << "Could not create texture: " + file_path << "\n";
    return false;
  }

  SDL_FreeSurface(temp_surface);

  if (texture != 0) {
    tiles[id] = texture;
    return true;
  }

  std::cout << "Could not load image: " + file_path << "\n";
  return false;
}

void Tilemap::render(std::string id, int x, int y) {
  SDL_Rect src_rect;
  SDL_Rect dest_rect;

  src_rect.x = 0;
  src_rect.y = 0;
  src_rect.w = tile_w;
  src_rect.h = tile_h;

  dest_rect.x = x;
  dest_rect.y = y;
  dest_rect.w = tile_w;
  dest_rect.h = tile_h;

  SDL_RenderCopyEx(
      renderer, tiles[id], &src_rect, &dest_rect, 0, 0, SDL_FLIP_NONE);
}

void Tilemap::fillWith(std::string id, int x, int y, int w, int h) {
  for (int cur_x = x; cur_x < w; cur_x += tile_w) {
    for (int cur_y = y; cur_y < h; cur_y += tile_h) {
      render(id, cur_x, cur_y-30); // -30 --> hack to leave the bottom of window without filling
    }
  }
}

void Tilemap::clean() {
}
