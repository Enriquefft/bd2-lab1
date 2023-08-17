#ifndef FIXEDRECORD_HPP
#define FIXEDRECORD_HPP

#include <cstddef>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

using std::vector;

constexpr std::size_t MAX_CODIGO_LEN = 5;
constexpr std::size_t MAX_NOMBRE_LEN = 11;
constexpr std::size_t MAX_APELLIDO_LEN = 20;
constexpr std::size_t MAX_CARRERA_LEN = 15;

struct Alumno {
  // NOLINTBEGIN
  char codigo[MAX_CODIGO_LEN];
  char nombre[MAX_NOMBRE_LEN];
    char apellidos[MAX_APELLIDO_LEN];
  char carrera[MAX_CARRERA_LEN];
    int ciclo;
  float mensualidad;
  // NOLINTEND
};

class FixedRecord {
public:
  // Enables storing of EOF
  using pos_type = int;

  static constexpr pos_type RECORD_SIZE = sizeof(Alumno);

  explicit FixedRecord(const std::string_view &file_name);
  vector<Alumno> load();
  void add(const Alumno &record);
  Alumno &readRecord(pos_type pos);
  Alumno readRecord(pos_type pos) const;

  [[nodiscard]] bool remove(pos_type pos);

  ~FixedRecord() { m_file_stream.close(); }

private:
  std::fstream m_file_stream;
  std::string m_file_name;
  pos_type m_deleted = -1;
};

inline FixedRecord::FixedRecord(const std::string_view &file_name)
    : m_file_name(file_name) {
  m_file_stream.open(file_name.data(), std::ios::in | std::ios::out);

  // If file doesn't exists create it
  if (std::filesystem::exists(file_name.data()) &&
      std::filesystem::is_regular_file(file_name.data())) {
  } else {
    m_file_stream.close();
    m_file_stream.open(file_name.data(), std::ios::out);
  }

  m_file_stream.close();
}

inline void FixedRecord::add(const Alumno &record) {
  m_file_stream.open(m_file_name.data(), std::ios::app | std::ios::binary);
  // TODO: write record to file
  m_file_stream << std::endl;
  m_file_stream.close();
}

inline bool FixedRecord::remove(pos_type pos) {

  // TODO: check if pos is valid

  m_file_stream.open(m_file_name.data(),
                     std::ios::in | std::ios::out | std::ios::binary);

  m_file_stream.seekg(0, std::ios::end); // ubicar cursor al final del archivo
  long total_bytes = m_file_stream.tellg(); // cantidad de bytes del archivo
  if (total_bytes < (static_cast<long>(pos) * RECORD_SIZE)) {
    return false;
  }

  m_file_stream.seekp(static_cast<int>(pos * RECORD_SIZE));
  m_file_stream.write(reinterpret_cast<const char *>(&pos), sizeof(int));

  if (m_deleted == -1) {
    m_deleted = pos;
  } else {
    m_file_stream.seekp(static_cast<int>(m_deleted * RECORD_SIZE));
    m_file_stream.write(reinterpret_cast<const char *>(&pos), sizeof(int));
  }
  m_file_stream.close();

  return true;
}

#endif // FIXEDRECORD_HPP
