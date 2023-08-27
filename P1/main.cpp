#include "FixedRecord.hpp"
#include <iostream>
#include <random>

namespace {
[[maybe_unused]] void student_test() {
  Student student1("a", "b");
  Student student2("b", "c");
  Student student3("c", "d");
  Student student4("d", "e");

  std::cout << student1 << '\n';
  std::cout << student2 << '\n';
  std::cout << student3 << '\n';
  std::cout << student4 << '\n';
}

[[maybe_unused]] void writing_test() {

  FixedRecord db("students.db");

  db.add(Student{"Code102", "Renato", "Cernades", "CS", 4, 94.1});
  db.add(Student{"CE@#@", "Hola", "Avasca", "CS", 4, 1233.2});
  db.add(Student{"CE**(D", "Haie", "Cerqnas", "CS", 1, 421.2});
}

[[maybe_unused]] void read_test() {
  FixedRecord db("students.db");

  for (const auto &student : db.load()) {
    std::cout << student << '\n';
  }
}

[[maybe_unused]] void deletion_test() {
  FixedRecord db("students.db");
  bool rm = db.remove(1);
  std::cout << "Remove: " << rm << '\n';
}

} // namespace

int main() {

  deletion_test();
  writing_test();
  read_test();

  return 0;
}
