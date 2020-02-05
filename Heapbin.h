#ifndef __HEAPBIN_H
#define __HEAPBIN_H

#include <cstddef>

// codigo fuente tomado de los materiales de apoyo de la asignatura de EDA
#include "pila.h"

using namespace std;

/// Excepción generada por algunos métodos
class EHeapbinVacio {};

/**
 Implementación del TAD Monticulo binomial utilizando arbol.

 Las operaciones son:

 publicas:
 - join: Heapbin -> Void. Modificadora
 - min: -> Elem. Observadora.
 - insert: Elem -> Void. Modificadora.
 - decreaseKey: Nodo, Elem -> Void. Modificadora.
 - deleteMin: -> Void. Modificadora.
 - liberaHeap: -> Void. Modificadora.
 privatas:
 - libera: Nodo -> Void. Modificadora
 - merge: Nodo, Nodo -> Nodo. Modificadora

 @author Xukai Chen
 */

template <class T>
class Heapbin {
public:
	class Nodo {
	public:
		/** Constructor por defecto
		*/
		Nodo() : _up(NULL), _down(NULL), _right(NULL), _degree(0) {}

		/** Constructor; construye un nodo para un árbol unitario (sin
		hijo izquierdo, sin hijo derecho)

		@param el valor a alojar en el nodo
		*/
		Nodo(const T &elem) : _elem(elem), _up(NULL), _down(NULL), _right(NULL), _degree(0) {}

		/** Constructor; construye un nodo para un árbol con
		hijo izquierdo e hijo derecho

		@param iz,  el hijo izquierdo
		@param elem, el valor a alojar en la raíz
		@param dr, el hijo derecho
		*/
		Nodo(Nodo *up, const T &elem, Nodo *down, Nodo* right) :
			_elem(elem), _up(up), _down(down), _right(right), _degree(0) {}

		/** Contabiliza una nueva referencia al nodo
		*/
		void addDegree() { _degree++; }

		void linkTwo(Nodo *nodo) {
			this->addDegree();
			nodo->_up = this;
			nodo->_right = this->_down;
			this->_down = nodo;
		}

		T _elem;         // valor del nodo
		Nodo* _up;         // padre
		Nodo* _down;       // hijo izquierdo
		Nodo* _right;      // hijo derecho
		int _degree;      // número de grados
	};

	/** Constructor; operación monticulo vacio */
	Heapbin() {
		_ra = NULL;
		_min = NULL;
	};
	/** Constructor; operacion constructora (construye un monticulo con un puntero a la raíz del
	conjunto de arbolores binarios y otro al aquel arbol que contenga el minimo valor)
	
	@param raiz puntero al nodo de la raiz
	@param min puntero al nodo del arbol que tiene raiz como el minimo de la estructura
	*/
	Heapbin(Nodo *raiz, Nodo *min) {
		_ra = raiz;
		_min = min;
	};

	/**Constructor; operacion monticulo simple (construye un monticulo con un unico arbol binario
	y por lo tanto cuya raiz es el minimo valor)

	@param raiz puntero al nodo de la raiz
	*/
	Heapbin(Nodo *raiz) {
		_ra = raiz;
		_min = raiz;
	};

	/** metodo modificador
	funcion: dado otro monticulo hace una union con el monticulo invocador

	@param h2 monticulo a unir
	*/
	void join(Heapbin h2) {
		_min = NULL;

		/* mergesort entre los dos monticulos, los arboles estan ordenados por su grado de orden creciente*/
		_ra = merge(_ra, h2._ra);

		if (_ra != NULL) {

			/** Existen 3 casos despues de realizar el mergesort:
				1) un unico arbol binario para un grado		situado en curr
				2) dos arboles binarios para un grado		situado en curr y next
				3) tres arboles binarios para grado k, uno de cada monticulo y el otro de haber enlazado dos arboles de grado k-1	situados en curr,next y next->_right
				por lo tanto necesitamos 3 para ir comprobando las anteriores posibilidades*/
			Nodo *prev = NULL, *curr = _ra, *next = curr->_right;
			_min = curr;
			while (next != NULL) {
				// caso 3 y 1
				if (curr->_degree != next->_degree || (next->_right != NULL && next->_right->_degree == curr->_degree)) {
					prev = curr;
					curr = next;
				}
				// caso 2
				else {
					if (curr->_elem <= next->_elem) {
						curr->_right = next->_right;
						curr->linkTwo(next);
					}
					else {
						if (prev == NULL) {
							_ra = next;
						}
						else {
							prev->_right = next;
						}
						next->linkTwo(curr);
						curr = next;
					}
				}

				// el <= es para los casos de claves repetidas
				if (curr->_elem <= _min->_elem) {
					_min = curr;
				}
				next = curr->_right;
			}
		}
	}

	/**Devuelve el valor del nodo apuntado por _min
	*/
	const T& min() {
		if (_ra == NULL) {
			throw EHeapbinVacio();
		}
		return _min->_elem;
	}

	Nodo *&getRaiz() {
		return _ra;
	}

	/**Insertar un valor en la estructura

	@param x el valor a insertar
	*/
	void insert(const T& x) {

		/**Se inserta como un arbol de grado 0 al principio sustituyendo al raiz actual
		*/
		Nodo* newB0 = new Nodo(x);
		newB0->_right = _ra;
		_ra = newB0;
		if (_min != NULL && x <= _min->_elem) {
			_min = newB0;
		}

		/**Se comporta como el metodo join, requiere una reorganizacion de toda estructura en el peor caso
		*/
		Nodo *prev = NULL, *curr = _ra, *next = curr->_right;
		while (next != NULL && curr->_degree == next->_degree) {
			if (curr->_elem <= next->_elem) {
				curr->_right = next->_right;
				curr->linkTwo(next);
			}
			else {
				if (prev == NULL) {
					_ra = next;
				}
				else {
					prev->_right = next;
				}
				next->linkTwo(curr);
				curr = next;
			}
			next = curr->_right;
		}
		if (_min == NULL) {
			_min = newB0;
		}
	}

