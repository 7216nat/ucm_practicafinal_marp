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
	std::chrono::time_point<std::chrono::high_resolution_clock> ini, fin;
	ofstream salida;

	double array1[5000] = {};
	for (int k = 0; k < 10000; k++) {
		Heapbin<int> heap = Heapbin<int>();
		for (int i = 0; i < 5000; i++) {
			ini = std::chrono::high_resolution_clock::now();
			heap.insert(rand() % 5000);
			fin = std::chrono::high_resolution_clock::now();
			double time = std::chrono::duration_cast<std::chrono::nanoseconds>(fin - ini).count();
			array1[i] += time / 1000.0;
		}
		heap.liberaHeap();
	}

	double array2[5000] = {};
	for (int k = 0; k < 1000; k++) {
		Heapbin<int> heap = Heapbin<int>();
		for (int i = 0; i < 5000; i++) {
			heap.insert(rand() % 5000);
		}
		for (int j = 0; j < 5000; j++) {
			ini = std::chrono::high_resolution_clock::now();
			heap.deleteMin();
			fin = std::chrono::high_resolution_clock::now();
			double time = std::chrono::duration_cast<std::chrono::nanoseconds>(fin - ini).count();
			array2[j] += time / 1000.0;
		}
		heap.liberaHeap();
	}

	double array3[5000] = {};
	for (int i = 0; i < 10; i++) {
		for (int j = 1; j <= 5000; j++) {
			Heapbin<int> heap = Heapbin<int>();
			Heapbin<int> heapaux = Heapbin<int>();
			for (int k = 0; k < j; k++) {
				heap.insert(rand() % 5000);
				heapaux.insert(rand() % 5000);
			}
			ini = std::chrono::high_resolution_clock::now();
			heap.join(heapaux);
			fin = std::chrono::high_resolution_clock::now();
			double time = std::chrono::duration_cast<std::chrono::nanoseconds>(fin - ini).count();
			array3[j] += time / 1000.0;
			heap.liberaHeap();
		}
	}

	salida.open("salida.txt", ios::out);
	if (salida.is_open()) {
		for (int i = 0; i < 5000; i++) {
			salida << i << "\t" << setprecision(10) << fixed << array1[i] / 10000.0 << "\t" << array2[4999 - i] / 1000.0 << "\t" << array3[i] / 10.0 << endl;
		}
	}
	salida.close();
	return 0;
}