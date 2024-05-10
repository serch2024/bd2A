#ifndef BASEDEDATOS_DBMS_ARBOL_H
#define BASEDEDATOS_DBMS_ARBOL_H
#include<bits/stdc++.h>


using namespace std;

struct Registro{
    int valor;
    string direccion;
    int NumBloque;
};

struct Nodo
{
    Nodo *padre;
    Nodo *siguiente;
    Nodo *anterior;

    static int max_llaves;

    //Numero de llaves del nodo
    int  cuantas_llaves;

    bool es_hoja;

    //Posiciones llaves
    int *llaves;
    struct {
        Registro **datos;
        Nodo **hijos;
    };

    Nodo();
    Nodo(int o);
};

struct Info
{
    bool encontrado;
    Nodo *donde;
    int indice;

    Info();
};

class Arbol {
    Nodo *raiz;
    Nodo *principio;
    Nodo *final;
    int orden;

public:
    Arbol(int o);

    Info buscar(int k);
    bool agregar(int k,string dir,int b);

    void imprimirArbol();


protected:

    Nodo *buscar_hoja(Nodo *n, int k);

    void agregar_en_nodo_lleno(Nodo *n, int k, int i, Registro *r);
    void agregar_en_nodo_no_lleno(Nodo *n, int k, int i, Registro *r);
    bool agregar_y_partir_hoja(Nodo *n, int k, int i, Registro *r, Nodo *&n_s, int &k_s);
    bool agregar_y_partir_intermedio(Nodo *n, Nodo *&n_s, int &k_s);
    void agregar_y_partir_raiz(Nodo *n, int k, int i, Registro *r);

    void inicializar();
};

Registro *crearRegistro(int valor, string dir, int b);

bool nodo_lleno(Nodo *n);

int buscar_hijo_en_nodo(Nodo *n, int k);
int buscar_llave_en_nodo(Nodo *n, int k);

void desplazar_en_hoja_a_la_derecha(Nodo *n, int desde, int hasta, int e);
void desplazar_en_intermedio_a_la_derecha(Nodo *n, int desde, int hasta, int e);

void copiar_hoja(Nodo *origen, Nodo *destino, int desde, int hasta, int e);
void copiar_intermedio(Nodo *origen, Nodo *destino, int desde, int hasta, int e);

void imprimirNodo(Nodo *n);
void imprimirNivel(Nodo *n);


bool Arbol::agregar(int k,string dir,int b)
{
    Info info = buscar(k);
    //Lugar donde se debe insertar el elemento.
    int i = info.indice;

    //Repeticion
    bool encontrado = info.encontrado;

    //Direccion
    Nodo *n = info.donde;

    //El elemento se encuentra repetido en el arbol
    if(encontrado)
        return false;

    //Se crea un registro con la llave a ingresar
    Registro *r = crearRegistro(k,dir,b);

    //En la ubicacion obtenida anterioremte, se especifica si ese nodo esta lleno o no

    //printf("---> %d",info.indice);

    //printf("\nK: %d", k);
    //printf("\nI: %d", i);

    if(nodo_lleno(n))
        agregar_en_nodo_lleno(n, k, i, r);
    else
        agregar_en_nodo_no_lleno(n, k, i, r);

    //Nodo insertado
    return true;
}

void Arbol::agregar_en_nodo_no_lleno(Nodo *n, int k, int i, Registro *r)
{
    //Mover los elementos a la derecha desde i
    desplazar_en_hoja_a_la_derecha(n, i, n->cuantas_llaves-1, 1);

    //Agregar el registro en i
    n->llaves[i] = k;
    n->datos[i] = r;
    //Nueva llave en el nodo
    ++(n->cuantas_llaves);
}

