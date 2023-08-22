#include "Student.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

/*
 * VariableRecord class
 * Only available for txt files.
 * Support MAX_INT number of records.
 * */
class VariableRecord {
private:
  constexpr static char M_DELIMITER = '|';

  std::string m_filename;

  [[nodiscard]] static auto pack(const Student &alumno) -> std::string {
    return alumno.nombres + M_DELIMITER + alumno.apellidos + M_DELIMITER +
           alumno.carrera + M_DELIMITER + std::to_string(alumno.mensualidad) +
           "\n";
  }

  static auto unpack(std::stringstream &record) -> Student {
    Student student;
    std::getline(record, student.nombres, M_DELIMITER);
    std::getline(record, student.apellidos, M_DELIMITER);
    std::getline(record, student.carrera, M_DELIMITER);

    std::string mensualidad;
    std::getline(record, mensualidad, '\n');
    try {
      student.mensualidad = std::stof(mensualidad);
    } catch (std::invalid_argument &e) {
      throw e;
    }

    return student;
  }

public:
  explicit VariableRecord(const std::string &file_name) {
    m_filename = file_name + ".txt";
  }

  void add(const Student &record) {
    std::string pack_record = pack(record);
    std::ofstream file(m_filename,
                       std::ios::app); // Envia el puntero al final del archivo
    if (!file.is_open()) {
      throw std::runtime_error("ERROR: No se pudo abrir el archivo");
    }
    file.write(pack_record.c_str(), pack_record.size());
    file.close();
  }

  auto load() -> std::vector<Student> {
    std::fstream file(m_filename);
    if (!file.is_open()) {
      throw std::runtime_error("ERROR: No se pudo abrir el archivo");
    }

    std::vector<Student> vec;

    std::string buff;
    while (getline(file, buff)) {
      std::stringstream str_stream(buff);
      vec.push_back(unpack(str_stream));
    }

    return vec;
  }

  auto read_record(int pos = 0) -> Student {
    if (pos <= 0) {
      throw std::invalid_argument("ERROR: posicion invalida");
    }

    std::ifstream file(m_filename);
    if (!file.is_open()) {
      throw std::runtime_error("ERROR: No se pudo abrir el archivo");
    }

    std::string s;
    for (int i = 0; i < pos; ++i) {
      std::getline(file, s);

      if (file.fail()) {
        throw(std::invalid_argument("ERROR: posicion invalida"));
      }
    }
    std::stringstream buff(s);

    Student alumno = unpack(buff);

    file.close();

    return alumno;
  }
};

int main() {

  try {

    // Testing del constructor()
    VariableRecord var_record("data");

    // Testing de la función add()
    // NOLINTBEGIN(readability-magic-numbers)
    var_record.add(Student("Renato", "Cernades", "CS", 12000));
    var_record.add(Student("Juan Diego", "Castro", "CS", 8632));
    var_record.add(Student("Chachi", "Jose", "CS", 7769));
    var_record.add(Student("Joaquín", "Jordan", "CS", 98754));
    // NOLINTEND(readability-magic-numbers)

    // Testing de la función readRecord()
    std::cout << var_record.read_record(1) << std::endl;

    // Testing de la función load()
    std::vector<Student> alumnos = var_record.load();
    for (const auto &alumn : alumnos) {
      std::cout << alumn << std::endl;
    }

  } catch (std::exception &e) {
    std::cout << e.what() << std::endl;
  }

  return 0;
}
