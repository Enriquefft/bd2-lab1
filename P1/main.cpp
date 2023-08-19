#include "FixedRecord.hpp"
#include <random>

// struct Alumno {
//   char codigo[MAX_CODIGO_LEN];
//   char nombre[MAX_NOMBRE_LEN];
//   char apellidos[MAX_APELLIDO_LEN];
//   char carrera[MAX_CARRERA_LEN];
//   int ciclo;
//   float mensualidad;
// };

int main() {

  constexpr std::string_view FILE_NAME = "students.dat";

  FixedRecord students{FILE_NAME};

  students.add(Alumno{"1", "jhon", "doe1", "carr1", 3, 3.5});
  students.add(Alumno{"2", "jane", "doe2", "carr2", 4, 4.5});

  auto removed = students.remove(1);
  auto removed2 = students.remove(0);

  std::cout << "Removed jhane: " << removed << '\n';
  std::cout << "Removed jhon: " << removed2 << '\n';

  auto students_vec = students.load();

  for (const auto &student : students_vec) {
    student.showData();
  }

  return 0;
}