void Arbol::agregar_en_nodo_lleno(Nodo *n, int k, int i, Registro *r)
{
    Nodo *p, *n_s;
    int k_s;
    bool parar;

    if(n == raiz){
        //Dividir el primer y unico nodo cuando este lleno
        agregar_y_partir_raiz(n, k, i, r);
    }else{
        do{
            p = n->padre;

            if(n->es_hoja){
                //Dividir el nodo de las hojas
                parar = agregar_y_partir_hoja(n, k, i, r, n_s, k_s);

                //true: Se inserto la hoja, se dividio el padre y el padre no se lleno
                //false: el padre se lleno

            }
            else{
                //Dividir el nodo padre de las hojas y/o nodos abuelos
                parar = agregar_y_partir_intermedio(n, n_s, k_s);
            }
            n = p;
        }while(parar == false);
    }
}

void Arbol::agregar_y_partir_raiz(Nodo *n, int k, int i, Registro *r)
{
    //En copia se inserta el nuevo registro, pero toca dividir ya que esta en orden +1
    Nodo *copia = new Nodo;
    copia->cuantas_llaves = orden;
    copia->llaves = new int[orden+1];
    copia->datos = new Registro*[orden+1];
    copiar_hoja(n, copia, 0, orden-1, 0);

    desplazar_en_hoja_a_la_derecha(copia, i, orden-1, 1);

    copia->llaves[i] = k;
    copia->datos[i] = r;

    //Se crea el nuevo nodo para agregarlo a la izquierda del ultimo nivel
    Nodo *hermano = new Nodo;
    hermano->llaves = new int[orden];
    hermano->datos = new Registro*[orden];

    //Con mitad, se definen las hojas que van a un nodo y las otras que van al nuevo nodo
    int mitad = (orden+1)/2;

    //Primer nodo
    copiar_hoja(copia, n, 0, mitad-1, 0);

    //Segundo nodo
    copiar_hoja(copia, hermano, mitad, orden, 0);
    n->cuantas_llaves = mitad;
    hermano->cuantas_llaves = orden+1 - mitad;

    hermano->siguiente = n->siguiente;
    //Puntero del primer nodo hacia el segundo nuevo nodo
    n->siguiente = hermano;
    hermano->anterior = n;

    //Nuevo padre que a la final tambien es la actualizacion de la raiz
    Nodo *nueva_raiz = new Nodo;
    nueva_raiz->es_hoja = false;
    nueva_raiz->llaves = new int[orden];
    nueva_raiz->hijos = new Nodo*[orden+1];

    nueva_raiz->llaves[0] = hermano->llaves[0];
    nueva_raiz->cuantas_llaves = 1;
    nueva_raiz->hijos[0] = n;
    nueva_raiz->hijos[1] = hermano;
    n->padre = nueva_raiz;
    hermano->padre = nueva_raiz;

    raiz = nueva_raiz;
    principio = n;
    final = hermano;

    delete copia;
}

bool Arbol::agregar_y_partir_hoja(Nodo *n,int k,int i,Registro *r,Nodo *&n_s,int &k_s)
{
    Nodo *p = n->padre;

    //Valor del indice menor del nodo padre
    int i_padre = buscar_hijo_en_nodo(p, n->llaves[0]);

    //En copia se inserta el nuevo registro, pero toca dividir ya que esta en orden +1
    Nodo *copia = new Nodo;
    copia->cuantas_llaves = orden;
    copia->llaves = new int[orden+1];
    copia->datos = new Registro*[orden+1];

    //Se inserta el registro en el nodo
    copiar_hoja(n, copia, 0, orden-1, 0);
    desplazar_en_hoja_a_la_derecha(copia, i, orden-1, 1);
    copia->llaves[i] = k;
    copia->datos[i] = r;

    //Se crea el nuevo nodo para agregarlo a la izquierda del ultimo nivel
    Nodo *hermano = new Nodo;
    hermano->llaves = new int[orden];
    hermano->datos = new Registro*[orden];

    //Con mitad, se definen las hojas que van a un nodo y las otras que van al nuevo nodo
    int mitad = (orden+1)/2;
    copiar_hoja(copia, n, 0, mitad-1, 0);
    copiar_hoja(copia, hermano, mitad, orden, 0);
    n->cuantas_llaves = mitad;
    hermano->cuantas_llaves = orden+1 - mitad;

    hermano->siguiente = n->siguiente;
    n->siguiente = hermano;
    hermano->anterior = n;

    //El nodo antes de la insercion era el ultimo de la izquierda
    if(n == final)
        final = hermano;
    else
        hermano->siguiente->anterior = hermano;//No era el ultimo de la izquierda

    k_s = hermano->llaves[0];
    n_s = hermano;

    delete copia;

    //Si el nodo no esta lleno se inserta el indice en el padre
    if(!nodo_lleno(p)){
        desplazar_en_intermedio_a_la_derecha(p, i_padre, p->cuantas_llaves-1, 1);
        p->llaves[i_padre] = k_s;
        p->hijos[i_padre+1] = n_s;
        ++(p->cuantas_llaves);
        n_s->padre = p;
        return true; //Insercion completa
    }
    return false;
}

