#include <iostream>
#include <stdlib.h>
#include <ctime>
#include <cstdlib>
#include <chrono>
#include <iomanip>
#include "Heapbin.h"
#include <fstream>
using namespace std;

//  @author Xukai Chen
int main() {
	Heapbin<int> heap = Heapbin<int>(), heapaux = Heapbin<int>();
	int valores[] = { 40,10,20,30,25,35,15 };
	for (int i = 0; i < 7; i++) {
		heap.insert(valores[i]);
		heapaux.insert(i);
	}
	cout << "Dos monticulos inicializados con siguientes valores, heap1 { 40,10,20,30,25,35,15 } y heap2 { 0,1,2,3,4,5,6 }" << endl;
	cout << "El minimo de heap1 esperado: 10"<< endl <<  heap.min() << endl;
	heap.deleteMin();
	cout << "Borrado del minimo, el nuevo minimo esperado: 15" << endl << heap.min() << endl;
	heap.join(heapaux);
	cout << "Union de ambos monticulos, minimo esperado: 0" << endl << heap.min() << endl;
	heap.deleteMin();
	cout << "Borrado del minimo, minimo esperado: 1" << endl << heap.min() << endl;
	Heapbin<int>::Nodo *nodo = heap.getRaiz()->_right->_down->_down->_down;
	cout << "Un puntero al valor 35 que pertenece a un arbol de grado 3"<< endl << nodo->_elem <<  endl;
	heap.decreaseKey(nodo, 0);
	cout << "Decrece su valor al 0, ahora el valor del puntero pasa a ser de su padre 25 \t el nuevo minimo sera 0" << endl  << nodo->_elem << "\t"<< heap.min() << endl;
	heap.deleteMin();
	cout << "Borrado del minimo, minimo sera 1" << endl << heap.min() << endl;
	heap.liberaHeap();
	return 0;
}
