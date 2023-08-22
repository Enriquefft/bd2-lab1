#ifndef STUDENT_HPP
#define STUDENT_HPP

#include <cstring>
#include <format>
#include <limits>
#include <random>
#include <utility>

struct Student {

  std::string nombres;
  std::string apellidos;
  std::string carrera;
  float mensualidad{};

  Student() = default;

  Student(std::string _nombre, std::string _apellido,
          std::string _carrera = generate_string(size_dist(get_generator())),
          float _mensualidad = float_dist(get_generator()))
      : nombres(std::move(_nombre)), apellidos(std::move(_apellido)),
        carrera(std::move(_carrera)), mensualidad(_mensualidad) {}

  friend auto operator<<(std::ostream &stream, const Student &student)
      -> std::ostream & {
    return stream << std::format("{:<10}  {:<10}\n"
                                 "Carrera: {:<8}\tMensualidad: {:<4}\n",
                                 student.nombres, student.apellidos,
                                 student.carrera, student.mensualidad);
  }

private:
  [[maybe_unused]] static auto get_generator() -> std::mt19937 &;
  [[maybe_unused]] static auto generate_string(size_t size) -> std::string;

  inline static std::uniform_int_distribution<size_t> size_dist{
      1, std::numeric_limits<size_t>::max()};

  inline static std::uniform_real_distribution<float> float_dist{
      0.0F, std::numeric_limits<float>::max()};
};

#endif // !STUDENT_HPP
