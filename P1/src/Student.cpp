#include "Student.hpp"

constexpr std::string_view CHARS =
    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!@#$";
constexpr size_t CHARS_SIZE = CHARS.size();

auto Student::get_generator() -> std::mt19937 & {
  static auto gen = std::mt19937(std::random_device{}());
  return gen;
}

void Student::generate_string(char *member, size_t size) {
  auto gen = get_generator();

  while (size > 0) {
    auto rand = gen();

    while (rand > CHARS_SIZE && size > 0) {

      *member = CHARS[rand % CHARS.size()];
      ++member;

      --size;
      rand /= CHARS.size();
    }
  }
}
