//
// Created by renat on 8/19/2023.
//
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

struct Alumno {
  std::string Nombre;
  std::string Apellidos;
  std::string Carrera;
  float Mensualidad{};

public:
  Alumno() = default;

  Alumno(std::string nombre, std::string apellidos, std::string carrera,
         float mensualidad)
      : Nombre(std::move(nombre)), Apellidos(std::move(apellidos)),
        Carrera(std::move(carrera)), Mensualidad(mensualidad) {}

  friend std::ostream &operator<<(std::ostream &stream, const Alumno &alumno) {
    stream << alumno.Nombre << " " << alumno.Apellidos << " " << alumno.Carrera
           << " " << alumno.Mensualidad;
    return stream;
  }
};

/*
 * VariableRecord class
 * Only available for txt files.
 * Support MAX_INT number of records.
 * */
class VariableRecord {
private:
  std::string m_filename; // name of the .txt file
  char m_delimiter = '|';

  std::string pack(Alumno alumno) const {
    return alumno.Nombre + m_delimiter + alumno.Apellidos + m_delimiter +
           alumno.Carrera + m_delimiter + std::to_string(alumno.Mensualidad) +
           "\n";
  }

  Alumno unpack(std::stringstream &record) const {
    Alumno a;
    std::getline(record, a.Nombre, m_delimiter);
    std::getline(record, a.Apellidos, m_delimiter);
    std::getline(record, a.Carrera, m_delimiter);

    std::string mensualidad;
    std::getline(record, mensualidad, '\n');
    try {
      a.Mensualidad = std::stof(mensualidad);
    } catch (std::invalid_argument &e) {
      throw e;
    }

    return a;
  }

public:
  explicit VariableRecord(const std::string &fn) { m_filename = fn + ".txt"; }

  void add(const Alumno &record) {
    std::string pack_record = pack(record);
    std::ofstream file(m_filename,
                       std::ios::app); // Envia el puntero al final del archivo
    if (!file.is_open())
      throw("ERROR: No se pudo abrir el archivo");
    file.write(pack_record.c_str(), pack_record.size());
    file.close();
  }

  std::vector<Alumno> load() {
    std::fstream file(m_filename);
    if (!file.is_open()) {
      throw std::runtime_error("ERROR: No se pudo abrir el archivo");
    }

    std::vector<Alumno> vec;

    std::string buff;
    while (getline(file, buff)) {
      std::stringstream ss(buff);
      vec.push_back(unpack(ss));
    }

    return vec;
  }

  Alumno readRecord(int pos = 0) {
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

    Alumno alumno = unpack(buff);

    file.close();

    return alumno;
  }
};

int main() {

  // Testing del constructor()
  VariableRecord vr("data");

  // Testing de la función add()
  vr.add(Alumno("Renato", "Cernades", "CS", 12000));
  vr.add(Alumno("Juan Diego", "Castro", "CS", 8632));
  vr.add(Alumno("Chachi", "Jose", "CS", 7769));
  vr.add(Alumno("Joaquín", "Jordan", "CS", 98754));

  // Testing de la función readRecord()
  std::cout << vr.readRecord(1) << std::endl;

  // Testing de la función load()
  std::vector<Alumno> alumnos = vr.load();
  for (auto a : alumnos) {
    std::cout << a << std::endl;
  }

  return 0;
}
