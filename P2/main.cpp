#include "VariableRecord.hpp"

[[maybe_unused]] void test_add() {
  VariableRecord var_record("data.txt");
  // NOLINTBEGIN(readability-magic-numbers)
  var_record.add(Student("Renato", "Cernades", "CS", 12000));
  var_record.add(Student("Juan Diego", "Castro", "CS", 8632));
  var_record.add(Student("Chachi", "Jose", "CS", 7769));
  var_record.add(Student("Joaquín", "Jordan", "CS", 98754));
  // NOLINTEND(readability-magic-numbers)
}

[[maybe_unused]] void test_load() {
  VariableRecord var_record("data.txt");
  std::vector<Student> loaded_alumnos = var_record.load();

  std::vector<Student> test_alumnos;

  test_alumnos.emplace_back("Renato", "Cernades", "CS", 12000);
  test_alumnos.emplace_back("Juan Diego", "Castro", "CS", 8632);
  test_alumnos.emplace_back("Chachi", "Jose", "CS", 7769);
  test_alumnos.emplace_back("Joaquín", "Jordan", "CS", 98754);

  if (loaded_alumnos == test_alumnos) {
    std::cout << "Load test passed" << std::endl;
  } else {
    std::cout << "Load test failed" << std::endl;
  }
}

[[maybe_unused]] void test_read() {
  VariableRecord var_record("data.txt");

  std::vector<Student> read_alumnos;

  auto al1 = var_record.read_record(1);
  auto al2 = var_record.read_record(2);
  auto al3 = var_record.read_record(3);
  auto al4 = var_record.read_record(4);

  read_alumnos.push_back(al1);
  read_alumnos.push_back(al2);
  read_alumnos.push_back(al3);
  read_alumnos.push_back(al4);

  std::vector<Student> test_alumnos;

  test_alumnos.emplace_back("Renato", "Cernades", "CS", 12000);
  test_alumnos.emplace_back("Juan Diego", "Castro", "CS", 8632);
  test_alumnos.emplace_back("Chachi", "Jose", "CS", 7769);
  test_alumnos.emplace_back("Joaquín", "Jordan", "CS", 98754);

  if (read_alumnos == test_alumnos) {
    std::cout << "Read test passed" << std::endl;
  } else {
    std::cout << "Read test failed" << std::endl;
  }
}

int main() {

  // test_add();
  test_load();
  test_read();

  std::cout << "Finished\n";

  return 0;
}
