#include "game.h"
#include <iostream>
#include "SDL.h"

Game::Game(std::size_t grid_width, std::size_t grid_height)
    : snake(grid_width, grid_height),
      enemy_snake(grid_width, grid_height),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width)),
      random_h(0, static_cast<int>(grid_height)) {
  PlaceFood();
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

  while (running) {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, snake);
    Update();
    renderer.Render(snake, enemy_snake, food);

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(score, frame_count);
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

void Game::Update() {
  if (!snake.alive) return;

  MoveEnemy();
  snake.Update();
  enemy_snake.Update();

  int new_x = static_cast<int>(snake.head_x);
  int new_y = static_cast<int>(snake.head_y);
  // Check if there's food over here
  if (food.x == new_x && food.y == new_y) {
    // std::cout << "Snake at: " << new_x << "," << new_y << "\n";
    score++;
    PlaceFood();
    // Grow snake and increase speed.
    snake.GrowBody();
    snake.speed += 0.02;
  }
  // For enemy snake
  new_x = static_cast<int>(enemy_snake.head_x);
  new_y = static_cast<int>(enemy_snake.head_y);
  if (food.x == new_x && food.y == new_y) {
    // std::cout << "Enemy Snake at: " << new_x << "," << new_y << "\n";
    PlaceFood();
    enemy_snake.GrowBody();
  }
}

int Game::GetScore() const { return score; }
int Game::GetSize() const { return snake.size; }


void Game::MoveEnemy(){
  // std::cout << "Snake2 at: " << enemy_snake.head_x << "," << enemy_snake.head_y << "\n";
  // std::cout << "Food at: " << food.x << "," << food.y << "\n";

  if (static_cast<int>(enemy_snake.head_x) > food.x) {
    // snake2.direction = Snake::Direction::kRight;
    enemy_snake.direction = Snake::Direction::kLeft;
    // std::cout << "Moving left, Snake2 at x: " << enemy_snake.head_x << "\n";
  }
  else if (static_cast<int>(enemy_snake.head_x) < food.x) {
    // snake2.direction = Snake::Direction::kLeft;
    enemy_snake.direction = Snake::Direction::kRight;
    // std::cout << "Moving left, Snake2 at x: " << enemy_snake.head_x << "\n";
  }

  if (static_cast<int>(enemy_snake.head_y) > food.y) {
    enemy_snake.direction = Snake::Direction::kUp;
  }
  else if (static_cast<int>(enemy_snake.head_y) < food.y) {
    enemy_snake.direction = Snake::Direction::kDown;
  }
  return;
}
