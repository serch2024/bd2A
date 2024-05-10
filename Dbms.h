#ifndef DBMS_H
#define DBMS_H
#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include "BufferManager.h"
#include "Disco.h"
#include "map"
using namespace std;
namespace fs = filesystem;


class Dbms {
private:
    string diskName;
    int capacidadDelSector;
    int numSectores;
    int numPistas;
    int numSuperficies;
    int numPlatos;


public:
    Dbms(const string& diskName) : diskName(diskName) {
        if (diskName.empty()) {
            cout << "Error: El nombre del disco no fue proporcionado." << endl;
            return;
        }

        string filePath = diskName + "/" + diskName + ".txt";
        if (!fs::exists(filePath)) {
            cout << "El archivo '" << filePath << "' no existe." << endl;
            return;
        }

        ifstream diskFile(filePath);
        if (!diskFile) {
            cout << "No se pudo abrir el archivo del disco en '" << filePath << "'." << endl;
            return;
        }

        diskFile >> capacidadDelSector >> numSectores >> numPistas >> numSuperficies >> numPlatos;
        diskFile.close();
    }
    struct nodo {
        string clave;
        string nombreArchivo;
    };

    map<string, nodo> BPlusTree;

    void fillSectors(string dbName) {
        int pesoLine = 0, linePerSector = 0, totalLineCount = 0;
        ifstream weightFile("weightline.txt");
        if (!weightFile) {
            cout << "No se pudo abrir el archivo de pesos." << endl;
            return;
        }
        weightFile >> pesoLine;
        weightFile.close();
        ifstream countFile(dbName);
        string countLineFile, currentPosition;
        while (getline(countFile, countLineFile)){
            totalLineCount++;
        }
        countFile.close();
        if(pesoLine > capacidadDelSector){
            cout<<"El peso del registro excede al peso del Sector"<<endl;
        }else{
            ifstream databaseFile(dbName);
            string line;
            int lineCount = 0;
            //getline(databaseFile, line);
            linePerSector = capacidadDelSector / pesoLine;
            bool copyCompleted = false;

            while (getline(databaseFile, line) && !copyCompleted) {
                for (int plato = 0; plato < numPlatos; ++plato) {
                    string carpetaPlato = diskName + "/Plato " + to_string(plato + 1);

                    for (int superficie = 0; superficie < numSuperficies; ++superficie) {
                        string carpetaSuperficie = carpetaPlato + "/Superficie " + to_string(superficie + 1);

                        for (int pista = 0; pista < numPistas; ++pista) {
                            string carpetaPista = carpetaSuperficie + "/Pista " + to_string(pista + 1);

                            for (int sector = 0; sector < numSectores; ++sector) {
                                string position;
                                position += (plato < 9 ? "0" : "") + to_string(plato + 1);
                                position += (superficie < 9 ? "0" : "") + to_string(superficie + 1);
                                position += (pista < 9 ? "0" : "") + to_string(pista + 1);
                                position += (sector < 9 ? "0" : "") + to_string(sector + 1);
                                string archivoSector = carpetaPista + "/Sector " + position + ".txt";
                                //cout << "Archivo: " << archivoSector << endl;
                                ofstream sectorFile(archivoSector);

                                for (int i = 0; i < linePerSector && !copyCompleted; ++i) {
                                    getline(databaseFile, line);
                                    //cout << line << endl;
                                    sectorFile << line << endl;
                                    lineCount++;

                                    if (lineCount >= totalLineCount) {
                                        copyCompleted = true;
                                        currentPosition = position;
                                        //cout << position << endl;
                                        break;
                                    }
                                }

                                //cout << "\n" << endl;
                                sectorFile.close();

                                if (copyCompleted) {
                                    break;
                                }
                            }

                            if (copyCompleted) {
                                break;
                            }
                        }

                        if (copyCompleted) {
                            break;
                        }
                    }

                    if (copyCompleted) {
                        break;
                    }
                }
            }

            databaseFile.close();

        }
    }
    void info() {
        string diskname = diskName;
        int opc=0;
        cout << "Elige la opcion que desea: " << endl;
        cout << "1. Info de del Disco" << endl;
        cout << "2. Info de un Plato" << endl;
        cout << "3. Info de una Superficie" << endl;
        cout << "4. Info de una Pista" << endl;
        cout << "5. Info de un Sector" << endl;
        cout << "6. Salir" << endl;
        cout << "Opcion: "; cin >> opc;

        if(opc == 1){
            int tamanioDisco=0;
            tamanioDisco = capacidadDelSector * numSectores * numPistas * numSuperficies * numPlatos;
            cout<<"El tamaño del disco '"<<diskname<<"' es de: "<<tamanioDisco<<" bytes"<<endl;
            cout<<"Y esta estructurado de la siguiente manera: "<<endl;
            for (int plato = 0; plato < numPlatos; ++plato) {
                string carpetaPlato = diskname + "/Plato " + to_string(plato + 1);
                cout<<"Plato "<<to_string(plato +1 )<<": "<<endl;

                for (int superficie = 0; superficie < numSuperficies; ++superficie) {
                    string carpetaSuperficie = carpetaPlato + "/Superficie " + to_string(superficie + 1);
                    cout<<" Superficie "<<to_string(superficie + 1)<<": "<<endl;

                    for (int pista = 0; pista < numPistas; ++pista) {
                        string carpetaPista = carpetaSuperficie + "/Pista " + to_string(pista + 1);
                        cout<<"     Pista "<<to_string(pista + 1)<<": "<<endl;

                        for(int sector = 0; sector < numSectores; ++sector){
                            string position;
                            position += (plato < 9 ? "0" : "") + to_string(plato + 1);
                            position += (superficie < 9 ? "0" : "") + to_string(superficie + 1);
                            position += (pista < 9 ? "0" : "") + to_string(pista + 1);
                            position += (sector < 9 ? "0" : "") + to_string(sector + 1);
                            string archivoSector = carpetaPista + "/Sector " +  position + ".txt";
                            ifstream file(archivoSector);
                            //cout<<archivoSector<<endl;
                            cout<<"         Sector "<<to_string(sector + 1)<<": "<<endl;
                            if(file){
                                string line;
                                while (getline(file, line)) {
                                    cout<<"             "<<line<<endl;
                                }
                            }
                            file.close();
                        }
                    }
                }
            }
        }else if(opc == 2) {
            int tamanioPlato = 0, plato;
            tamanioPlato = capacidadDelSector * numSectores * numPistas * numSuperficies;
            cout << "Ingrese el numero de plato: ";
            cin >> plato;
            cout << "El tamaño del Plato " << plato << " es de: " << tamanioPlato << " bytes" << endl;
            cout << "Y tiene la siguiente estructura: " << endl;
            string carpetaPlato = diskname + "/Plato " + to_string(plato);
            cout << "Plato " << plato << ": " << endl;

            for (int superficie = 0; superficie < numSuperficies; ++superficie) {
                string carpetaSuperficie = carpetaPlato + "/Superficie " + to_string(superficie + 1);
                cout << " Superficie " << to_string(superficie + 1) << ": " << endl;

                for (int pista = 0; pista < numPistas; ++pista) {
                    string carpetaPista = carpetaSuperficie + "/Pista " + to_string(pista + 1);
                    cout << "     Pista " << to_string(pista + 1) << ": " << endl;

                    for (int sector = 0; sector < numSectores; ++sector) {
                        string position;
                        position += (plato < 9 ? "0" : "") + to_string(plato);
                        position += (superficie < 9 ? "0" : "") + to_string(superficie + 1);
                        position += (pista < 9 ? "0" : "") + to_string(pista + 1);
                        position += (sector < 9 ? "0" : "") + to_string(sector + 1);
                        string archivoSector = carpetaPista + "/Sector " + position + ".txt";
                        ifstream file(archivoSector);
                        //cout<<archivoSector<<endl;
                        cout << "         Sector " << to_string(sector + 1) << ": " << endl;
                        if (file) {
                            string line;
                            while (getline(file, line)) {
                                cout << "             " << line << endl;
                            }
                        }
                        file.close();
                    }
                }
            }
        } else if (opc == 3) {
            int tamanioSuperficie = 0, superficie, plato;
            tamanioSuperficie = capacidadDelSector * numSectores * numPistas;
            cout << "Ingrese el numero de plato: ";
            cin >> plato;
            cout << "Ingrese el numero de superficie: ";
            cin >> superficie;
            cout << "El tamaño de la Superficie " << plato << " del Plato " << plato << " es de: " << tamanioSuperficie << " bytes" << endl;
            cout << "Y tiene la siguiente estructura: " << endl;

            string carpetaPlato = diskname + "/Plato " + to_string(plato);
            cout << "Plato " << plato << ": " << endl;
            string carpetaSuperficie = carpetaPlato + "/Superficie " + to_string(superficie);
            cout << " Superficie " << superficie<< ": " << endl;

            for (int pista = 0; pista < numPistas; ++pista) {
                string carpetaPista = carpetaSuperficie + "/Pista " + to_string(pista + 1);
                cout << "     Pista " << to_string(pista + 1) << ": " << endl;

                for (int sector = 0; sector < numSectores; ++sector) {
                    string position;
                    position += (plato < 9 ? "0" : "") + to_string(plato);
                    position += (superficie < 9 ? "0" : "") + to_string(superficie);
                    position += (pista < 9 ? "0" : "") + to_string(pista + 1);
                    position += (sector < 9 ? "0" : "") + to_string(sector + 1);
                    string archivoSector = carpetaPista + "/Sector " + position + ".txt";
                    ifstream file(archivoSector);
                    //cout<<archivoSector<<endl;
                    cout << "         Sector " << to_string(sector + 1) << ": " << endl;
                    if (file) {
                        string line;
                        while (getline(file, line)) {
                            cout << "             " << line << endl;
                        }
                    }
                    file.close();
                }
            }

        } else if (opc == 4) {
            int tamanioPista = 0, pista, superficie, plato;
            tamanioPista = capacidadDelSector * numSectores;
            cout << "Ingrese el numero de plato: ";
            cin >> plato;
            cout << "Ingrese el numero de superficie: ";
            cin >> superficie;
            cout << "Ingrese el numero de pista: ";
            cin >> pista;
            cout << "El tamaño de la Pista "<<pista<<" de la Superficie " << plato << " del Plato " << plato << " es de: " << tamanioPista << " bytes" << endl;
            cout << "Y tiene la siguiente estructura: " << endl;

            string carpetaPlato = diskname + "/Plato " + to_string(plato);
            cout << "Plato " << plato << ": " << endl;
            string carpetaSuperficie = carpetaPlato + "/Superficie " + to_string(superficie);
            cout << " Superficie " << superficie<< ": " << endl;
            string carpetaPista = carpetaSuperficie + "/Pista " + to_string(pista);
            cout << "     Pista " << to_string(pista) << ": " << endl;

            for (int sector = 0; sector < numSectores; ++sector) {
                string position;
                position += (plato < 9 ? "0" : "") + to_string(plato);
                position += (superficie < 9 ? "0" : "") + to_string(superficie);
                position += (pista < 9 ? "0" : "") + to_string(pista);
                position += (sector < 9 ? "0" : "") + to_string(sector + 1);
                string archivoSector = carpetaPista + "/Sector " + position + ".txt";
                ifstream file(archivoSector);
                //cout<<archivoSector<<endl;
                cout << "         Sector " << to_string(sector + 1) << ": " << endl;
                if (file) {
                    string line;
                    while (getline(file, line)) {
                        cout << "             " << line << endl;
                    }
                }
                file.close();
            }

        } else if (opc == 5) {
            int tamanioSector = 0, sector, pista, superficie, plato;
            tamanioSector = capacidadDelSector;
            cout << "Ingrese el numero de plato: ";
            cin >> plato;
            cout << "Ingrese el numero de superficie: ";
            cin >> superficie;
            cout << "Ingrese el numero de pista: ";
            cin >> pista;
            cout << "Ingrese el numero de sector: ";
            cin >> sector;
            cout << "El tamaño del Sector "<<sector<<" de la Pista "<<pista<<" de la Superficie " << plato << " del Plato " << plato << " es de: " << tamanioSector<< " bytes" << endl;
            cout << "Y tiene la siguiente estructura: " << endl;

            string carpetaPlato = diskname + "/Plato " + to_string(plato);
            cout << "Plato " << plato << ": " << endl;
            string carpetaSuperficie = carpetaPlato + "/Superficie " + to_string(superficie);
            cout << " Superficie " << superficie<< ": " << endl;
            string carpetaPista = carpetaSuperficie + "/Pista " + to_string(pista);
            cout << "     Pista " << to_string(pista) << ": " << endl;
            string position;
            position += (plato < 9 ? "0" : "") + to_string(plato);
            position += (superficie < 9 ? "0" : "") + to_string(superficie);
            position += (pista < 9 ? "0" : "") + to_string(pista);
            position += (sector < 9 ? "0" : "") + to_string(sector);
            string archivoSector = carpetaPista + "/Sector " + position + ".txt";
            ifstream file(archivoSector);
            //cout<<archivoSector<<endl;
            cout << "         Sector " << to_string(sector) << ": " << endl;
            if (file) {
                string line;
                while (getline(file, line)) {
                    cout << "             " << line << endl;
                }
            }
            file.close();
        } else if (opc == 6) {
            return;
        }
    }
    void searchBPlusTree(const string& clave) {
        auto it = BPlusTree.find(clave);
        if (it != BPlusTree.end()) {
            string archivo = it->second.nombreArchivo;
            ifstream file(archivo);
            if (file.is_open()) {
                string linea;
                while (getline(file, linea)) {
                    istringstream ss(linea);
                    string atributo;
                    while (getline(ss, atributo, ',')) {
                        cout << atributo << " ";
                    }
                    cout << " (En archivo: " << archivo << ")" << endl;
                }
                file.close();
            }
            else {
                cout << "Error al abrir el archivo: " << archivo << endl;
            }
        }
        else {
            cout << "Registro no encontrado en el B+ tree." << endl;
        }
    }

