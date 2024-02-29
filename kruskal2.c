#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define VERTICES 5


typedef int tipo_nombre ;
typedef int tipo_elemento ;
typedef int vertice ;

typedef struct _ARISTA {
    vertice u ;
    vertice v ;
    int costo ;
} arista ;

typedef struct _RAMA {
    struct _ARISTA a ;
    struct _RAMA * sig ;
} rama ;

typedef struct _ENCABEZADO {
int cuenta ;
int primer_elemento ;
} encabezado ;

typedef struct _NOMBRE {
    tipo_nombre nombre_conjunto ;
    int siguiente_elemento ;
} nombre ;

typedef struct _CONJUNTO_CE {                           //grafo al que le iremos asignando vertices
    nombre nombres [ VERTICES ];
    encabezado encabezamientos_conjunto [ VERTICES ];
} conjunto_CE ;

void inicial ( tipo_nombre , tipo_elemento , conjunto_CE *) ;
void fusionar(tipo_nombre , tipo_nombre , conjunto_CE *);
void combina ( tipo_nombre , tipo_nombre , conjunto_CE *) ;

tipo_nombre encuentra ( int , conjunto_CE *) ;

void kruskal ( rama **) ;

void inserta ( int , int , int , rama **) ;

arista sacar_min ( rama **) ;

void lista ( rama *) ;


int main() {
    
    int M_Costos[VERTICES][VERTICES] = {0};
    rama *arbol = NULL;

    printf("Ingrese costo de lado entre los siguientes vertices para calcular el arbol de expansion!\n"); 

    for (int i = 0; i <= VERTICES - 1; i++) {
        for (int j = i + 1; j <= VERTICES - 1; j++) {
            printf("Costo de lado entre vertices %d y %d: ", i, j);
            
            int ingreso = 0, salir = 0;         //por si ingresa 0
            while(salir == 0){
                scanf("%d", &ingreso);
                if(ingreso <= 0){
                    printf("Ingrese un valor mayor a 0.\n");
                } else {
                    salir = 1;
                }
            }
            M_Costos[i][j] = ingreso;

        }
    }
    

    for (int i = 0; i <= VERTICES - 1; i++) {
        for (int j = i + 1; j <= VERTICES - 1; j++) {
            if (M_Costos[i][j] != 0) {
                inserta(i, j, M_Costos[i][j], &arbol);  //inserto en cola prior
            }
        }
    }

    printf("\nValores sin aplicar el Algoritmo de Kruskal:\n");
    lista(arbol);


    kruskal(&arbol);

    printf("\nAplicando el algoritmo de Kruskal:\n");
    lista(arbol);

    return 0;
}


 //le asigno un nombre al componente y al vertice y lo agrego al conjunto, como no estan conectados, forman un conjunto por si mismos.

void inicial(tipo_nombre nombre, tipo_elemento i, conjunto_CE *conjuntos) {
    
    (conjuntos->nombres)[i].nombre_conjunto = nombre;
    (conjuntos->nombres)[i].siguiente_elemento = -1; //porque solo tiene un vertice y no sig.
    (conjuntos->encabezamientos_conjunto)[nombre].cuenta = 1;           //indicamos que tiene un elemento
    (conjuntos->encabezamientos_conjunto)[nombre].primer_elemento = i;  //indicamos que el primer elemento del conjunto es el elemento en la posicion i
    
}


//combina dos componentes, formando un nombre nuevo.
void combina(tipo_nombre conjunto_u, tipo_nombre conjunto_v, conjunto_CE *conjuntos) {
    // La cantidad de componentes del conjunto
    int comp_a = (conjuntos->encabezamientos_conjunto)[conjunto_u].cuenta;
    int comp_b = (conjuntos->encabezamientos_conjunto)[conjunto_v].cuenta;
    
    if(comp_a > comp_b){                                    //dependiendo de la cantidad de componentes elijo uno u otro.
        fusionar(conjunto_u, conjunto_v, conjuntos);
    } else {
        fusionar(conjunto_v, conjunto_u, conjuntos);
    }
}

