#include "Student.hpp"

using std::string;

constexpr std::string_view CHARS =
    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!@#$";
constexpr size_t CHARS_SIZE = CHARS.size();

auto Student::get_generator() -> std::mt19937 & {
  static auto gen = std::mt19937(std::random_device{}());
  return gen;
}

auto Student::generate_string(size_t size) -> string {
  string str(size, ' ');
  auto gen = get_generator();

  while (size > 0) {
    auto rand = gen();

    while (rand > CHARS_SIZE && size > 0) {

      str[size] = CHARS[rand % CHARS.size()];

      --size;
      rand /= CHARS.size();
    }
  }
  return str;
}
