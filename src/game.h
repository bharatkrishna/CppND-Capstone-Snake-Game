#ifndef GAME_H
#define GAME_H

#include <random>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"
#include "bonus.h"

class Game {
 public:
  Game(std::size_t grid_width, std::size_t grid_height);
  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration);
  int GetScore() const;
  int GetSize() const;

 private:
  Snake snake;
  Snake enemy_snake;
  SDL_Point food;
  Bonus bonus;
  Uint32 bonus_timer_start{0};

  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;
  std::uniform_int_distribution<int> bonus_interval;

  void PlaceFood();
  void PlaceBonus();
  void RemoveBonus();
  void UpdateBonus();
  void Update();
  void MoveEnemy();
  void ChangeDirection(Snake &snake, Snake::Direction input,
                       Snake::Direction opposite) const;
  void ConsumeFoodOrBonus(Snake &snake);
};

#endif