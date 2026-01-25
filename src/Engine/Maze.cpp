
#include "Engine/Maze.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <random>
#include <sstream>
#include <string>

//   █

Direction opposite_direction(Direction dir) {
  switch (dir) {
    case Direction::UP:
      return Direction::DOWN;
    case Direction::RIGHT:
      return Direction::LEFT;
    case Direction::DOWN:
      return Direction::UP;
    case Direction::LEFT:
      return Direction::RIGHT;
  }
}

std::string to_string(Direction dir) {
  switch (dir) {
    case Direction::UP:
      return "up";
    case Direction::RIGHT:
      return "right";
    case Direction::DOWN:
      return "down";
    case Direction::LEFT:
      return "left";
  }
}

Cell::Cell() {
  for (auto& dir : is_connected) {
    dir = false;
  }
}

bool Cell::exists_connetion(Direction dir) {
  return is_connected[(uint8_t)dir];
}

void Cell::set_connection(Direction dir, bool b) {
  is_connected[(uint8_t)dir] = b;
}

/// @brief Converts this maze to a string
std::string Cell::to_string() {
  switch (direction) {
    case Direction::UP:
      return "^";
      break;
    case Direction::RIGHT:
      return ">";
      break;
    case Direction::DOWN:
      return "v";
      break;
    case Direction::LEFT:
      return "<";
      break;
  }
  std::cerr << "could not convert cell direction to string";
  return "";
}

std::string Cell::to_string_rows(size_t row) {
  switch (row) {
    case 0: {
      if (exists_connetion(Direction::UP)) {
        return "█ █";
      } else {
        return "███";
      }
    }
    case 1: {
      std::string res = "";
      if (exists_connetion(Direction::LEFT)) {
        res += " ";
      } else {
        res += "█";
      }
      res += " ";
      if (exists_connetion(Direction::RIGHT)) {
        res += " ";
      } else {
        res += "█";
      }
      return res;
    }
    case 2: {
      if (exists_connetion(Direction::DOWN)) {
        return "█ █";
      } else {
        return "███";
      }
    }
    default:
      return "";
  }
}

static std::mt19937* maze_gen = nullptr;
static uint32_t current_seed = 42;

void set_maze_seed(uint32_t seed) {
  current_seed = seed;
  if (maze_gen) {
    delete maze_gen;
  }
  maze_gen = new std::mt19937(seed);
}

Direction pick_random() {
  if (!maze_gen) {
    maze_gen = new std::mt19937(current_seed);
  }
  std::uniform_int_distribution<> distr(0, 3);
  return static_cast<Direction>(distr(*maze_gen));
}

Point Point::shift(Direction direction) {
  switch (direction) {
    case Direction::UP:
      return {x, y - 1};
    case Direction::RIGHT:
      return {x + 1, y};
    case Direction::DOWN:
      return {x, y + 1};
    case Direction::LEFT:
      return {x - 1, y};
  }
}

std::string Point::to_string() {
  std::stringstream ss;
  ss << "(x: " << x << ", y: " << y << ")";
  return ss.str();
}