    void agregarClaves(string diskname, int capacidadSector, int numSectores, int numPistas, int numSuperficies, int numPlatos) {

        for (int plato = 0; plato < numPlatos; ++plato) {
            string carpetaPlato = diskname + "/Plato " + to_string(plato + 1);

            for (int superficie = 0; superficie < numSuperficies; ++superficie) {
                string carpetaSuperficie = carpetaPlato + "/Superficie " + to_string(superficie + 1);

                for (int pista = 0; pista < numPistas; ++pista) {
                    string carpetaPista = carpetaSuperficie + "/Pista " + to_string(pista + 1);

                    for (int sector = 0; sector < numSectores; ++sector) {
                        string position;
                        position += (plato < 9 ? "0" : "") + to_string(plato + 1);
                        position += (superficie < 9 ? "0" : "") + to_string(superficie + 1);
                        position += (pista < 9 ? "0" : "") + to_string(pista + 1);
                        position += (sector < 9 ? "0" : "") + to_string(sector + 1);
                        string archivoSector = carpetaPista + "/Sector " + position + ".txt";

                        ifstream file(archivoSector);
                        if (file.is_open()) {
                            string linea;
                            while (getline(file, linea)) {
                                istringstream ss(linea);
                                string clave;
                                getline(ss, clave, ','); // La clave es el primer atributo
                                nodo registro = { clave, archivoSector };
                                BPlusTree.insert({ clave, registro });
                            }
                            file.close();
                        }
                        else {
                            cout << "Error al abrir el archivo: " << archivoSector << endl;
                        }
                    }
                }
            }
        }
    }
    void select() {
        string diskname = diskName;
        int capacidadSector = capacidadDelSector;
        int Sectores = numSectores;
        int Pistas = numPistas;
        int Superficies = numSuperficies;
        int Platos = numPlatos;

        agregarClaves(diskname, capacidadSector, Sectores, Pistas, Superficies, Platos);

        int opcion;
        string clave;
        do {
            cout << "====== MENÚ ======" << endl;
            cout << "1. Buscar registro por clave" << endl;
            cout << "0. Salir" << endl;
            cout << "===================" << endl;
            cout << "Ingrese opción: ";
            cin >> opcion;

            switch (opcion) {
                case 1:
                    cout << "Ingrese la clave del registro a buscar: ";
                    cin >> clave;
                    searchBPlusTree(clave);
                    break;
                case 0:
                    cout << "Saliendo del programa." << endl;
                    break;
                default:
                    cout << "Opción inválida. Intente nuevamente." << endl;
                    break;
            }
        } while (opcion != 0);
        /*
        //recuperamos la info del bufferpool
        string nombredisco = diskName;
        int numframes = 0, sizeframes = 0;
        ifstream fileBufferPool(diskName + "/BufferPool/info.txt");
        fileBufferPool >> numframes >> sizeframes;
        fileBufferPool.close();
        cout <<"se recupero la info del buffer" <<numframes<<sizeframes<<endl;
        BufferManager bufferManager(numframes, sizeframes);
        //comprobamos si existe los archivos txt que simulan los frames del bufferpool
        if(bufferManager.paginasExists(diskName)){
            cout <<"Si existe buffer"<<endl;
        }else{
            cout <<"No existe buffer"<<endl;
            Disco disco(capacidadDelSector, numSectores, numPistas, numPlatos);
            cout <<capacidadDelSector<<"  "<<numSectores<<"  "<<numPistas<<"  "<<numPlatos<<endl;
            disco.select(nombredisco, busqueda);
            cout <<nombredisco<<"  "<<busqueda<<endl;
        }
        */

    }

