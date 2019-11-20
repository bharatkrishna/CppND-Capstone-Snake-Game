#include <iostream>
#include "controller.h"
#include "game.h"
#include "renderer.h"

int main() {
  constexpr std::size_t kFramesPerSecond{60};
  constexpr std::size_t kMsPerFrame{1000 / kFramesPerSecond};
  constexpr std::size_t kScreenWidth{640};
  constexpr std::size_t kScreenHeight{640};
  constexpr std::size_t kGridWidth{32};
  constexpr std::size_t kGridHeight{32};

  Renderer renderer(kScreenWidth, kScreenHeight, kGridWidth, kGridHeight);
  Controller controller;
  Game game(kGridWidth, kGridHeight-2);
  game.Run(controller, renderer, kMsPerFrame);
  std::pair<int, int> scores = game.GetScore();
  std::cout << "Game has terminated successfully!\n";
  std::cout << "Score: " << scores.first << "\n";
  std::cout << "Size: " << game.GetSize() << "\n";
  std::cout << "Enemy Score: " << scores.second << "\n";

  return 0;
}