// dados dos componentes, convierto los vertices de la segunda en la del primero.
void fusionar(tipo_nombre conjunto_u, tipo_nombre conjunto_b, conjunto_CE * conjuntos){
   
    tipo_elemento elem = (conjuntos->encabezamientos_conjunto)[conjunto_b].primer_elemento; //indice con el primer elemento del conjunto B
        
        while((conjuntos->nombres)[elem].siguiente_elemento != -1){     // mientras no se haya encontrado el último elemento de la segunda lista
            (conjuntos->nombres)[elem].nombre_conjunto = conjunto_u;    //se cambian los elementos del conjunto_b al del conjunto u
            elem = (conjuntos->nombres)[elem].siguiente_elemento;
        }
        
        (conjuntos->nombres)[elem].nombre_conjunto = conjunto_u;        //el ultimo elemento de conjunto b apunta al primero de u
        (conjuntos->nombres)[elem].siguiente_elemento = (conjuntos->encabezamientos_conjunto)[conjunto_u].primer_elemento;

        //el indice del primer elemento del conjunto u ahora apunta al primer elemento del conjunto b
        (conjuntos->encabezamientos_conjunto)[conjunto_u].primer_elemento = (conjuntos->encabezamientos_conjunto)[conjunto_b].primer_elemento;
        (conjuntos->encabezamientos_conjunto)[conjunto_u].cuenta += (conjuntos->encabezamientos_conjunto)[conjunto_b].cuenta;
}       //actualizo la cantidad de elementos del conjunto u sumandole la cantidad de elementos del conjunto b


//recibe vertice y el conjunto de componentes de aristas y devuelve el nombre del componente al que pertenece el vertice
tipo_nombre encuentra(int vertice, conjunto_CE *conjuntos) {
    
    return ((conjuntos->nombres)[vertice].nombre_conjunto);
}

//imprimo la lista enlazada
void lista(rama * arbol){
    printf("\n");
    rama * temp = arbol;
    while(temp != NULL){
        printf("(%i -> %i con costo: %i) ... ", temp->a.u, temp->a.v,temp->a.costo);
        temp = temp -> sig;
    } 
    printf(" :3\n");
}


//esta funcion recibe los vertices,la arista y su costo para luego ingresarla en la lista enlazada dada
void inserta(int vA, int vB, int costo, rama ** arbol) {        
    rama *nueva_rama = (rama *)malloc(sizeof(rama));
    
    arista nuevaArista;
    nuevaArista.u = vA;
    nuevaArista.v = vB;
    nuevaArista.costo = costo;

    nueva_rama->a = nuevaArista;    //indico que es una nueva rama y la inserto 
    nueva_rama->sig = NULL;

    if(*arbol == NULL) {
        *arbol = nueva_rama;
    } else {                            //si el arbol no esta vacio lo recorro hasta llegar al final
        rama * temp = *arbol;
        while(temp -> sig!= NULL){
            temp = temp -> sig;
        }
        temp->sig = nueva_rama;
    }

}

//devuelve el menor costo de una lista enlazada de aristas
arista sacar_min(rama **arbol) {

    arista costo_min = (*arbol)->a; //obtenemos la primera arista de la lista
    
    rama * temp = *arbol;   //puntero para recorrer el arbol


    while(temp != NULL){
        if(temp->a.costo < costo_min.costo){
            costo_min = temp->a;
        }
        temp = temp->sig;
    }
    temp = *arbol;              //ARISTA DE MENOR COSTO


    //buscamos el nodo que contiene a la arista de menor costo
    if(temp->a.costo == costo_min.costo){
        *arbol = (*arbol)->sig;
        free(temp);                  //si esta en el primer nodo lo borramos, y se actualiza la raiz del arbol
    }else{
        while(temp->sig->a.costo != costo_min.costo){
            temp = temp->sig;
        }
        rama * borrar = temp->sig;
        temp->sig = temp->sig->sig; //al elemento previo del que vamos a borrar lo conectamos con el siguiente
        free(borrar);
    }
    return costo_min;
}

//funcion kruskal que realiza el AEM
void kruskal(rama ** arbol){
    int comp_n, comp_siguiente, comp_u, comp_v;
    tipo_elemento u, v;
    conjunto_CE *componentes = (conjunto_CE*) malloc(sizeof(conjunto_CE));
    rama * resultado = NULL;
    comp_siguiente = 0;
    comp_n = VERTICES;
    arista arista_minima;

   
    for(int i = 0; i<VERTICES; i++){                //por cada vertice generamos uno nuevo sin siguiente.
        inicial(comp_siguiente, i, componentes);
        comp_siguiente++;
    }

    while(comp_n > 1){
        arista_minima = sacar_min(arbol);
        u = arista_minima.u;
        v = arista_minima.v;
        comp_u = encuentra(u, componentes); //se encuentran los componentes y se comprueba que no formen un ciclo
        comp_v = encuentra(v, componentes);

        if(comp_u != comp_v){                               //si no forman un ciclo entonces los une 
            combina(comp_u, comp_v, componentes);
            comp_n--;
            inserta(arista_minima.u, arista_minima.v, arista_minima.costo, &resultado);
        }
    }

    //libero el espacio de memoria del arbol
    rama * temp = *arbol;
    while(temp != NULL){
        *arbol = temp;
        temp = (*arbol)->sig;
        free(*arbol);
    }
    
    //devuelvo el resultado del kruskal
    *arbol = resultado;
}







