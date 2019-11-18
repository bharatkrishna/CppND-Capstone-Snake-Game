#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include <memory>
#include "SDL.h"
#include "snake.h"
#include "bonus.h"
#include "text.h"

class Renderer {
 public:
  Renderer(const std::size_t screen_width, const std::size_t screen_height,
           const std::size_t grid_width, const std::size_t grid_height);
  ~Renderer();

  void Render(Snake snake, Snake const snake2, SDL_Point const &food, Bonus const &bonus);
  void UpdateWindowTitle(int fps);

 private:
  SDL_Window *sdl_window;
  SDL_Renderer *sdl_renderer;

  const std::size_t screen_width;
  const std::size_t screen_height;
  const std::size_t grid_width;
  const std::size_t grid_height;
  std::unique_ptr<Text> text;
};

#endif