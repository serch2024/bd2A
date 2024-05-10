#ifndef DISCO_H
#define DISCO_H
#include "algorithm"
#include "cctype"
#include "iostream"
#include "filesystem"
#include "fstream"
#include "sstream"
#include "string"
#include "vector"


using namespace std;
namespace fs = filesystem; // fs se llama a la librería de filesystem
class Disco {
private:
    int capacidadDisco, capacidadSector, numSectores, numPistas, numSuperficies=2, numPlatos;
    string diskname; //el nombre que tiene la carpeta que hace como disco
public:
    //constructor para guardar los diferente valores del disco
    Disco(int capacidadSector, int numSectores, int numPistas,  int numPlatos) :
    capacidadSector(capacidadSector), numSectores(numSectores), numPistas(numPistas), numPlatos(numPlatos)
    {
        capacidadDisco = capacidadSector * numSectores * numPistas * numPistas * numPlatos;


    }
    bool diskExists(const string& diskName) //verifica si el disco existe
    {
        return fs::exists(diskName);
    }
    void createFolder(string diskname){ //crea la carpeta
        if (diskExists(diskname)) {    //llamamos a la funcion de verificar si el disco existe
            cout << "El disco ya existe." << endl;
            return;
        }else{
            // creamos las caarpetas, subcarpetas y archivos de txt
            fs::create_directory(diskname);
            for (int plato = 0; plato < numPlatos; ++plato) {
                //definimos  la ubicacion de la carpeta, iterando segun nuemro de platos
                // sumamos +1 a plato ya que comenzamos desde 0 en la iteracion
                string carpetaPlato = diskname + "/Plato " + to_string(plato + 1);
                fs::create_directory(carpetaPlato);

                for (int superficie = 0; superficie < numSuperficies; ++superficie) {
                    //definimos  la ubicacion de la carpeta, iterando segun nuemro de superficies
                      //sumamos +1 a superficie ya que comenzamos desde 0 en la iteracion
                    string carpetaSuperficie = carpetaPlato + "/Superficie " + to_string(superficie + 1);
                    fs::create_directory(carpetaSuperficie);

                    for (int pista = 0; pista < numPistas; ++pista) {
                        //definimos  la ubicacion de la carpeta, iterando segun nuemro de pista
                        //sumamos +1 a pista ya que comenzamos desde 0 en la iteracion
                        string carpetaPista = carpetaSuperficie + "/Pista " + to_string(pista + 1);
                        fs::create_directory(carpetaPista);
                        for(int sector = 0; sector < numSectores; ++sector){
                            string position;
                            //segun los valores de numSectores, numPistas, numSuperficies, numPlatos actuales
                            //creamos la posicion del sector que tambien funcionara como nombre del archivo txt
                            position += (plato < 9 ? "0" : "") + to_string(plato + 1);
                            position += (superficie < 9 ? "0" : "") + to_string(superficie + 1);
                            position += (pista < 9 ? "0" : "") + to_string(pista + 1);
                            position += (sector < 9 ? "0" : "") + to_string(sector + 1);
                            string archivoSector = carpetaPista + "/Sector " +  position + ".txt";
                            ofstream file(archivoSector);
                            file.close();
                        }
                    }
                }
            }
        }
        //al crear el disco creamos el archivo con los valores de capacidad, numSectores, numPistas, numSuperficies, numPlatos
        // y lo guardamos en un archivo de texto con el mismo nombre de la carpeta del disco
        ofstream file;
        file.open(diskname + "/" + diskname + ".txt", ios::out);
        file << capacidadSector << " " << numSectores << " " << numPistas << " " << numSuperficies << " " <<numPlatos << endl;
        file.close();
        cout<<"Disco creado con exito"<<endl;
        cout<< "El disco "<<diskname<<" tiene "<<capacidadDisco<<" bytes."<<endl;
    }

