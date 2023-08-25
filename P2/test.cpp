#include "Student.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>

constexpr char M_DELIMITER = '|';
constexpr std::string_view METADATA_FILE = "test_metadata.txt";

class VariableRecord {

public:
  using pos_type = int64_t;
  using value_type = Student;

  explicit VariableRecord(std::string file_name);
  void add(const value_type &record);
  void open_or_create(const char *file_name);
  auto read_metadata() -> bool;
  void update_record_count(pos_type pos);

private:
  std::string m_filename;
  std::fstream m_file_stream;
  pos_type m_record_count{};

  // Disable padding
#pragma pack(push, 1)
  struct MetadataRecord {
    pos_type pos;
    pos_type size;
    friend auto operator<<(std::fstream &stream, const MetadataRecord &record)
        -> std::fstream & {
      stream << record.pos << ' ' << record.size << '\n';
      return stream;
    }
    friend auto operator<<(std::ostream &stream, const MetadataRecord &record)
        -> std::ostream & {
      stream << static_cast<int>(record.pos) << ' '
             << static_cast<int>(record.size);
      return stream;
    }
    [[nodiscard]] static auto size_in_bytes() -> size_t {
      return 2 * sizeof(pos_type) + 2;
    }
  };
#pragma pack(pop)
};

VariableRecord::VariableRecord(std::string file_name)
    : m_filename(std::move(file_name)) {

  std::ofstream metadata(METADATA_FILE.data(), std::ios::out);
  metadata << 0 << '\n';
}

void VariableRecord::update_record_count(pos_type pos) {

  m_record_count = pos;

  std::ofstream metadata(METADATA_FILE.data(), std::ios::out);
  metadata << pos << '\n';
  // metadata.flush();
}

void VariableRecord::add(const value_type &record) {

  // Update metadata
  // ERROR HERE
  // update_record_count(m_record_count +
  //                     1); // Posible ios optimization by not opening twice

  // Insert new element pointer to metadata
  std::ofstream metadata;
  metadata.open(METADATA_FILE.data(), std::ios::app);
  // curr pos in metadata
  // Unexpecte value here
  std::cout << "Metadata pos: " << metadata.tellp() << std::endl;

  MetadataRecord new_record{rand(), static_cast<pos_type>(rand())};
  // metadata.write(reinterpret_cast<const char *>(&new_record),
  //                sizeof(new_record));
  std::cout << "New record: " << new_record << std::endl;

  metadata << new_record;

  std::cout << "New pos: " << metadata.tellp() << std::endl;
  metadata.close();
}
int main(int argc, char *argv[]) {

  VariableRecord variable_record("test.txt");
  variable_record.add(Student("Juan", "Diego", "a", 3.5));
  variable_record.add(Student("carlos", "Diego", "b", 3.5));

  return 0;
}