    //
    void insert() {
        //ASEGURARSE DE QUE EL DISCO ESTA SELECCIONADO
        if (diskName.empty()) {
            cout << "No se ha configurado ningún disco. ";
            diskName = solicitarNombreDisco();
            if (diskName.empty()) {
                cout << "No se proporcionó un nombre de disco válido. Abortando inserción." << endl;
                return;
            }
            inicializarDisco(diskName);
        }
        //ASEGURADO DE QUE EL DISCO ESTA SELECCIONADO
        string clave, valor;

        // Solicitar al usuario la clave y el valor del nuevo registro
        cout << "Ingrese la clave del nuevo registro: ";
        getline(cin, clave);
        cout << "Ingrese el valor del nuevo registro: ";
        getline(cin, valor);

        // Validación de Datos
        if (!validarDatos(clave, valor)) {
            cout << "Validación fallida para los datos proporcionados." << endl;
            return;
        }

        // Preparación de Datos
        string dataToInsert = clave + "," + valor; // Asumiendo un formato CSV simple

        // Determinación del Sector
        Disco disco(capacidadDelSector, numSectores, numPistas, numPlatos);
        string sectorFile = disco.determinarSectorParaInsercion(clave);

        // Escritura en el Disco
        if (!disco.escribirEnSector(sectorFile, dataToInsert)) {
            cout << "Error al escribir en el disco." << endl;
        } else {
            cout << "Datos insertados correctamente en " << sectorFile << endl;
        }
    }


