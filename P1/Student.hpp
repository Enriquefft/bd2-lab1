#ifndef STUDENT_HPP
#define STUDENT_HPP

#include <cstring>
#include <format>
#include <limits>
#include <random>

constexpr std::size_t MAX_CODIGO_LEN = 5;
constexpr std::size_t MAX_NOMBRE_LEN = 11;
constexpr std::size_t MAX_APELLIDO_LEN = 20;
constexpr std::size_t MAX_CARRERA_LEN = 15;

struct Student {

  // NOLINTBEGIN(modernize-avoid-c-arrays)
  char codigo[MAX_CODIGO_LEN];
  char nombre[MAX_NOMBRE_LEN];
  char apellidos[MAX_APELLIDO_LEN];
  char carrera[MAX_CARRERA_LEN];
  // NOLINTEND(modernize-avoid-c-arrays)

  int ciclo;
  float mensualidad;

  Student(const char *_codigo, const char *_nombre)
      : ciclo(ciclo_dist(get_generator())),
        mensualidad(mensualidad_dist(get_generator()))

  {
    generate_string(static_cast<char *>(apellidos), MAX_APELLIDO_LEN);
    generate_string(static_cast<char *>(carrera), MAX_CARRERA_LEN);

    strncpy(static_cast<char *>(codigo), _codigo, MAX_CODIGO_LEN);
    strncpy(static_cast<char *>(nombre), _nombre, MAX_NOMBRE_LEN);
  }

  friend auto operator<<(std::ostream &stream, const Student &student)
      -> std::ostream & {
    return stream << std::format("{:<3}\t{:<10}  {:<10}\n"
                                 "Ciclo: {:<2}\tMensualidad: {:<4}\n",
                                 student.codigo, student.nombre,
                                 student.apellidos, student.carrera,
                                 student.ciclo, student.mensualidad);
  }

private:
  static auto get_generator() -> std::mt19937 &;
  static void generate_string(char *member, size_t size);

  inline static std::uniform_int_distribution<int> ciclo_dist{
      1, std::numeric_limits<int>::max()};

  inline static std::uniform_real_distribution<float> mensualidad_dist{
      0.0F, std::numeric_limits<float>::max()};
};

#endif // !STUDENT_HPP
