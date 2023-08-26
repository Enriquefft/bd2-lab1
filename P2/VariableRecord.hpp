#ifndef VARIABLE_RECORD_HPP
#define VARIABLE_RECORD_HPP

#include "Student.hpp"
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

constexpr char M_DELIMITER = '|';
constexpr std::string_view METADATA_FILE = "metadata.txt";

/*
 * VariableRecord class
 * Only available for txt files.
 * Support MAX_INT number of records.
 * */
class VariableRecord {

public:
  using pos_type = int64_t;
  using value_type = Student;

  explicit VariableRecord(std::string file_name);
  void add(const value_type &record);
  [[nodiscard]] auto load() -> std::vector<value_type>;
  [[nodiscard]] auto read_record(pos_type pos) -> value_type;

private:
  std::string m_filename;
  std::fstream m_file_stream;
  pos_type m_record_count{};

  [[nodiscard]] static auto pack(const value_type &alumno) -> std::string;
  static auto unpack(std::stringstream &record) -> value_type;
  void open_or_create(const char *file_name);
  auto read_metadata() -> bool;
  void update_record_count(pos_type pos);

  // Disable padding
#pragma pack(push, 1)
  struct MetadataRecord {
    pos_type pos;
    pos_type size;
    friend auto operator<<(std::ofstream &stream, const MetadataRecord &record)
        -> std::ofstream & {
      // stream << record.pos << ' ' << record.size << '\n';

      stream.write(reinterpret_cast<const char *>(&record.pos),
                   sizeof(record.pos));
      stream << ' ';
      stream.write(reinterpret_cast<const char *>(&record.size),
                   sizeof(record.size));
      stream << '\n';

      return stream;
    }
    // friend auto operator<<(std::ostream &stream, const MetadataRecord
    // &record)
    //     -> std::ostream & {
    //   stream << static_cast<int>(record.pos) << ' '
    //          << static_cast<int>(record.size);
    //   return stream;
    // }

    friend auto operator>>(std::ifstream &stream, MetadataRecord &record)
        -> std::ifstream & {

      stream.read(reinterpret_cast<char *>(&record.pos), sizeof(record.pos));
      stream.seekg(1, std::ios::cur);
      stream.read(reinterpret_cast<char *>(&record.size), sizeof(record.size));
      stream.seekg(1, std::ios::cur);

      return stream;
    }

    constexpr static int64_t METADATA_RECORD_SIZE = 2 * sizeof(pos_type) + 2;
  };
#pragma pack(pop)
};

inline VariableRecord::VariableRecord(std::string file_name)
    : m_filename(std::move(file_name)) {

  if (m_filename.substr(m_filename.size() - 4) != ".txt") {
    throw std::invalid_argument("ERROR: El archivo debe ser .txt");
  }

  open_or_create(m_filename.data()); // Can throw

  // Read or create metadata
  //
  if (!read_metadata()) {
    std::cerr << "ERROR: No se pudo leer la metadata, creando archivo\n";
    update_record_count(0);
  }
}

inline void VariableRecord::add(const value_type &record) {
  std::string pack_record = pack(record);

  m_file_stream.open(m_filename,
                     std::ios::app); // Envia el puntero al final del archivo

  const auto NEW_POS = m_file_stream.tellg();

  if (!m_file_stream.is_open()) {
    throw std::runtime_error("ERROR: No se pudo abrir el archivo");
  }

  m_file_stream.write(pack_record.c_str(),
                      static_cast<int64_t>(pack_record.size()));
  m_file_stream.close();

  if (m_file_stream.fail()) {
    throw std::runtime_error("ERROR: No se pudo escribir el registro");
  }

  std::ofstream metadata;

  // Update record count
  update_record_count(m_record_count + 1);

  metadata.open(METADATA_FILE.data(), std::ios::in | std::ios::app);
  metadata.seekp(0, std::ios::end);
  // Insert new element pointer to metadata
  // curr pos in metadata

  MetadataRecord new_record{NEW_POS, static_cast<pos_type>(pack_record.size())};
  // metadata.write(reinterpret_cast<const char *>(&new_record),
  //                sizeof(new_record));

  metadata << new_record;

  metadata.close();
}

inline auto VariableRecord::load() -> std::vector<value_type> {
  std::ifstream metadata(METADATA_FILE.data());
  if (!metadata.is_open()) {
    throw std::runtime_error("ERROR: No se pudo abrir la metadata");
  }

  m_file_stream.open(m_filename, std::ios::in);
  if (!m_file_stream.is_open()) {
    throw std::runtime_error("Error: No se pudo abrir la data.");
  }

  pos_type record_count = 0;
  if (!(metadata.read(reinterpret_cast<char *>(&record_count),
                      sizeof(record_count)))) {
    std::cerr << "ERROR: No se pudo leer la cantidad de registros\n";
  }

  std::vector<value_type> vec;
  std::stringstream buff_stream;

  MetadataRecord record{};

  // Find eof
  metadata.seekg(0, std::ios::end);
  auto eof = metadata.tellg();

  // Go to start of the record pointers.
  metadata.seekg(sizeof(m_record_count) + 1, std::ios::beg);

  while (metadata.tellg() < eof) {

    if (!(metadata >> record)) {
      throw std::runtime_error("Error: No se pudo leer la metadata");
    }
    if (metadata.fail() || metadata.bad()) {
      throw std::runtime_error("Error: Hubo un error con el archivo");
    }

    char tmp_buffer[record.size];
    m_file_stream.read(tmp_buffer, record.size);

    buff_stream.write(tmp_buffer, record.size);
    vec.push_back(unpack(buff_stream));
  }
  metadata.close();
  m_file_stream.close();

  return vec;
}

