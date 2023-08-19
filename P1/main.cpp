// #include "FixedRecord.hpp"
#include "Student.hpp"
#include <iostream>
#include <random>

// struct Student {
//   char codigo[MAX_CODIGO_LEN];
//   char nombre[MAX_NOMBRE_LEN];
//   char apellidos[MAX_APELLIDO_LEN];
//   char carrera[MAX_CARRERA_LEN];
//   int ciclo;
//   float mensualidad;
// };

namespace {
[[maybe_unused]] void student_test() { Student student("a", "b"); }

} // namespace

auto main() -> int {

  int x = 54;

  student_test();
  std::cout << "Hello World\n";

  return 0;
}
