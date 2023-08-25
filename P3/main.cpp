#include <fstream>
#include <iostream>
#include <vector>

struct Alumno {
  std::string codigo;
  int ciclo; // Luego usar int32
  float mensualidad;
  std::string observaciones;

  friend std::ostream &operator<<(std::ostream &os, Alumno &other) {
    os << other.codigo << " ";
    os << other.ciclo << " ";
    os << other.mensualidad << " ";
    os << other.observaciones;
    return os;
  }

  int size_of() const {
    return codigo.size() + sizeof(ciclo) + sizeof(mensualidad) +
           observaciones.size();
  }

  friend std::ofstream &operator<<(std::ofstream &of, Alumno &other) {
    int sz2 = other.codigo.size(), sz3 = other.observaciones.size();

    of.write((char *)&sz2, sizeof(sz2));
    of.write(other.codigo.c_str(), sz2);

    of.write((char *)&other.ciclo, sizeof(ciclo));

    of.write((char *)&other.mensualidad, sizeof(mensualidad));

    of.write((char *)&sz3, sizeof(sz3));
    of.write(other.observaciones.c_str(), sz3);

    return of;
  }

  friend std::ifstream &operator>>(std::ifstream &in, Alumno &other) {
    int tmn_codigo, tmn_observaciones;

    in.read((char *)&tmn_codigo, sizeof(tmn_codigo));
    char *buffer1 = new char[tmn_codigo + 1];
    in.read(buffer1, tmn_codigo);
    buffer1[tmn_codigo] = '\0';
    other.codigo = buffer1;

    in.read((char *)&other.ciclo, sizeof(other.ciclo));
    in.read((char *)&other.mensualidad, sizeof(other.mensualidad));

    in.read((char *)&tmn_observaciones, sizeof(tmn_observaciones));
    char *buffer2 = new char[tmn_observaciones + 1];
    in.read(buffer2, tmn_observaciones);
    buffer2[tmn_observaciones] = '\0';
    other.observaciones = buffer2;

    delete[] buffer1;
    delete[] buffer2;
    return in;
  }
};

class VariableRecord {
private:
  std::string data_filename;
  std::string index_filename;

public:
  explicit VariableRecord(const std::string &dfile) {
    data_filename = dfile + ".dat";
    index_filename = "index_" + dfile + ".dat";
  }

  // Escribe un Alumno en el archivo
  void add(Alumno &record) {
    // Añadimos el alumno al data file
    std::ofstream file(data_filename, std::ios::binary | std::ios::app);
    file.seekp(0, std::ios::end);
    int pos = file.tellp(); // Posición del cursor al final del archivo (En bytes)
    file << record;
    file.close();

    // Añadimos la posición
    std::ofstream idx_file(index_filename, std::ios::binary | std::ios::app);
    idx_file.seekp(0, std::ios::end);
    idx_file.write((char *)&pos, sizeof(pos));
    idx_file.close();
  }

  std::vector<Alumno> load() {
    std::vector<Alumno> vec;
    // Calculamos la cantidad de registros con el index file
    std::ifstream index(index_filename, std::ios::binary);
    index.seekg(0, std::ios::end);
    int final = index.tellg();
    int n = final / sizeof(final);
    index.close();

    std::ifstream file(data_filename, std::ios::binary);
    Alumno alumno;
    for (int i = 0; i < n; ++i) {
      file >> alumno;
      vec.push_back(alumno);
    }
    file.close();
    return vec;
  }

  Alumno readRecord(int pos = 0) {
    if (pos < 0)
      throw("Posicion invalida");
    // Se lee la posición física y el tamaño del record en el index file
    int pos_fisica;
    std::ifstream idx_file(index_filename, std::ios::binary);
    idx_file.seekg(pos * sizeof(pos_fisica));
    idx_file.read((char *)&pos_fisica, sizeof(pos_fisica));
    idx_file.close();

    // Se lee el alumno en el data file
    Alumno alumno;
    std::ifstream file(data_filename, std::ios::binary);
    file.seekg(pos_fisica);
    file >> alumno;
    file.close();

    return alumno;
  }
};

void test_write(VariableRecord &vr) {
  std::cout << "-----TESTING WRITE------" << std::endl;
  Alumno a1 = Alumno{"2022", 11, 1040.5, "A"};
  Alumno a2 = Alumno{"2021", 12, 1020.5, "C"};
  Alumno a3 = Alumno{"2024", 10, 1030.5, "B"};

  vr.add(a1);
  vr.add(a2);
  vr.add(a3);
}

void test_read(VariableRecord &vr) {
  std::cout << "-----TESTING READ------" << std::endl;
  Alumno a, b, c;
  a = vr.readRecord(0);
  b = vr.readRecord(1);
  c = vr.readRecord(2);
  std::cout << a << std::endl;
  std::cout << b << std::endl;
  std::cout << c << std::endl;
}

void test_load(VariableRecord &vr) {
  std::cout << "-----TESTING LOAD------" << std::endl;
  std::vector<Alumno> vec = vr.load();
  for (auto e : vec) {
    std::cout << e << std::endl;
  }
}

int main() {
  VariableRecord vr("data");
  test_write(vr);
  test_read(vr);
  test_load(vr);
  return 0;
}