    //PARA AYUDAR A LA FUNCION INSERT
    string solicitarNombreDisco() {
        string nombreDisco;
        cout << "Ingrese el nombre del disco para la operación de inserción: ";
        getline(cin, nombreDisco);
        if (nombreDisco.empty() || !fs::exists(nombreDisco + "/" + nombreDisco + ".txt")) {
            cout << "El disco especificado no existe o el nombre está vacío. Por favor, intente nuevamente." << endl;
            return "";
        }
        return nombreDisco;
    }
    void inicializarDisco(const string& nombreDisco) {
        string filePath = nombreDisco + "/" + nombreDisco + ".txt";
        ifstream diskFile(filePath);
        if (!diskFile) {
            cout << "No se pudo abrir el archivo del disco '" << filePath << "'." << endl;
            return;
        }

        diskFile >> capacidadDelSector >> numSectores >> numPistas >> numSuperficies >> numPlatos;
        diskFile.close();
        cout << "Disco '" << nombreDisco << "' inicializado correctamente." << endl;
    }

    //TERMINO DE LA AYUDA A LA FUNCION INSERT

    bool validarDatos(const string& clave, const string& valor) {
        // Aquí puedes agregar lógica para validar que los datos cumplan con el esquema de la base de datos
        // Por ejemplo, comprobar que la clave no esté duplicada, que el valor cumpla con el formato, etc.
        return true; // Simplificación para el ejemplo
    }

};




#endif
