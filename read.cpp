#include <array>
#include <cstddef>
#include <fstream>
#include <iostream>

constexpr std::size_t MAX_NAME_LEN = 12;
constexpr std::size_t MAX_SURNAME_LEN = 12;
constexpr std::string_view FILE_NAME = "students.dat";

struct Student {
  std::array<char, MAX_NAME_LEN> m_name;
  std::array<char, MAX_SURNAME_LEN> m_surname;
};

class StudentFile {
public:
  // Enables storing of EOF
  using pos_type = int;

  static constexpr pos_type RECORD_SIZE = MAX_NAME_LEN + MAX_SURNAME_LEN + 1;

  explicit StudentFile(const std::string_view &file_name) {

    // If file doesn't exists create it
    // TODO(enrique):

    m_file_stream.open(file_name.data(), std::ios::in | std::ios::out);
  }
  explicit StudentFile(const char *file_name) : m_file_stream(file_name) {}

  void operator<<(const Student &record) {
    m_file_stream.write(record.m_name.data(), MAX_NAME_LEN);
    m_file_stream.write(record.m_surname.data(), MAX_SURNAME_LEN);
    m_file_stream << std::endl;
  }
  [[nodiscard]] bool removeEntry(const std::string_view &name) {

    m_file_stream.seekg(0);

    // Search for name
    std::string line;
    int line_number = 0;

    while (std::getline(m_file_stream, line)) {
      std::cout << "Reading line \n";
      if (compareKey(line, name)) {
        if (deleted == -1) {
          deleted = line_number;
        } else {
          m_file_stream.seekp(static_cast<int>(deleted * RECORD_SIZE));
          m_file_stream << line;
          deleted = line_number;
        }

        return true;
      }
      line_number++;
    }
    line_number = -1;
    return false;
  }

  ~StudentFile() { m_file_stream.close(); }

private:
  std::fstream m_file_stream;
  pos_type deleted = -1;

  static bool compareKey(const std::string_view &line,
                         const std::string_view &key) {
    std::cout << "Line: " << line << std::endl;
    std::cout << "Key: " << key << std::endl;

    return line.substr(0, MAX_NAME_LEN) == key;
    // TODO(nobody):
    // This can be optimized by not constructin the substring
  }
};

int main() {

  StudentFile students{FILE_NAME};
  students << Student{{"John"}, {"Doe"}};
  students << Student{{"Jane"}, {"Doe"}};

  // if (students.removeEntry("John")) {
  //   std::cout << "Entry removed" << std::endl;
  // }

  return 0;
}