bool Arbol::agregar_y_partir_intermedio(Nodo *n, Nodo *&n_s, int &k_s)
{
    Nodo *p = n->padre;
    int i_padre;


    Nodo *copia = new Nodo;
    copia->es_hoja = false;
    copia->cuantas_llaves = orden;
    copia->llaves = new int[orden+1];
    copia->hijos = new Nodo*[orden+2];
    copiar_intermedio(n, copia, 0, orden-1, 0);

    //Valor del indice menor del nodo padre (el abuelo de las hojas, el visabuelo, etc.. Segun el while del agregar)
    i_padre = buscar_hijo_en_nodo(copia, k_s);

    //Desplazar a la derecha para poder insertar el nuevo indice en la posicion del i_padre
    desplazar_en_intermedio_a_la_derecha(copia, i_padre, orden-1, 1);
    copia->llaves[i_padre] = k_s;//Primer elemento del nodo en las hojas y elemento que asciende
    copia->hijos[i_padre+1] = n_s;

    Nodo *hermano = new Nodo;
    hermano->es_hoja = false;
    hermano->llaves = new int[orden];
    hermano->hijos = new Nodo*[orden+1];

    int mitad = (orden+1)/2;
    //El nodo de la izquierda
    copiar_intermedio(copia, n, 0, mitad-1, 0);

    //El nodo de la derecha
    copiar_intermedio(copia, hermano, mitad+1, orden, 0);

    //Llaves del primer nodo
    n->cuantas_llaves = mitad;

    //Llaves del segundo nodo (derecha)
    hermano->cuantas_llaves = orden - mitad;

    //Actualizacion de los padres de los hijos del nodo izquierdo
    for(int j=0; j<=n->cuantas_llaves; ++j)
        n->hijos[j]->padre = n;
    //Actualizacion de los padres de los hijos del nodo derecho
    for(int j=0; j<=hermano->cuantas_llaves; ++j)
        hermano->hijos[j]->padre = hermano;

    hermano->siguiente = n->siguiente;
    n->siguiente = hermano;
    hermano->anterior = n;

    if(hermano->siguiente != NULL)
        hermano->siguiente->anterior = hermano;
    n_s = hermano;
    k_s = copia->llaves[mitad];

    delete copia;


    if(p != NULL && !nodo_lleno(p)){
        i_padre = buscar_hijo_en_nodo(p, k_s);
        desplazar_en_intermedio_a_la_derecha(p, i_padre, p->cuantas_llaves-1, 1);
        p->llaves[i_padre] = k_s;
        p->hijos[i_padre+1] = n_s;
        n_s->padre = p;
        ++(p->cuantas_llaves);
        return true;
    }


    if(n == raiz){
        Nodo *nueva_raiz = new Nodo;
        nueva_raiz->es_hoja = false;
        nueva_raiz->llaves = new int[orden];
        nueva_raiz->hijos = new Nodo*[orden+1];

        nueva_raiz->llaves[0] = k_s;
        ++(nueva_raiz->cuantas_llaves);
        nueva_raiz->hijos[0] = n;
        nueva_raiz->hijos[1] = n_s;
        n->padre = nueva_raiz;
        n_s->padre = nueva_raiz;

        raiz = nueva_raiz;
        return true;
    }
    //Aun falta por dividir
    return false;
}

