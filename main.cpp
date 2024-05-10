#include "iostream"
#include "filesystem"
#include "fstream"
#include "sstream"
#include "string"
#include "Disco.h"
#include "Esquema.h"
#include "Dbms.h"
#include "BufferManager.h"
using namespace std;
namespace fs = std::filesystem;


class File {
private:
    int numSectores, numPistas, numSuperficies, numPlatos, capacidadSector;
    string bd;
public:
    void createDisk(){
        cout << "Ingrese la cantidad de Platos: " <<endl;
        cin >> numPlatos;
        cout << "Ingrese la cantidad de Pistas por Superficie: " <<endl;
        cin >> numPistas;
        cout << "Ingrese la cantidad de Sectores por Pista: " <<endl;
        cin >> numSectores;
        cout << "Ingrese la capacidad de Sector (bytes): " <<endl;
        cin >> capacidadSector;
        Disco disco(capacidadSector, numSectores, numPistas, numPlatos);
        cin.ignore();
        cout << "Ingrese el nombre del Disco: "<<endl;
        getline(std::cin, bd);
        disco.createFolder(bd);
        cout << "Ingrese el numero de sectores por Bloque: "<<endl;
        int numSectoresPerBloque, sizeFrame, numFrames;
        cin >> numSectoresPerBloque;
        disco.asignarBloques(numSectoresPerBloque, bd);
        cout << "Ahora s eprocedera a crear el BufferPool..."<<endl;
        cout << "Ingrese el numero de Frames del BufferPool: "<<endl;
        cin >> numFrames;
        sizeFrame = capacidadSector * numSectoresPerBloque;
        BufferManager bufferManager(numFrames, sizeFrame);
        bufferManager.createBufferPool(bd);
    }
};

int main() {
    int numSupBrficies=2, numPistas, numSectores, capacidadSector, numPlatos;
    string query, fileName, search, DataBaseName, bd, response;;
    File file;
    Esquema esquema;
    while (true) {
        cout << "Ingrese una consulta (CREATE_DISK, READ, CREATE, INSERT, SELECT, LOCATION, BUFFER): ";
        getline(std::cin, query);

        if (query == "CREATE_DISK") {
            file.createDisk();

        }else if(query == "READ") {
            cout<<"Ingrese el nombre de la Base de Datos para cargar (sin extensión): ";
            getline(cin, DataBaseName);
            cout<<"Ingrese el nombre del Disco a utilizar:";
            getline(cin,bd);
            string DataBase = DataBaseName + ".txt";
            string EsquemaName = DataBaseName + "_esquema.txt";
            cout<<"Ingrese el tipo de registro a utilizar:";
            cout<<"1. Longitud Variable"<<endl;
            cout<<"2. Longitud Fija"<<endl;
            int opcion;
            cout<<"Opcion: "; cin>>opcion;
            if(opcion == 1){
                esquema.processLongitudVariable(bd,DataBaseName);
            }else if(opcion == 2){
                esquema.processEsquema(DataBase,EsquemaName);
            }
            Dbms dbms(bd);
            dbms.fillSectors(DataBase);
        } else if (query == "CREATE") {
            cout << "Ingrese el nombre del archivo: ";
            getline(std::cin, fileName);

            //megatron.create(fileName);
        } else if (query == "INSERT") {
            Dbms dbms(fileName);
            dbms.insert();
            //megatron.insert();
        } else if (query == "SELECT") {
            cout << "Ingrese el nombre del disco: ";
            getline(std::cin, fileName);
            Dbms dbms(fileName);
            dbms.select();
            //megatron.select(fileName, search);
        } else if (query == "LOCATION") {
            //megatron.location();
        } else if (query == "DELETE"){
            //megatron.del();
        } else if (query == "INFO"){
            cout << "Ingrese el nombre del Disco: ";
            getline(cin, fileName);
            Dbms dbms(fileName);
            dbms.info();
        } else if (query == "BUFFER"){
            string name_dico;
            cout<<"Ingrese el nombre del disco: ";
            getline(cin,name_dico);
            BufferManager buffermanager(name_dico);
            buffermanager.menu_buffer();
        }

        cout << "Continua (s/n)? ";
        getline(std::cin, response);

        if (response != "s")
            break;
    }
    return 0;
}