#include <cstddef>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

using std::vector;

constexpr std::string_view FILE_NAME = "students.dat";

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

  static constexpr pos_type RECORD_SIZE = MAX_CODIGO_LEN + MAX_NOMBRE_LEN +
                                          MAX_APELLIDO_LEN + MAX_CARRERA_LEN +
                                          sizeof(int) + sizeof(float) + 1;

  explicit FixedRecord(const std::string_view &file_name);
  vector<Alumno> load();
  void add(const Alumno &record);
  Alumno &readRecord(pos_type pos);
  Alumno readRecord(pos_type pos) const;

  [[nodiscard]] bool remove(pos_type pos);

  void operator<<(const Alumno &record) {
    m_file_stream.open(m_file_name.data(), std::ios::app | std::ios::binary);
    // TODO: write record to file
    m_file_stream << std::endl;
    m_file_stream.close();
  }

  ~FixedRecord() { m_file_stream.close(); }

private:
  std::fstream m_file_stream;
  std::string m_file_name;
  pos_type deleted = -1;
};

FixedRecord::FixedRecord(const std::string_view &file_name)
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

bool FixedRecord::remove(pos_type pos) {
  m_file_stream.open(m_file_name.data(),
                     std::ios::in | std::ios::out | std::ios::binary);

  m_file_stream.seekg(0);

  // Search for name
  std::string line;
  int line_number = 0;

  //
  while (std::getline(m_file_stream, line)) {
    std::cout << "Reading line \n";
    if (compareKey(line, name)) {
      if (deleted == -1) {
        deleted = line_number;
      } else {
        // TODO: This might not work, test it
        m_file_stream.seekp(static_cast<int>(deleted * RECORD_SIZE));
        m_file_stream << line;
        deleted = line_number;
      }

      m_file_stream.close();
      return true;
    }
    line_number++;
  }
  line_number = -1;
  m_file_stream.close();
  return false;
}