int Nodo::max_llaves;

// Constructor info
Info::Info(){
    encontrado = false;
    donde = NULL;
    indice = -1;
}

// Construccion nodo
Nodo::Nodo()
{
    cuantas_llaves = 0;
    es_hoja = true;
    padre = siguiente = anterior = NULL;
}

Nodo::Nodo(int o)
{
    cuantas_llaves = 0;
    es_hoja = true;
    padre = siguiente = anterior = NULL;
    max_llaves = o;
}

//Constructor del arbol de orden O
Arbol::Arbol(int o)
{
    orden = o;
    inicializar();
}

// Inicializacion del arbol
void Arbol::inicializar()
{
    raiz = new Nodo(orden);
    raiz->llaves = new int[orden];
    raiz->datos  = new Registro*[orden];
    principio = final = raiz;
}

//Crear dato del tipo Registro
Registro *crearRegistro(int valor, string dir, int b) {
    Registro * r = new Registro;
    r->valor = valor;
    r->direccion = dir;
    r->NumBloque = b;
    return r;
}

Info Arbol::buscar(int k) {
    Info info;


    ////La posicion en el nodo (El numero de la posicion)
    int i;

    //True si se encuentra el indice (el valor)
    bool valEncontrado;

    //El nodo donde se debe insertar el registro UBICACION   n->llaves[i] La posicion a la izquierda
    Nodo * n;

    //Nodo en donde se debe insertar la hoja
    n = buscar_hoja(raiz, k);

    //Posicion en el nodo (El valor donde se debe insertar la hoja)
    i = buscar_llave_en_nodo(n, k);

    //Llave de la derecha
    //n->llaves[i]

    valEncontrado = (i < n->cuantas_llaves && k == n->llaves[i]);
    //valEncontrado = (k == n->llaves[i]);

    info.encontrado = valEncontrado;
    info.indice = i;
    info.donde = n;

    return info;
}

//Buscar en una hoja el valor k
Nodo *Arbol::buscar_hoja(Nodo *n, int k)
{
    int i;

    //Se busca en los nodos
    while(n->es_hoja == false){
        i = buscar_hijo_en_nodo(n, k);
        n = n->hijos[i];
    }
    return n;
}


//Verifica si un nodo ya llega al orden O
bool nodo_lleno(Nodo *n) {
    return n->cuantas_llaves >= n->max_llaves;
}

//El valor k en los hijos del nodo n
int buscar_hijo_en_nodo(Nodo *n, int k){
    int i = 0;

    while(i < n->cuantas_llaves && k >= n->llaves[i])
        ++i;

    return i;
}

int buscar_llave_en_nodo(Nodo *n, int k)
{
    int i = 0;
    while(i < n->cuantas_llaves && k > n->llaves[i])
        ++i;
    return i;
}

void copiar_hoja(Nodo *origen, Nodo *destino, int desde, int hasta, int e)
{
    int j, k;
    for(j=desde, k = e; j <= hasta; ++j, ++k){
        destino->llaves[k] = origen->llaves[j];
        destino->datos[k]  = origen->datos[j];
    }
}

void copiar_intermedio(Nodo *origen, Nodo *destino, int desde, int hasta, int e)
{
    int j, k;
    for(j=desde, k = e; j <= hasta; ++j, ++k){
        destino->llaves[k] = origen->llaves[j];
        destino->hijos[k]  = origen->hijos[j];
    }
    destino->hijos[k] = origen->hijos[j];
}


