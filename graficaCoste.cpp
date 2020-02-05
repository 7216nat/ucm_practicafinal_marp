#include <iostream>
#include <stdlib.h>
#include <ctime>
#include <cstdlib>
#include <chrono>
#include <iomanip>
#include "Heapbin_conCoste.h"
#include <fstream>
using namespace std;

//  @author Xukai Chen
int main() {
	srand(time(NULL));
	ofstream salida;

	long array1[5000] = {};
	for (int k = 0; k < 10000; k++) {
		Heapbin<int> heap = Heapbin<int>();
		for (int i = 0; i < 5000; i++) {
			array1[i] += heap.insert(rand() % 5000);
		}
		heap.liberaHeap();
	}

	long array2[5000] = {};
	for (int k = 0; k < 1000; k++) {
		Heapbin<int> heap = Heapbin<int>();
		for (int i = 0; i < 5000; i++) {
			heap.insert(rand() % 5000);
		}
		for (int j = 0; j < 5000; j++) {
			array2[j] += heap.deleteMin();
		}
		heap.liberaHeap();
	}

	long array3[5000] = {};
	for (int i = 0; i < 10; i++) {
		for (int j = 1; j <= 5000; j++) {
			Heapbin<int> heap = Heapbin<int>();
			Heapbin<int> heapaux = Heapbin<int>();
			for (int k = 0; k < j; k++) {
				heap.insert(rand() % 5000);
				heapaux.insert(rand() % 5000);
			}
			array3[j-1] += heap.join(heapaux);
			heap.liberaHeap();
		}
	}

	salida.open("salida.txt", ios::out);
	if (salida.is_open()) {
		for (int i = 0; i < 5000; i++) {
			salida << i << "\t" << setprecision(10) << fixed << array1[i] / 10000 << "\t" << array2[4999 - i] / 1000 << "\t" << array3[i] / 10 << endl;
		}
	}
	salida.close();
	return 0;
}