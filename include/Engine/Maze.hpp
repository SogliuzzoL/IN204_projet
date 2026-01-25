#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <string>

//   █

enum class Direction : uint8_t { UP, RIGHT, DOWN, LEFT };

Direction opposite_direction(Direction dir);

std::string to_string(Direction dir);

struct Cell {
  Direction direction;
  std::array<bool, 4> is_connected;

  bool exists_connetion(Direction dir);

  void set_connection(Direction dir, bool b);

  Cell();

  /// @brief Converts this maze to a string
  std::string to_string();

  std::string to_string_rows(size_t row);
};

void set_maze_seed(uint32_t seed);
Direction pick_random();

struct Point {
  uint32_t x;
  uint32_t y;

  Point shift(Direction direction);

  std::string to_string();
};

template <size_t Size>
struct Maze {
  std::array<std::array<Cell, Size>, Size> rows;
  Point origin;

  Maze() {
    origin.x = Size - 1;
    origin.y = Size - 1;

    for (auto &row : rows) {
      for (auto &cell : row) {
        cell.direction = Direction::RIGHT;
        cell.set_connection(Direction::LEFT, true);
        cell.set_connection(Direction::RIGHT, true);
      }

      row.back().direction = Direction::DOWN;
    }

    // close off the ends
    for (auto i = 0; i < Size; i++) {
      rows[i][0].set_connection(Direction::LEFT, false);
      rows[i][Size - 1].set_connection(Direction::RIGHT, false);
      rows[i][Size - 1].set_connection(Direction::UP, true);
      rows[i][Size - 1].set_connection(Direction::DOWN, true);
    }
    rows[0][Size - 1].set_connection(Direction::UP, false);
    rows[Size - 1][Size - 1].set_connection(Direction::DOWN, false);
  }

  /// @brief Returns a new direction for the origin
  Direction random_valid_direction() {
    while (true) {
      Direction dir = pick_random();

      if (origin.x == 0 && dir == Direction::LEFT) {
        continue;
      }

      if (origin.y == 0 && dir == Direction::UP) {
        continue;
      }

      if (origin.x == Size - 1 && dir == Direction::RIGHT) {
        continue;
      }

      if (origin.y == Size - 1 && dir == Direction::DOWN) {
        continue;
      }

      return dir;
    }
  }

  //   /// @brief Converts this maze to a string
  //   std::string to_string() {
  //     auto ss = std::stringstream();

  //     ss << "\n";
  //     for (int y = 0; y < Size; y++) {
  //       for (int x = 0; x < Size; x++) {
  //         if (y == origin.y && x == origin.x) {
  //           ss << "o";
  //           continue;
  //         }

  //         ss << rows[y][x].to_string();
  //       }
  //       ss << "\n";
  //     }
  //     return ss.str();
  //   }

  void pretty_print() {
    for (auto &row : rows) {
      for (int i = 0; i < 3; i++) {
        for (auto &cell : row) {
          std::cout << cell.to_string_rows(i);
        }
        std::cout << "\n";
      }
    }
  }

  Cell &get_origin() { return rows[origin.y][origin.x]; }
  Cell &get_cell(Point p) { return rows[p.y][p.x]; }

  void update_connections(Direction shift_dir) {
    Cell &old_origin = get_origin();
    Point new_origin_coordinates = origin.shift(shift_dir);
    Cell &new_origin = get_cell(new_origin_coordinates);

    Point new_origin_neighbor_cc =
        new_origin_coordinates.shift(new_origin.direction);

    new_origin.set_connection(new_origin.direction, false);
    Cell &new_origin_neighbor = get_cell(new_origin_neighbor_cc);
    new_origin_neighbor.set_connection(opposite_direction(new_origin.direction),
                                       false);

    old_origin.set_connection(shift_dir, true);
    new_origin.set_connection(opposite_direction(shift_dir), true);
  }

  ///@brief shifts the origin one step
  void shift_origin() {
    auto new_direction = random_valid_direction();
    update_connections(new_direction);
    Cell &origin_cell = get_origin();
    origin_cell.direction = new_direction;
    origin = origin.shift(new_direction);
  };

  ///@brief shifts the origin n times
  void n_shifts(uint32_t nb_shifts) {
    for (int i = 0; i < nb_shifts; i++) {
      shift_origin();
    }
  }
  bool is_open(uint32_t y, uint32_t x) {
    uint32_t quad_x = x % 3;
    uint32_t quad_y = y % 3;

    uint32_t cell_x = x / 3;
    uint32_t cell_y = y / 3;

    Cell &cell = get_cell(Point{x = cell_x, y = cell_y});
    if ((quad_x == 0) && (quad_y == 0)) {
      return false;
    }
    if ((quad_x == 1) && (quad_y == 0)) {
      return cell.exists_connetion(Direction::UP);
    }
    if ((quad_x == 2) && (quad_y == 0)) {
      return false;
    }
    if ((quad_x == 0) && (quad_y == 1)) {
      return cell.exists_connetion(Direction::LEFT);
    }
    if ((quad_x == 1) && (quad_y == 1)) {
      return true;
    }
    if ((quad_x == 2) && (quad_y == 1)) {
      return cell.exists_connetion(Direction::RIGHT);
    }
    if ((quad_x == 0) && (quad_y == 2)) {
      return false;
    }
    if ((quad_x == 1) && (quad_y == 2)) {
      return cell.exists_connetion(Direction::DOWN);
    }
    if ((quad_x == 2) && (quad_y == 2)) {
      return false;
    }
  }
};

// template <size_t Size>
// struct Maze {
//   std::array<std::array<Cell, Size>, Size> rows;
//   Point origin;

//   Maze();

//   //   TODO COMMENT QUADS ETC
//   bool is_open(uint32_t y, uint32_t x);

//   /// @brief Returns a new direction for the origin
//   Direction random_valid_direction();

//   /// @brief Converts this maze to a string
//   std::string to_string();

//   void pretty_print();

//   Cell &get_origin();
//   Cell &get_cell(Point p);

//   void update_connections(Direction shift_dir);

//   ///@brief shifts the origin one step
//   void shift_origin();

//   ///@brief shifts the origin n times
//   void n_shifts(uint32_t nb_shifts);
// };
