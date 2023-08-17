#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

class Alumno//Record
{	
private:    
	char Nombre[12];    
	char Apellidos[12];	
public:
    void setData(){
        cout<<"Nombre:";
        cin>>Nombre;
        cout<<"Apellidos:";
        cin>>Apellidos;
    }    
    void showData(){
        cout<<Nombre<<" - "<<Apellidos<<endl;
    }
};


class FixedRecordFile
{
private:
    string file_name;
public:
    FixedRecordFile(string file_name){
        this->file_name = file_name;        
    } 

    void writeRecord(Alumno record){
        ofstream file(this->file_name, ios::app | ios::binary);
        if(!file.is_open()) throw ("No se pudo abrir el archivo");
        file.write((char*) &record, sizeof(Alumno));//guardar en formato binario
        file.close();
    }  

    void writeRecord(Alumno record, int pos){
        ofstream file(this->file_name, ios::app | ios::binary);
        if(!file.is_open()) throw ("No se pudo abrir el archivo");

        file.seekp(pos * sizeof(Alumno), ios::beg);//fixed length record
        file.write((char*) &record, sizeof(Alumno));
        file.close();
    } 

    vector<Alumno> scanAll(){
        ifstream file(this->file_name, ios::binary);
        if(!file.is_open()) throw ("No se pudo abrir el archivo");
        
        vector<Alumno> alumnos;
        Alumno record; 

        while(file.peek() != EOF){
            record = Alumno();               
            file.read((char*) &record, sizeof(Alumno));            
            alumnos.push_back(record);    
        }
        file.close();

        return alumnos;
    } 

    Alumno readRecord(int pos){
        ifstream file(this->file_name, ios::binary);
        if(!file.is_open()) throw ("No se pudo abrir el archivo");

        Alumno record;
        file.seekg(pos * sizeof(Alumno), ios::beg);//fixed length record
        file.read((char*) &record, sizeof(Alumno));
        file.close();
        return record;
    }

    int size(){
        ifstream file(this->file_name, ios::binary);
        if(!file.is_open()) throw ("No se pudo abrir el archivo");
        
        file.seekg(0, ios::end);//ubicar cursos al final del archivo
        long total_bytes = file.tellg();//cantidad de bytes del archivo        
        file.close();
        return total_bytes / sizeof(Alumno);
    }
};

int main()
{
    //Escritura
    FixedRecordFile file1("data.bin");
    Alumno record;
    record.setData();
    file1.writeRecord(record);

    //Lectura
    FixedRecordFile file2("data.bin");
    vector<Alumno> alumnos = file2.scanAll();
    for(Alumno r : alumnos){
        r.showData();
    }
    return 0;
}