void desplazar_en_hoja_a_la_derecha(Nodo *n, int desde, int hasta, int e){
    for(int j = hasta+e; j >= desde+e; --j){
        n->llaves[j] = n->llaves[j-e];
        n->datos[j]  = n->datos[j-e];
    }
}

void desplazar_en_intermedio_a_la_derecha(Nodo *n, int desde, int hasta, int e){
    for(int j = hasta+e; j >= desde+e; --j){
        n->llaves[j]  = n->llaves[j-e];
        n->hijos[j+1] = n->hijos[j-e+1];
    }
}

void imprimirNodo(Nodo *n)
{
    if(n->es_hoja)
        std::cout<<"h";
    std::cout<<"( ";
    for(int j=0; j<n->cuantas_llaves; j=j+1)
        std::cout<<n->llaves[j]<<" ";
    std::cout<<") ";
}

void imprimirNivel(Nodo *n)
{
    while(n){
        imprimirNodo(n);
        n = n->siguiente;
    }
    std::cout<<"\n        ";
}

void Arbol::imprimirArbol()
{
    printf("\n        ");
    Nodo *n = raiz;
    if(raiz->cuantas_llaves == 0){
        std::cout<<"\n        El arbol esta vacio.\n";
        return;
    }
    while(!(n->es_hoja)){
        imprimirNivel(n);
        n = n->hijos[0];
    }
    imprimirNivel(principio);
    std::cout<<"\n";
}

void menu() {
    system("cls");
    int i = 0;
    // Inicio cabecera
    printf("\n        %c", 218);
    for (i = 1; i < 55; i++) {
        printf("%c", 196);
    }
    printf("%c", 191);
    printf("\n        %c                     MEN%c DE OPCIONES", 179, 233);
    for (i = 1; i < 18; i++) {
        printf(" ");
    }
    printf("%c\n", 179);
    // fin semicabecera
    printf("        %c", 201);
    for (i = 1; i < 55; i++) {
        printf("%c", 205);
    }
    printf("%c", 187);
    printf("\n           (1) Insertar un elemento en el Arbol.");
    printf("\n           (2) Mostrar el Arbol.");
    printf("\n           (3) Buscar elemento.");
    printf("\n           (4) Salir. \n");
    printf("        %c", 200);
    for (i = 1; i < 55; i++) {
        printf("%c", 205);
    }
    printf("%c\n\n", 188);
    // Fin cabecera
}
/*
int main(int argc, char **argv) {
    system("color 02");
    Arbol *a = new Arbol(3);

    menu();
    int optMenu;
    int numero = 0;
    while (optMenu != -1) {
        printf("        Ingrese una opcion: ");
        cin >> optMenu;

        switch (optMenu) {
            case 0:
                menu();
                break;
            case 1:
                printf("\n        ------------------------------------------\n");
                printf("\n        Numero a ingresar: ");
                cin >> numero;

                if (a->agregar(numero)) {
                    printf("        Se ha agregado el elemento!\n");
                    a->imprimirArbol();
                } else
                    printf("\n        Elemento repetido, no se ha agregado el elemento!\n");
                printf("        ------------------------------------------\n");
                break;
            case 2:
                a->imprimirArbol();
                break;
            case 3:
                printf("\n        ------------------------------------------\n");
                printf("\n        Numero a buscar: ");
                cin >> numero;
                if (a->buscar(numero).encontrado) {
                    cout<<a->buscar(numero).donde->datos[a->buscar(numero).indice]->valor<<": Elemento encontrado"<<endl;
                } else {
                    cout<<a->buscar(numero).donde->datos[a->buscar(numero).indice]->valor<<": Elemento encontrado"<<endl;
                }
                printf("        ------------------------------------------\n");
                break;
            case 4:
                optMenu = -1;
                break;
            default:
                break;
        }
    }
}*/

#endif //BASEDEDATOS_DBMS_ARBOL_H
