#include "game.h"
#include <iostream>
#include "SDL.h"

Game::Game(std::size_t grid_width, std::size_t grid_height)
    : snake(grid_width, grid_height),
      enemy_snake(grid_width, grid_height),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width)-1),
      random_h(0, static_cast<int>(grid_height)-1),
      bonus_interval(20, 30) {
  PlaceFood();
  bonus.place_bonus = false;
  bonus.interval = 10;
  enemy_snake.speed = 0.08;
  enemy_snake.head_x = snake.head_x - 10;
  enemy_snake.head_y = snake.head_y - 10;
}

void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;
  bonus_timer_start = SDL_GetTicks();

  while (running) {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, snake);
    Update();
    renderer.Render(snake, enemy_snake, food, bonus);

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }
    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }
}

void Game::PlaceBonus() {
  bonus.place_bonus = true;
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item before placing
    // food.
    if (!snake.SnakeCell(x, y) && !enemy_snake.SnakeCell(x, y)) {
      bonus.loc.x = x;
      bonus.loc.y = y;
      std::cout << "Bonus placed at: " << x << "," << y << "\n";
      return;
    }
  }
}

void Game::RemoveBonus() {
  bonus.place_bonus = false;
  bonus.loc.x = -1;
  bonus.loc.y = -1;
  bonus.interval = bonus_interval(engine);
  // std::cout << "Bonus interval: " << bonus.interval << "\n";
}

void Game::PlaceFood() {
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item before placing
    // food.
    if (!snake.SnakeCell(x, y) && !enemy_snake.SnakeCell(x, y)) {
      food.x = x;
      food.y = y;
      std::cout << "Food placed at: " << x << "," << y << "\n";
      return;
    }
  }
}

void Game::UpdateBonus() {
  Uint32 elapsed_time;
  Uint32 bonus_timer_end = SDL_GetTicks();
  elapsed_time =  bonus_timer_end - bonus_timer_start;
  // Place bonus every `interval` seconds
  if (bonus_timer_end - bonus_timer_start >= bonus.interval * 1000) {
    bonus_timer_start = bonus_timer_end;
    PlaceBonus();
  }
  // Remove bonus in 5 seconds
  if (bonus.place_bonus && bonus_timer_end - bonus_timer_start >= 5 * 1000) {
    RemoveBonus();
  }
}

void Game::Update() {
  if (!snake.alive) return;

  snake.Update();
  if (enemy_snake.alive) {
    MoveEnemy();
    enemy_snake.Update();
  }
  UpdateBonus();

  int new_x = static_cast<int>(snake.head_x);
  int new_y = static_cast<int>(snake.head_y);
  // Check if there's food over here
  if (food.x == new_x && food.y == new_y) {
    std::cout << "Snake at: " << new_x << "," << new_y << "\n";
    snake.score++;
    PlaceFood();
    // Grow snake and increase speed.
    snake.GrowBody();
    snake.speed += 0.02;
  }

  if (bonus.loc.x == new_x && bonus.loc.y == new_y) {
    snake.score += 2;
    // Grow snake.
    snake.double_growth = true;
    snake.GrowBody();
    snake.speed += 0.02;
    RemoveBonus();
  }

  // For enemy snake
  int enemy_snake_new_x = static_cast<int>(enemy_snake.head_x);
  int enemy_snake_new_y = static_cast<int>(enemy_snake.head_y);
  if (food.x == enemy_snake_new_x && food.y == enemy_snake_new_y) {
    enemy_snake.score++;
    PlaceFood();
    enemy_snake.GrowBody();
  }
}

void Game::MoveEnemy(){
  if (static_cast<int>(enemy_snake.head_x) > food.x) {
    enemy_snake.direction = Snake::Direction::kLeft;
  }
  else if (static_cast<int>(enemy_snake.head_x) < food.x) {
    enemy_snake.direction = Snake::Direction::kRight;
  }

  if (static_cast<int>(enemy_snake.head_y) > food.y) {
    enemy_snake.direction = Snake::Direction::kUp;
  }
  else if (static_cast<int>(enemy_snake.head_y) < food.y) {
    enemy_snake.direction = Snake::Direction::kDown;
  }
  return;
}

int Game::GetScore() const { return snake.score; }
int Game::GetSize() const { return snake.size; }