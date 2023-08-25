#ifndef FIXEDRECORD_HPP
#define FIXEDRECORD_HPP

#include <cstddef>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>

#include "Student.hpp"

constexpr std::string_view METADATA_FILE = "metadata.txt";
constexpr int DEFAULT_DELETED = -1;

enum class RecordStatus : bool { DELETED = false, OK = true };

struct Record {
  Student student;
  RecordStatus status = RecordStatus::OK;

  Record() = default;
  explicit Record(const Student &_student) : student(_student) {}
  explicit Record(RecordStatus _status) : status(_status) {}
};

class FixedRecord {
public:
  // Enables storing of EOF
  using pos_type = int;

  static constexpr pos_type RECORD_SIZE = sizeof(Record);
  static constexpr pos_type DELETED_FLAG_POS = RECORD_SIZE - sizeof(bool);

  FixedRecord(const FixedRecord &) = delete;
  FixedRecord(FixedRecord &&) = delete;
  FixedRecord &operator=(const FixedRecord &) = delete;
  FixedRecord &operator=(FixedRecord &&) = delete;

  explicit FixedRecord(const std::string_view &file_name);
  auto load() -> std::vector<Student>;
  void add(const Student &student);
  auto read_record(pos_type pos) -> Student &;
  auto read_record(pos_type pos) const -> Student;

  [[nodiscard]] auto remove(pos_type pos) -> bool;

  ~FixedRecord() { m_file_stream.close(); }

private:
  std::fstream m_file_stream;
  std::string m_file_name;
  pos_type m_first_deleted{};

  void update_first_deleted(pos_type pos);
  auto read_metadata() -> bool;
  void open_or_create(const char *file_name);
  static void generate_deleted_record(char *deleted, int pos);
  auto is_deleted(pos_type pos) const -> bool;
};

FixedRecord::FixedRecord(const std::string_view &file_name)
    : m_file_name(file_name) {

  open_or_create(file_name.data()); // Can throw

  // Read or create metadata
  if (!read_metadata()) {
    update_first_deleted(DEFAULT_DELETED);
  }
}

void FixedRecord::open_or_create(const char *file_name) {

  // If file doesn't exists create it
  if (!std::filesystem::exists(file_name) ||
      !std::filesystem::is_regular_file(file_name)) {
    std::clog << "Creating file\n";
    m_file_stream.open(file_name, std::ios::out);
    m_file_stream.close();
  }

  // Try to open for read and write in binary mode
  m_file_stream.open(file_name,
                     std::ios::in | std::ios::out | std::ios::binary);
  if (!m_file_stream.is_open()) {
    throw std::runtime_error("Couldn't open file");
  }
  m_file_stream.close();
}

auto FixedRecord::read_metadata() -> bool {

  try {
    open_or_create(METADATA_FILE.data());
  } catch (std::runtime_error &err) {
    std::cerr << err.what();
    return false;
  }

  std::ifstream file(
      METADATA_FILE.data(),
      std::ios::binary |
          std::ios::ate); // Already has been checked that it exists

  // File must contain a single integer
  std::streamsize size = file.tellg();
  std::cout << "metadata size: " << size << '\n';
  if (size != sizeof(int)) {
    std::cerr << "File does not contain a single integer";
    file.close();
    return false;
  }

  file.seekg(0, std::ios::beg);

  int value = 0;
  // The file must be readable
  if (!file.read(reinterpret_cast<char *>(&value), sizeof(int))) {
    std::cerr << "Error reading file";
    return false;
  }

  m_first_deleted = value;
  return true;
}

void FixedRecord::update_first_deleted(pos_type pos) {
  m_first_deleted = pos;
  std::ofstream metadata(METADATA_FILE.data(), std::ios::binary);
  metadata.write(reinterpret_cast<const char *>(&pos), sizeof(int));
  metadata.close();
}

inline void FixedRecord::add(const Student &student) {

  m_file_stream.open(m_file_name.data(),
                     std::ios::in | std::ios::app | std::ios::binary);

  if (!m_file_stream.is_open()) {
    throw std::runtime_error("Couldn't open file");
  }

  if (m_first_deleted != -1) {
    m_file_stream.seekg(static_cast<int>(m_first_deleted * RECORD_SIZE));
    int next_deleted = 0;
    m_file_stream.read(reinterpret_cast<char *>(&next_deleted), sizeof(int));
    update_first_deleted(next_deleted);
  }

  Record record_to_write{student};

  m_file_stream.write(reinterpret_cast<const char *>(&record_to_write),
                      RECORD_SIZE);
  m_file_stream.close();
}

inline auto FixedRecord::load() -> std::vector<Student> {

  std::vector<Student> alumnos;

  m_file_stream.open(m_file_name.data(),
                     std::ios::in | std::ios::binary); // Open for reading

  if (!m_file_stream.is_open()) {
    throw std::runtime_error("Couldn't open file");
  }

  Record record_buffer; // Temporary buffer

  while (m_file_stream.peek() != EOF) {

    if (!m_file_stream.read(reinterpret_cast<char *>(&record_buffer),
                            RECORD_SIZE)) {
      throw std::runtime_error("Couldn't read record");
    }

    if (record_buffer.status == RecordStatus::OK) {
      alumnos.push_back(record_buffer.student);
    }
  }
  m_file_stream.close();

  return alumnos;
}

inline auto FixedRecord::remove(pos_type pos) -> bool {

  m_file_stream.open(m_file_name.data(),
                     std::ios::in | std::ios::out | std::ios::binary);

  // Check if pos is valid
  m_file_stream.seekg(0, std::ios::end); // ubicar cursor al final del archivo
  long total_bytes = m_file_stream.tellg(); // cantidad de bytes del archivo
  if (total_bytes < (static_cast<long>(pos) * RECORD_SIZE)) {
    return false;
  }

  // Seek to the position
  m_file_stream.seekp(static_cast<int>(pos * RECORD_SIZE));

  // Generate the deleted record
  char deleted[RECORD_SIZE];
  generate_deleted_record(static_cast<char *>(deleted), DEFAULT_DELETED);

  // Write the buffer to the file
  m_file_stream.write(reinterpret_cast<char *>(&deleted), RECORD_SIZE);

  if (m_first_deleted == -1) {
    update_first_deleted(pos);
  } else {

    m_file_stream.seekp(static_cast<int>(m_first_deleted * RECORD_SIZE));

    char previous_deleted[RECORD_SIZE];
    generate_deleted_record(static_cast<char *>(previous_deleted), pos);

    m_file_stream.write(reinterpret_cast<char *>(&previous_deleted),
                        RECORD_SIZE);
  }
  m_file_stream.close();

  return true;
}

/// Fill the buffer with the integer value
void FixedRecord::generate_deleted_record(char deleted[RECORD_SIZE], int pos) {

  Record deleted_record(RecordStatus::DELETED);
  deleted = reinterpret_cast<char *>(&deleted_record);

  // write pos to the buffer
  std::memcpy(deleted, &pos, sizeof(int));
}

auto FixedRecord::is_deleted(pos_type pos) const -> bool {
  if (pos == m_first_deleted) {
    return true;
  }

  std::ifstream file(m_file_name, std::ios::binary);

  file.seekg(static_cast<int>((pos * RECORD_SIZE) + DELETED_FLAG_POS));
  bool is_deleted = false;

  if (!file.read(reinterpret_cast<char *>(&is_deleted), sizeof(bool))) {
    std::cerr << "Error reading file";
    file.close();
    return false;
  }

  file.close();
  return is_deleted;
}

#endif // FIXEDRECORD_HPP