inline auto VariableRecord::read_record(pos_type pos) -> value_type {

  // Open necesary files

  std::ifstream metadata(METADATA_FILE.data());
  if (!metadata.is_open()) {
    throw std::runtime_error("ERROR: No se pudo abrir la metadata");
  }

  pos_type record_count = 0;
  if (!(metadata.read(reinterpret_cast<char *>(&record_count),
                      sizeof(record_count)))) {
    std::cerr << "ERROR: No se pudo leer la cantidad de registros\n";
  }

  if (pos < 0) {
    throw std::invalid_argument("ERROR: posicion invalida");
  }

  if (pos > record_count) {
    throw std::invalid_argument("Error: posicion inválida");
  }

  MetadataRecord record{};

  const pos_type metdata_pos =
      sizeof(record_count) + 1 +
      ((pos - 1) * MetadataRecord::METADATA_RECORD_SIZE);

  // Find position from metadata
  metadata.seekg(metdata_pos, std::ios::beg);

  metadata >> record;

  metadata.close();

  // Read student
  Student student;

  m_file_stream.open(m_filename, std::ios::in);
  if (!m_file_stream.is_open()) {
    throw std::runtime_error("Error: No se pudo abrir la data.");
  }

  m_file_stream.seekg(record.pos);

  char tmp_buffer[record.size];
  m_file_stream.read(tmp_buffer, record.size);

  std::stringstream buff_stream;
  buff_stream.write(tmp_buffer, record.size);
  student = unpack(buff_stream);

  m_file_stream.close();

  return student;
};

inline void VariableRecord::open_or_create(const char *file_name) {

  // If file doesn't exists create it
  if (!std::filesystem::exists(file_name) ||
      !std::filesystem::is_regular_file(file_name)) {
    std::clog << "Creating file\n";
    m_file_stream.open(file_name, std::ios::app);

    if (!m_file_stream.is_open()) {
      throw std::runtime_error("Couldn't create file");
    }

    m_file_stream.close();
  }

  // Try to open for read and write in binary mode
  m_file_stream.open(file_name,
                     std::ios::in | std::ios::app /* | std::ios::binary */);
  if (!m_file_stream.is_open()) {
    throw std::runtime_error("Couldn't open file");
  }
  m_file_stream.close();
}

inline auto VariableRecord::read_metadata() -> bool {

  open_or_create(METADATA_FILE.data());

  std::ifstream metadata(
      METADATA_FILE.data(),
      std::ios::in); // Already has been checked that it exists

  // The record count must be readable
  if (!(metadata.read(reinterpret_cast<char *>(&m_record_count),
                      sizeof(m_record_count)))) {
    std::cerr << "ERROR: No se pudo leer la cantidad de registros\n";
    return false;
  }

  // Validate that the file is not corrupted

  MetadataRecord record{};

  // Find eof
  metadata.seekg(0, std::ios::end);
  auto eof = metadata.tellg();

  // Go to start of the record pointers.
  metadata.seekg(sizeof(m_record_count) + 1, std::ios::beg);

  while (metadata.tellg() < eof) {

    if (!(metadata >> record)) {

      throw std::runtime_error("Error: No se pudo leer la metadata");
    }
    if (metadata.fail() || metadata.bad()) {
      throw std::runtime_error("Error: Hubo un error con el archivo");
    }
  }
  metadata.close();

  return true;
}

inline void VariableRecord::update_record_count(pos_type pos) {

  m_record_count = pos;

  std::ofstream metadata(METADATA_FILE.data(), std::ios::in | std::ios::out);
  metadata.seekp(0, std::ios::beg);

  char tmp_buffer[sizeof(m_record_count)];
  std::memcpy(tmp_buffer, &m_record_count, sizeof(m_record_count));
  metadata.write(tmp_buffer, sizeof(m_record_count));
  metadata << '\n';
}

[[nodiscard]] inline auto VariableRecord::pack(const value_type &alumno)
    -> std::string {
  return alumno.nombres + M_DELIMITER + alumno.apellidos + M_DELIMITER +
         alumno.carrera + M_DELIMITER + std::to_string(alumno.mensualidad) +
         "\n";
}

inline auto VariableRecord::unpack(std::stringstream &record) -> value_type {

  value_type student;
  std::getline(record, student.nombres, M_DELIMITER);
  std::getline(record, student.apellidos, M_DELIMITER);
  std::getline(record, student.carrera, M_DELIMITER);

  std::string mensualidad;
  std::getline(record, mensualidad, '\n');

  // try {
  student.mensualidad = std::stof(mensualidad);
  // } catch (std::invalid_argument &e) {
  //   throw e;
  // }

  return student;
}

#endif // VARIABLE_RECORD_HPP
