#include <array>
#include <cstddef>
#include <filesystem>
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

  explicit StudentFile(const std::string_view &file_name)
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

  void operator<<(const Student &record) {
    m_file_stream.open(m_file_name.data(), std::ios::app | std::ios::binary);
    m_file_stream.write(record.m_name.data(), MAX_NAME_LEN);
    m_file_stream.write(record.m_surname.data(), MAX_SURNAME_LEN);
    m_file_stream << std::endl;
    m_file_stream.close();
  }
  [[nodiscard]] bool removeEntry(const std::string_view &name) {
    m_file_stream.open(m_file_name.data(),
                       std::ios::in | std::ios::out | std::ios::binary);

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

  ~StudentFile() { m_file_stream.close(); }

private:
  std::fstream m_file_stream;
  std::string m_file_name;
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

  if (students.removeEntry("John")) {
    std::cout << "Entry removed" << std::endl;
  }

  return 0;
}