	/** Decrecer una clave dado un puntero al nodo que la contiene

	@param nodo  puntero al nodo que hay que modificar
	@param key  el valor que sustituira al actual
	*/
	void decreaseKey(Nodo* nodo, const T& key) {
		int aux;
		nodo->_elem = key;
		Nodo* curr = nodo, *parent = curr->_up;

		/** el nuevo valor se ira comprobando su relacion con su padre hasta quedar 
			en una relacion buena
		*/
		while (parent != NULL && curr->_elem < parent->_elem) {
			aux = parent->_elem;
			parent->_elem = curr->_elem;
			curr->_elem = aux;
			curr = parent;
			parent = parent->_up;
		}
		if (parent == NULL && curr->_elem < _min->_elem) {
			_min = curr;
		}
	}

	/** metodo modificador
	funcion: borra el nodo minimo

	*/
	void deleteMin() {
		/** primer recorrido para encontrar el arbol al que esta colgado el arbol que contiene el minimo
			1) o bien es el primer arbol
			2) o no es el primer arbol
		*/
		Nodo *curr = _ra, *ant = NULL;
		if (_ra == NULL) {
			throw EHeapbinVacio();
		}
		while (curr != _min) {
			ant = curr;
			curr = ant->_right;
		}
		// caso 1
		if (ant == NULL) {
			_ra = curr->_right;
		}
		// caso 2
		else {
			ant->_right = curr->_right;
		}
		/** tras descolgado el arbol objetivo, se libera la memoria del nodo minimo, y se cuelga todos sus hijos al monticulo principal
			1) grado del arbol == 0
			2) grado del arbol != 0
		*/
		// caso 2
		if (curr->_degree > 0) {
			Nodo *it = curr->_down;
			
			// Se pone su hermano y descendiente a NULL para poder liberar su memoria
			curr->_down = NULL;
			curr->_right = NULL;
			libera(curr);

			/** Dada una estructura de pila
				Se va apilando los hijos del nodo descolgado para obtener luego un monticulo inverso al como estaban antes
				izquierda->derecha pasa a derecha->izquierda
				con esto facilita al mergesort por como esta implementado
			*/
			Pila<Nodo*> pila = Pila<Nodo*>();
			do {
				it->_up = NULL;
				pila.apila(it);
				it = it->_right;
			} while(it != NULL);

			/**
				Se va ajustando por orden inverso los hermanos(_right)
				el arbol que se encuentra al final, su hermano sera NULL
			*/
			Nodo *raiz = pila.cima();
			it = raiz;
			pila.desapila();
			while (!pila.esVacia()) {
				it->_right = pila.cima();
				it = it->_right;
				pila.desapila();
			}
			it->_right = NULL;

			// Se construye el monticulo y hace una union con el monticulo modificado
			Heapbin<int> heap1 = Heapbin<int>(raiz);
			join(heap1);
		}
		// caso 1
		else {
			_ra = curr->_right;
			curr->_right = NULL;
			libera(curr);
			Heapbin<int> heapVacio = Heapbin<int>();
			join(heapVacio);
		}
	}

	/** Destructor
	*/
	void liberaHeap() {
		libera(_ra);
		_ra = NULL;
		_min = NULL;
	};

private:

	/** Liberación de la estructura de nodos, teniendo
	en cuenta la compartición.

	@param n, puntero a la raíz de la estructura a liberar
	*/
	static void libera(Nodo *n) {
		if (n != NULL) {
			if (n->_up == NULL) {
				if (n->_degree == 0) {
					libera(n->_right);
					delete n;
				}
				else {
					libera(n->_down);
					libera(n->_right);
					delete n;
				}
			}
			else {
				if (n->_degree > 0) {
					libera(n->_down);
					libera(n->_right);
					delete n;
				}
				else {
					delete n;
				}
			}
		}
	}
	
	/** Mergesort de dos estructuras de monticulos

	@param ra1 raiz de la estructura
	@param ra2 raiz de la estructura
	*/
	static Nodo *merge(Nodo *ra1, Nodo *ra2) {
		Nodo *ra = NULL;
		Nodo *curr1 = ra1, *curr2 = ra2;
		Nodo *curr_act = NULL;

		// En caso de que uno de las dos raices sea vacia
		if (ra1 == NULL || ra2 == NULL) {
			return ra1 == NULL ? ra2 : ra1;
		}

		// Mientras ambos monticulos no han terminado su recorrido
		do {

			// caso posterior
			if (ra != NULL) {

				/** condicion: si el monticulo 2 ha terminado su recorrido OR (el monticulo 1 no ha terminado AND 
					grado de monticulo 1 <= grado de monticulo 2)
				*/
				if (curr2 == NULL || (curr1 != NULL && curr1->_degree <= curr2->_degree)) {
					curr_act->_right = curr1;
					curr1 = curr1->_right;
				}
				else {
					curr_act->_right = curr2;
					curr2 = curr2->_right;
				}
				curr_act = curr_act->_right;
			}

			// caso inicial
			else {
				if (curr1->_degree <= curr2->_degree) {
					ra = curr1;
					curr1 = curr1->_right;
				}
				else {
					ra = curr2;
					curr2 = curr2->_right;
				}
				curr_act = ra;
			}
		} while (curr1 != NULL || curr2 != NULL);
		return ra;
	}

	Nodo* _ra; // puntero al nodo raíz
	Nodo* _min; //puntero al minimo
};



#endif

