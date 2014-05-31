#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "pila.h"

#define TAM_BASE 8 // Uso 2^N

/*
 * NOMBRE: Matias Tebele | PADRON: 95372
 * NOMBRE: Lucas Perea | PADRON: 95368
 */

/******************************************************************
 *                IMPLEMENTACION DE LOS TIPOS DE DATOS
 ******************************************************************/

struct _pila {
    void** datos;
    size_t tamanio;
    size_t cantidad;
};

/*******************************************************************
 *                        IMPLEMENTACION
 ******************************************************************/

// Redimensiona una pila.
// Post: devuelve verdadero y redimensiona la pila si se reservó
// exitosamente, devuelve falso en caso contrario.
bool pila_redimensionar(pila_t *pila, size_t tamanio)
{
	void* aux = realloc(pila->datos, tamanio * sizeof(void*));
	if (aux == NULL) return false;
	pila->datos = aux;
	pila->tamanio = tamanio;
	return true;
} 

// Crea una pila.
// Post: devuelve una nueva pila vacía.
pila_t* pila_crear()
{
	pila_t* pila = malloc(sizeof(pila_t));
	if (pila == NULL) return NULL;
	pila->datos = malloc(TAM_BASE * sizeof(void*));
	if (pila->datos == NULL)
	{
		free(pila);
		return NULL;
	}
	pila->cantidad = 0;
	pila->tamanio = TAM_BASE;
	return pila;
}

// Destruye la pila.
// Pre: la pila fue creada.
// Post: se eliminaron todos los elementos de la pila.
void pila_destruir(pila_t *pila)
{
	free(pila->datos);
	free(pila);
}

// Devuelve verdadero o falso, según si la pila tiene o no elementos apilados.
// Pre: la pila fue creada.
bool pila_esta_vacia(const pila_t *pila)
{
	return (pila->cantidad == 0);
}

// Agrega un nuevo elemento a la pila. Devuelve falso en caso de error.
// Pre: la pila fue creada.
// Post: se agregó un nuevo elemento a la pila, valor es el nuevo tope.
bool pila_apilar(pila_t *pila, void* valor)
{
	// Si me quedo sin lugar, reservo más memoria.
	if (pila->cantidad >= pila->tamanio)
		if (!pila_redimensionar(pila, pila->tamanio * 2)) return false;
	pila->datos[pila->cantidad++] = valor;
	return true;
}

// Obtiene el valor del tope de la pila. Si la pila tiene elementos,
// se devuelve el valor del tope. Si está vacía devuelve NULL.
// Pre: la pila fue creada.
// Post: se devolvió el valor del tope de la pila, cuando la pila no está
// vacía, NULL en caso contrario.
void* pila_ver_tope(const pila_t *pila)
{
	return (pila->cantidad == 0) ? NULL : pila->datos[pila->cantidad-1];
}

// Saca el elemento tope de la pila. Si la pila tiene elementos, se quita el
// tope de la pila, y se devuelve ese valor. Si la pila está vacía, devuelve 
// NULL.
// Pre: la pila fue creada.
// Post: si la pila no estaba vacía, se devuelve el valor del tope anterior 
// y la pila contiene un elemento menos.
void* pila_desapilar(pila_t *pila)
{
	void* val = pila_ver_tope(pila);
	if (val == NULL) return NULL;
	pila->cantidad--;
	// Si estoy usando menos de un cuarto del tamaño total y el tamaño
	// es al menos el doble del TAM_BASE, reduzco el tamaño a la mitad.
	if (pila->cantidad < (pila->tamanio / 4) && (pila->tamanio / 2) >= TAM_BASE)		
		if (!pila_redimensionar(pila, pila->tamanio / 2)) return NULL;
	return val;
}
