#include "FixedRecord.hpp"

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

  students.add(Alumno{"1", "jhon", "doe", "carr", 3, 3.5});
  students.add(Alumno{"2", "jane", "doe", "carr", 3, 3.5});

  auto removed = students.remove(1);

  std::cout << "Removed: " << removed << '\n';

  auto students_vec = students.load();

  for (const auto &student : students_vec) {
    student.showData();
  }

  return 0;
}
