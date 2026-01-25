
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

// template <size_t Size>
// Maze<Size>::Maze() {
//   origin.x = Size - 1;
//   origin.y = Size - 1;

//   for (auto &row : rows) {
//     for (auto &cell : row) {
//       cell.direction = Direction::RIGHT;
//       cell.set_connection(Direction::LEFT, true);
//       cell.set_connection(Direction::RIGHT, true);
//     }

//     row.back().direction = Direction::DOWN;
//   }

//   // close off the ends
//   for (auto i = 0; i < Size; i++) {
//     rows[i][0].set_connection(Direction::LEFT, false);
//     rows[i][Size - 1].set_connection(Direction::RIGHT, false);
//     rows[i][Size - 1].set_connection(Direction::UP, true);
//     rows[i][Size - 1].set_connection(Direction::DOWN, true);
//   }
//   rows[0][Size - 1].set_connection(Direction::UP, false);
//   rows[Size - 1][Size - 1].set_connection(Direction::DOWN, false);
// }

//   TODO COMMENT QUADS ETC
// template <size_t Size>
// bool Maze<Size>::is_open(uint32_t y, uint32_t x) {
//   uint32_t quad_x = x % 3;
//   uint32_t quad_y = y % 3;

//   uint32_t cell_x = x / 3;
//   uint32_t cell_y = y / 3;

//   Cell &cell = get_cell(Point{x = cell_x, y = cell_y});
//   if ((quad_x == 0) && (quad_y == 0)) {
//     return false;
//   }
//   if ((quad_x == 1) && (quad_y == 0)) {
//     return cell.exists_connetion(Direction::UP);
//   }
//   if ((quad_x == 2) && (quad_y == 0)) {
//     return false;
//   }
//   if ((quad_x == 0) && (quad_y == 1)) {
//     return cell.exists_connetion(Direction::LEFT);
//   }
//   if ((quad_x == 1) && (quad_y == 1)) {
//     return true;
//   }
//   if ((quad_x == 2) && (quad_y == 1)) {
//     return cell.exists_connetion(Direction::RIGHT);
//   }
//   if ((quad_x == 0) && (quad_y == 2)) {
//     return false;
//   }
//   if ((quad_x == 1) && (quad_y == 2)) {
//     return cell.exists_connetion(Direction::DOWN);
//   }
//   if ((quad_x == 2) && (quad_y == 2)) {
//     return false;
//   }
// }

// // /// @brief Returns a new direction for the origin
// // template <size_t Size>
// // Direction Maze<Size>::random_valid_direction() {
// //   while (true) {
// //     Direction dir = pick_random();

// //     if (origin.x == 0 && dir == Direction::LEFT) {
// //       continue;
// //     }

// //     if (origin.y == 0 && dir == Direction::UP) {
// //       continue;
// //     }

// //     if (origin.x == Size - 1 && dir == Direction::RIGHT) {
// //       continue;
// //     }

// //     if (origin.y == Size - 1 && dir == Direction::DOWN) {
// //       continue;
// //     }

// //     return dir;
// //   }
// // }

// // /// @brief Converts this maze to a string
// // template <size_t Size>
// // std::string Maze<Size>::to_string() {
// //   auto ss = std::stringstream();

// //   ss << "\n";
// //   for (int y = 0; y < Size; y++) {
// //     for (int x = 0; x < Size; x++) {
// //       if (y == origin.y && x == origin.x) {
// //         ss << "o";
// //         continue;
// //       }

// //       ss << rows[y][x].to_string();
// //     }
// //     ss << "\n";
// //   }
// //   return ss.str();
// // }

// // template <size_t Size>
// // void Maze<Size>::pretty_print() {
//   for (auto &row : rows) {
//     for (int i = 0; i < 3; i++) {
//       for (auto &cell : row) {
//         std::cout << cell.to_string_rows(i);
//       }
//       std::cout << "\n";
//     }
//   }
// }

// template <size_t Size>
// Cell &Maze<Size>::get_origin() {
//   return rows[origin.y][origin.x];
// }

// template <size_t Size>
// Cell &Maze<Size>::get_cell(Point p) {
//   return rows[p.y][p.x];
// }

// template <size_t Size>
// void Maze<Size>::update_connections(Direction shift_dir) {
//   Cell &old_origin = get_origin();
//   Point new_origin_coordinates = origin.shift(shift_dir);
//   Cell &new_origin = get_cell(new_origin_coordinates);

//   Point new_origin_neighbor_cc =
//       new_origin_coordinates.shift(new_origin.direction);

//   new_origin.set_connection(new_origin.direction, false);
//   Cell &new_origin_neighbor = get_cell(new_origin_neighbor_cc);
//   new_origin_neighbor.set_connection(opposite_direction(new_origin.direction),
//                                      false);

//   old_origin.set_connection(shift_dir, true);
//   new_origin.set_connection(opposite_direction(shift_dir), true);
// }

// ///@brief shifts the origin one step
// template <size_t Size>
// void Maze<Size>::shift_origin() {
//   auto new_direction = random_valid_direction();
//   update_connections(new_direction);
//   Cell &origin_cell = get_origin();
//   origin_cell.direction = new_direction;
//   origin = origin.shift(new_direction);
// };

// ///@brief shifts the origin n times
// template <size_t Size>
// void Maze<Size>::n_shifts(uint32_t nb_shifts) {
//   for (int i = 0; i < nb_shifts; i++) {
//     shift_origin();
//   }
// }
