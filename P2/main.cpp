#include "VariableRecord.hpp"

[[maybe_unused]] void test_add() {
  VariableRecord var_record("data.txt");
  // NOLINTBEGIN(readability-magic-numbers)
  var_record.add(Student("Renato", "Cernades", "CS", 12000));
  var_record.add(Student("Juan Diego", "Castro", "CS", 8632));
  var_record.add(Student("Chachi", "Jose", "CS", 7769));
  // var_record.add(Student("Joaquín", "Jordan", "CS", 98754));
  // NOLINTEND(readability-magic-numbers)
}

[[maybe_unused]] void test_load() {
  VariableRecord var_record("data.txt");
  std::vector<Student> alumnos = var_record.load();

  for (const auto &alumn : alumnos) {
    std::cout << alumn << std::endl;
  }
}

[[maybe_unused]] void test_read() {
  VariableRecord var_record("data.txt");
  auto student_1 = var_record.read_record(1);
  auto student_2 = var_record.read_record(2);
  auto student_3 = var_record.read_record(3);
  auto student_4 = var_record.read_record(4);

  std::cout << student_1 << std::endl;
  std::cout << student_2 << std::endl;
  std::cout << student_3 << std::endl;
  std::cout << student_4 << std::endl;
}

int main() {

  test_add();

  return 0;
}
