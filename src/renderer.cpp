#include "renderer.h"
#include <iostream>
#include <string>
#include "tilemap.h"

Renderer::Renderer(const std::size_t screen_width,
                   const std::size_t screen_height,
                   const std::size_t grid_width, const std::size_t grid_height)
    : screen_width(screen_width),
      screen_height(screen_height),
      grid_width(grid_width),
      grid_height(grid_height) {
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
    std::cerr << "SDL could not initialize.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }

  if (TTF_Init() == -1) {
    std::cerr << "SDL could not initialize SDL_ttf.\n";
  }

  // Create Window
  sdl_window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED, screen_width,
                                screen_height, SDL_WINDOW_SHOWN);

  if (nullptr == sdl_window) {
    std::cerr << "Window could not be created.\n";
    std::cerr << " SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create renderer
  sdl_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED);
  if (nullptr == sdl_renderer) {
    std::cerr << "Renderer could not be created.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }

  // Add tile images
  Tilemap *tilemap = Tilemap::instance();
  tilemap->init(sdl_renderer, 32, 32);
  tilemap->addTile("../assets/grass.png", "grass");
  tilemap->addTile("../assets/apple.png", "food");
  tilemap->addTile("../assets/snake_head.png", "snake_head");
  tilemap->addTile("../assets/snake_body.png", "snake_body");
  tilemap->addTile("../assets/snake_head_dead.png", "snake_head_dead");
  tilemap->addTile("../assets/enemy_snake_head.png", "snake2_head");
  tilemap->addTile("../assets/enemy_snake_body.png", "snake2_body");
  tilemap->addTile("../assets/enemy_snake_head_dead.png", "snake2_head_dead");
  tilemap->addTile("../assets/frog.png", "bonus");

  //Create Text
  std::unique_ptr<Text> ptr_text(new Text( "../assets/FreeSans.ttf"));
  text = std::move(ptr_text);
}

Renderer::~Renderer() {
  SDL_DestroyWindow(sdl_window);
  SDL_Quit();
  TTF_Quit();
}

void Renderer::Render(Snake const snake, Snake const snake2, SDL_Point const &food, Bonus const &bonus) {
  SDL_Rect block;
  block.w = screen_width / grid_width;
  block.h = screen_height / grid_height;

  // Clear screen
  SDL_SetRenderDrawColor(sdl_renderer, 0x1E, 0x1E, 0x1E, 0xFF);
  SDL_RenderClear(sdl_renderer);

  // Render grass
  Tilemap::instance()->fillWith("grass", 0, 0, screen_width, screen_height);

  // Render food
  Tilemap::instance()->render("food", food.x * block.w, food.y * block.h);

  // Render bonus
  if (bonus.active)
    Tilemap::instance()->render("bonus", bonus.loc.x * block.w, bonus.loc.y * block.h);

  // Render snake's body
  SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
  for (SDL_Point const &point : snake.body) {
    block.x = point.x * block.w;
    block.y = point.y * block.h;
    Tilemap::instance()->render("snake_body", block.x, block.y);
  }

  // Render snake's head
  block.x = static_cast<int>(snake.head_x) * block.w;
  block.y = static_cast<int>(snake.head_y) * block.h;
  if (snake.alive) {
    Tilemap::instance()->render("snake_head", block.x, block.y);
  } else {
    Tilemap::instance()->render("snake_head_dead", block.x, block.y);
    text->display(screen_width - 380, screen_height - 25, sdl_renderer, 18, "Game Over!", {0xFF, 0x00, 0x00, 0xFF});
    text->display(screen_width - 450, screen_height - 80, sdl_renderer, 30, "Press Enter to Reset", {0xFF, 0xFF, 0xFF, 0xFF});
  }

  // Render snake2's body
  SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
  for (SDL_Point const &point : snake2.body) {
    block.x = point.x * block.w;
    block.y = point.y * block.h;
    Tilemap::instance()->render("snake2_body", block.x, block.y);
  }

  // Render snake2's head
  block.x = static_cast<int>(snake2.head_x) * block.w;
  block.y = static_cast<int>(snake2.head_y) * block.h;
  if (snake2.alive) {
    Tilemap::instance()->render("snake2_head", block.x, block.y);
  } else {
    Tilemap::instance()->render("snake2_head_dead", block.x, block.y);
  }

  if (snake.alive && !snake2.alive) {
    text->display(screen_width - 380, screen_height - 25, sdl_renderer, 18, "Enemy Dead!", {0xFF, 0xFF, 0xFF, 0xFF});
  }

  //Render score
  text->display(5, screen_height - 25, sdl_renderer, 18, "Score: " + std::to_string(snake.score), {0xFF, 0xFF, 0xFF, 0xFF});
  text->display(screen_width - 135, screen_height - 25, sdl_renderer, 18, "Enemy Score: " + std::to_string(snake2.score), {0xFF, 0xFF, 0xFF, 0xFF});

  // Update Screen
  SDL_RenderPresent(sdl_renderer);
}

void Renderer::UpdateWindowTitle(int fps) {
  std::string title{"Snake Game | FPS: " + std::to_string(fps)};
  SDL_SetWindowTitle(sdl_window, title.c_str());
}
