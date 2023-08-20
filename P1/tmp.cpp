#include <iostream>
enum class E { A = true, B = false, C = true };

int main(int argc, char *argv[]) {

  E e = E::A;

  std::cout << "sizeof(E): " << sizeof(E) << std::endl;
  std::cout << "sizeof(e): " << sizeof(e) << std::endl;

  return 0;
}
