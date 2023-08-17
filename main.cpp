#include "FixedRecord.hpp"
int main() {

  FixedRecord students{FILE_NAME};
  students << Alumno{{"John"}, {"Doe"}};
  students << Alumno{{"Jane"}, {"Doe"}};

  if (students.removeEntry("John")) {
    std::cout << "Entry removed" << std::endl;
  }

  return 0;
}
