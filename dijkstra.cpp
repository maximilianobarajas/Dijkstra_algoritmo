#include <iostream>
#include <locale.h>

/* Programa con el Algoritmo de Dijkstra para encontrar el camino más corto entre dos vértices*/

using namespace std; 

#define MUY_GRANDE 1000000
int **C;
bool *visitado;
int *costoMin, *nodoPrevio;
int origen, destino;
void capturaDirigida( int n ){
  for( int i=0; i<n; i++){
    for ( int j=0; j<n; j++ ){
      cout << "C["<< i << ", "<< j << "]=? ";
      cin >> C[i][j];
      if(C[i][j] == -1 )
           C[i][j] = MUY_GRANDE;
    }
  }
}
void capturaNoDirigida(int n) {
  int i, j;
  for (i = 0; i < n; i++) {
    for (j = i; j < n; j++) {
      if (i == j) {
        C[i][j] = MUY_GRANDE;
      } else {
        cout << "C[" << i << ", " << j << "]=? ";
        cin >> C[i][j];
        if(C[i][j]==-1){
        	C[i][j]=MUY_GRANDE;
		}
		C[j][i] = C[i][j];
      }
    }
  }
}
void alojaMatrizAdy( int n ){
  C = new int *[n];
  for( int i = 0; i < n; i++ ){
    C[i] = new int [n];
  }
}
void desalojaMatriz( int n ){
  for( int i=0; i<n; i++ )
    delete [] C[i];
  delete [] C;
}
void despliegaMatriz(int n) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++)
      cout << "A[" << i << ", " << j << "]= " << C[i][j] << "\t";
    cout << endl;
  }
}
// La ruta solución se guarda en una pila con la siguiente 
// estructura:
struct s_caja{
  int nodo;
  s_caja *sig;
};
s_caja *rutaSolucion; 
void pushNodo( int nodo ){
  s_caja *q;
  q = new s_caja;
  q->nodo = nodo;
  q->sig = rutaSolucion;
  rutaSolucion = q;
}
void guardaRuta( ){
  // Guarda en una pila los nodos previos hasta llegar al nodo origen
   int nodo;
  // se comienza por el nodo destino 
   nodo = destino;
   do{
     pushNodo( nodo );
     nodo = nodoPrevio[nodo]; //se obtiene el nodo previo al actual
   }while( nodo != origen);
   // hasta arriba de la pila va el origen
   pushNodo( origen );  
}
void imprimeRuta( ){
  int nodo;
  s_caja *q;
  q = rutaSolucion;
  do{
    cout << q->nodo << ", ";
    q = q->sig;
  }while( q!= NULL );
  cout << endl;  
}
void inicializaDijkstra( int numNodos ){
  visitado = new bool[numNodos];
  nodoPrevio = new int[numNodos];
  costoMin = new int[numNodos];
  rutaSolucion = NULL;
   // inicializa el vector de nodos visitados
  for ( int i = 0; i < numNodos; i++ ){
      visitado[i] = false; // ninguno visitado
      nodoPrevio[i] = origen; 
      if( i == origen )
         costoMin[i] = 0;
      else
         costoMin[i] = MUY_GRANDE;
  }   
}
int nodoMasBarato(int numNodos ){
   int min = MUY_GRANDE, nodoMin = -1;
   for( int i = 0; i< numNodos; i++ ){  
      if ( !visitado[i] )
        if ( min > costoMin[i] ){
             min = costoMin[i];
             nodoMin = i;
        }
    }       
    return nodoMin; 
}    
void calculaMenorCosto( int numNodos, int intermedio ){
  int i = intermedio;
  for( int j= 0; j<numNodos; j++ ){
  
    if( !visitado[j] ){
      if( costoMin[j] <= costoMin[i] + C[i][j] )
        ; // opción 1 (no se altera el costo mínimo)
      
      else{
        // opción 2 (se elige la otra ruta)
        costoMin[j] = costoMin[i] + C[i][j];
        // se actualiza el nodo previo al nodo j
        nodoPrevio[j] = i; // para llegar al nodo j
               //es más barato pasar antes por el nodo i 
        }
    }
  }
}

void dijkstra( int numNodos ){
  int intermedio;  
  inicializaDijkstra( numNodos );
  
  do{
    intermedio = nodoMasBarato( numNodos );
    visitado[intermedio] = true;
    calculaMenorCosto( numNodos, intermedio );
    
  }while (intermedio != destino);
 
}
int main(int argc, char ** argv) {

  setlocale(LC_CTYPE, "Spanish"); // para poder poner acentos

  int opcion, numNodos;

  cout << "Cuántos nodos tiene tu gráfica?  \n";
  cin >> numNodos;
  alojaMatrizAdy(numNodos);

  do {
    cout << "Elige el tipo de gráfica que vas a capturar: \n";
    cout << " 1: Gráfica dirigida \n" <<
      " 2: Gráfica no dirigida \n";
    cin >> opcion;
  } while (opcion < 1 || opcion > 2);

  cout << "Primero captura la matriz de costos" << endl;
  cout << "indica con: -1 cuando no hay conexión entre dos nodos \n";

  switch (opcion) {
  case 1: {
    capturaDirigida(numNodos);
    break;
  }
  case 2: {
    capturaNoDirigida(numNodos);
    break;
  }
  }

  cout << "La matriz que me diste es: \n";
  despliegaMatriz(numNodos);

  do {
    cout << "Dime cuál es el número del nodo origen:  ";
    cin >> origen;
  } while (origen < 0 || origen >= numNodos);

  do {
    cout << "Dime cuál es el número del nodo destino  ";
    cin >> destino;
  } while (destino < 0 || destino >= numNodos);

  // Encuentra el camino más corto (barato)
  dijkstra(numNodos);

  // guarda en una pila los nodos de la ruta encontrada
  guardaRuta();

  cout << "La trayectoria mas corta del nodo: " << origen <<
    " hasta el nodo: " << destino << " es: \n";
  imprimeRuta();

  cout << "El costo mínimo encontrado es: " << costoMin[destino] << endl;

  cout << "Ahora voy a liberar la memoria. Adios ;)";
  delete[] costoMin;
  delete[] nodoPrevio;
  delete[] visitado;
  desalojaMatriz(numNodos);

  return 0;
}