    void asignarBloques(int numSectoresPerBloque, string diskname) {
        string numSectoresValue = diskname + "/infoBloque.txt";
        ofstream numSectoresporBloque(numSectoresValue);
        numSectoresporBloque << numSectoresPerBloque;
        numSectoresporBloque.close();
        //recibimos los parametros de numero de sectores por bloque y el nombre del disco
        string directoryFile = diskname + "/DirectorioDeBloques.txt";
        ofstream dirFile(directoryFile.c_str(), ios::app);
        //calculamos el numero de bloques a partir del numero total de sectores
        int numSectoresTotal = numSectores * numPistas * numSuperficies * numPlatos;
        int numBloques = numSectoresTotal / numSectoresPerBloque;

        for (int bloque = 0; bloque < numBloques; ++bloque) {
            string bloqueFixed;
            bloqueFixed = (bloque <= 9 ? "0" : "") + to_string(bloque);
            dirFile << "Bloque " << bloqueFixed << ": ";
            int sectorInicio = bloque * numSectoresPerBloque;

            for (int i = 0; i < numSectoresPerBloque; ++i) {
                int sector = sectorInicio + i;
                int plato = sector / (numSectores * numSuperficies * numPistas);
                int residuo = sector % (numSectores * numSuperficies * numPistas);
                int superficie = residuo / (numSectores * numPistas);
                residuo = residuo % (numSectores * numPistas);
                int pista = residuo / numSectores;
                int sectorEnPista = residuo % numSectores;

                string position;
                position += (plato < 9 ? "0" : "") + to_string(plato + 1);
                position += (superficie < 9 ? "0" : "") + to_string(superficie + 1);
                position += (pista < 9 ? "0" : "") + to_string(pista + 1);
                position += (sectorEnPista < 9 ? "0" : "") + to_string(sectorEnPista + 1);

                dirFile << position << " ";
            }

            dirFile << endl;
        }

        dirFile.close();
    }
    void select(string diskname, string search) {

        for (int plato = 0; plato < numPlatos; ++plato) {
            string carpetaPlato = diskname + "/Plato " + to_string(plato + 1);

            for (int superficie = 0; superficie < numSuperficies; ++superficie) {
                string carpetaSuperficie = carpetaPlato + "/Superficie " + to_string(superficie + 1);

                for (int pista = 0; pista < numPistas; ++pista) {
                    string carpetaPista = carpetaSuperficie + "/Pista " + to_string(pista + 1);

                    for(int sector = 0; sector < numSectores; ++sector){
                        string position;
                        position += (plato < 9 ? "0" : "") + to_string(plato + 1);
                        position += (superficie < 9 ? "0" : "") + to_string(superficie + 1);
                        position += (pista < 9 ? "0" : "") + to_string(pista + 1);
                        position += (sector < 9 ? "0" : "") + to_string(sector + 1);
                        string archivoSector = carpetaPista + "/Sector " +  position + ".txt";
                        ifstream file(archivoSector);
                        //cout<<archivoSector<<endl;
                        if(file){
                            string line;
                            while (getline(file, line)) {
                                line.erase(std::remove_if(line.begin(), line.end(), [](unsigned char c) { return std::isspace(c) || c == '\"'; }), line.end());
                                if (line.find(search) != string::npos) {
                                    //cout<<line<<endl;

                                    cout<<"El archivo "<<archivoSector<<" contiene el bloque "<<line<<endl;
                                }
                            }
                        }
                        file.close();
                    }
                }
            }
        }
    }
    //insertar registro


    //Este método en Disco.h sería responsable de la escritura física en el archivo de sector correspondiente.
    bool escribirEnSector(const string& sectorFile, const string& data) {
        // Comprobar si el directorio existe y crearlo si no
        string directoryPath = sectorFile.substr(0, sectorFile.find_last_of('/'));
        if (!fs::exists(directoryPath)) {
            fs::create_directories(directoryPath); // Asegúrate de que tienes permisos para crear directorios
        }

        ofstream out(sectorFile, ios::app); // Abrir en modo append
        if (!out) {
            cerr << "Error al abrir el archivo: " << sectorFile << endl;
            return false; // Falla al abrir el archivo
        }
        out << data << endl; // Escribir datos
        if (!out.good()) {
            cerr << "Error al escribir datos en el archivo." << endl;
            out.close();
            return false; // Error al escribir datos
        }
        out.close();
        return true; // Escritura exitosa
    }
    // Método para determinar en qué sector del disco se debe insertar el registro.
    string determinarSectorParaInsercion(const string& clave) {
        std::hash<std::string> hasher;
        size_t hashValue = hasher(clave);

        // Calcular el bloque en el que se debería insertar la clave
        int numTotalSectores = numSectores * numPistas * numSuperficies * numPlatos;
        int sectorIndex = hashValue % numTotalSectores;

        int plato = sectorIndex / (numSectores * numPistas * numSuperficies);
        int residuo = sectorIndex % (numSectores * numPistas * numSuperficies);
        int superficie = residuo / (numSectores * numPistas);
        residuo = residuo % (numSectores * numPistas);
        int pista = residuo / numSectores;
        int sector = residuo % numSectores;

        // Construir la ruta del archivo del sector
        std::stringstream archivoSector;
        archivoSector << diskname << "/Plato" << (plato + 1)
                      << "/Superficie" << (superficie + 1)
                      << "/Pista" << (pista + 1)
                      << "/Sector" << (sector + 1) << ".txt";
        return archivoSector.str();
    }
    bool abrirArchivo(const std::string& nombreArchivo) {
        std::ifstream archivo(nombreArchivo);
        if (!archivo) {
            std::cerr << "Error: No se pudo abrir el archivo " << nombreArchivo << std::endl;
            if (!std::filesystem::exists(nombreArchivo)) {
                std::cerr << "El archivo no existe." << std::endl;
            } else {
                std::cerr << "Verifica los permisos del archivo." << std::endl;
            }
            return false;
        }
        archivo.close();
        return true;
    }


};
#endif
