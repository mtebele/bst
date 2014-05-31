#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "abb.h"

/*
 * NOMBRE: Matias Tebele | PADRON: 95372
 * NOMBRE: Lucas Perea | PADRON: 95368
 */

/******************************************************************
 *                IMPLEMENTACION DE LOS TIPOS DE DATOS
 ******************************************************************/

typedef struct nodo_abb
{
    const char* clave;
    void* dato;
	struct nodo_abb* hijoIzq;
	struct nodo_abb* hijoDer;

} nodo_abb_t;

struct abb
{
	nodo_abb_t* raiz;
	size_t cantidad;
	abb_comparar_clave_t comparar;
	abb_destruir_dato_t destruir_dato;
};

struct abb_iter
{
	pila_t* pila;
};

/******************************************************************
 *                		FUNCIONES AUXILIARES
 ******************************************************************/

// Clona una cadena con memoria dinámica.
char *strdup(const char *s)
{
    char *n = malloc(strlen(s) + 1);
    if (n == NULL) return NULL;
    strcpy(n, s);
    return n;
}

// Devuelve el mínimo nodo (el más izquierdo) y setea al parámetro
// 'padre' recibido con el padre del mínimo encontrado.
nodo_abb_t *abb_obtener_minimo(nodo_abb_t *raiz, nodo_abb_t **padre)
{
	nodo_abb_t* min = raiz;
	while (min->hijoIzq) {
		*padre = min;
		min = min->hijoIzq;
	}
	return min;
}

// Crea un nodo con la clave y el dato.
nodo_abb_t *nodo_abb_crear(const char *clave, void *dato)
{
	nodo_abb_t* nodo = malloc(sizeof(nodo_abb_t));
	if (!nodo) return NULL;
	nodo->clave = strdup(clave);
	nodo->dato = dato;
	nodo->hijoIzq = NULL;
	nodo->hijoDer = NULL;
	return nodo;	
}

// Destruye un nodo. Si destruir_hijos es true, entonces destruye todos hijos.
void nodo_abb_destruir(nodo_abb_t *raiz, abb_destruir_dato_t destruir_dato, bool destruir_hijos)
{
	if (!raiz) return;	
	
	if (destruir_hijos) {
		nodo_abb_destruir(raiz->hijoIzq, destruir_dato, destruir_hijos);
		nodo_abb_destruir(raiz->hijoDer, destruir_dato, destruir_hijos);
	}

	free((char*)raiz->clave);
	if (destruir_dato)
		destruir_dato(raiz->dato);	
	free(raiz);
}

// Guarda un nodo en el árbol.
// -1: error | 0: reemplazado | 1: guardado
int nodo_abb_guardar(nodo_abb_t *nodo, nodo_abb_t *padre, abb_comparar_clave_t comparar, const char *clave, void *dato, abb_destruir_dato_t destruir)
{
	if (!nodo) {
		nodo_abb_t *nodo_nuevo = nodo_abb_crear(clave, dato);
		if (!nodo_nuevo) return -1;
		nodo = nodo_nuevo;
		if (comparar(padre->clave, clave) > 0)
			padre->hijoIzq = nodo;
		else
			padre->hijoDer = nodo;
		return 1;
	}

	int comp = comparar(nodo->clave, clave);
	if (comp > 0)
		return nodo_abb_guardar(nodo->hijoIzq, nodo, comparar, clave, dato, destruir);
	if (comp < 0)
		return nodo_abb_guardar(nodo->hijoDer, nodo, comparar, clave, dato, destruir);
	if (destruir)
		destruir(nodo->dato);
	nodo->dato = dato;
	return 0;
}

// Busca un nodo por clave y lo devuelve.
nodo_abb_t *nodo_abb_buscar(nodo_abb_t *raiz, abb_comparar_clave_t comparar, const char *clave)
{
	if (!raiz) return NULL;
	int comp = comparar(raiz->clave, clave);
	if (comp > 0)
		return nodo_abb_buscar(raiz->hijoIzq, comparar, clave);
	if (comp < 0)
		return nodo_abb_buscar(raiz->hijoDer, comparar, clave);
	return raiz;
}

// Borra un nodo y devuelve el dato asociado al mismo.
void *nodo_abb_borrar(abb_t* abb, nodo_abb_t *nodo, nodo_abb_t *padre, const char *clave, int* error)
{
	if (!nodo) {
		*error = -1;
		return NULL;
	}
	int comp = abb->comparar(nodo->clave, clave);
	if (comp > 0)
		return nodo_abb_borrar(abb, nodo->hijoIzq, nodo, clave, error);
	if (comp < 0)
		return nodo_abb_borrar(abb, nodo->hijoDer, nodo, clave, error);
	
	void* dato = NULL;
	
	// Caso en el que no tiene subárboles
	if (!nodo->hijoIzq && !nodo->hijoDer) {
		if (padre) {
			if (abb->comparar(padre->clave, clave) > 0)
				padre->hijoIzq = NULL;
			else
				padre->hijoDer = NULL;
		}
		else
			abb->raiz = NULL;
	}
	// Caso en el que tiene ambos subárboles
	else if (nodo->hijoIzq && nodo->hijoDer) {
		nodo_abb_t* min_padre = nodo;
		nodo_abb_t* min = abb_obtener_minimo(nodo->hijoDer, &min_padre);
		dato = nodo->dato;
		free((char*)nodo->clave);
		nodo->dato = min->dato;
		nodo->clave = strdup(min->clave);
		nodo_abb_borrar(abb, min, min_padre, min->clave, error);
		return dato;
	}
	// Caso en el que tiene subárbol derecho o izquierdo
	else {
		nodo_abb_t* aux = (!nodo->hijoDer) ? nodo->hijoIzq : nodo->hijoDer;
		if (padre) {
			if (abb->comparar(padre->clave, clave) > 0)
				padre->hijoIzq = aux;
			else
				padre->hijoDer = aux;
		}
		else
			abb->raiz = aux;
	}
	
	dato = nodo->dato;
	nodo_abb_destruir(nodo, abb->destruir_dato, false);
	return dato;
}

// Recorre el árbol in-order hasta que la función 'visitar' devuelva false,
// o bien no haya más nodos que procesar.
void nodo_abb_in_order(nodo_abb_t *raiz, bool visitar(const char *, void *, void *), void *extra)
{
	if (!raiz) return;
	nodo_abb_in_order(raiz->hijoIzq, visitar, extra);
	if (!visitar(raiz->clave, raiz->dato, extra)) return;
	nodo_abb_in_order(raiz->hijoDer, visitar, extra);
}

/******************************************************************
 *                IMPLEMENTACION DE LAS PRIMITIVAS
 ******************************************************************/
 
abb_t* abb_crear(abb_comparar_clave_t comparar, abb_destruir_dato_t destruir_dato)
{
	abb_t* abb = malloc(sizeof(abb_t));
	if (abb == NULL) return NULL;
	abb->raiz = NULL;
	abb->cantidad = 0;
	abb->comparar = comparar;
	abb->destruir_dato = destruir_dato;
	return abb;
}
 
bool abb_guardar(abb_t *arbol, const char *clave, void *dato)
{
	if (!arbol) return false;
	if (!arbol->raiz) {
		nodo_abb_t *nodo = nodo_abb_crear(clave, dato);
		if (!nodo) return false;
		arbol->raiz = nodo;
		arbol->cantidad++;
		return true;
	}
	int guardado = nodo_abb_guardar(arbol->raiz, NULL, arbol->comparar, clave, dato, arbol->destruir_dato);
	if (guardado == 1)
		arbol->cantidad++;
	return (guardado != -1) ? true : false;
}

void *abb_borrar(abb_t *arbol, const char *clave)
{
	if (!arbol || !arbol->raiz) return NULL;
	int error = 0;
	void* dato = nodo_abb_borrar(arbol, arbol->raiz, NULL, clave, &error);
	if (error != -1)
		arbol->cantidad--;
	return dato;
}

void *abb_obtener(const abb_t *arbol, const char *clave)
{
	nodo_abb_t* nodo = nodo_abb_buscar(arbol->raiz, arbol->comparar, clave);
	return (nodo) ? nodo->dato : NULL;
}

bool abb_pertenece(const abb_t *arbol, const char *clave)
{
	return nodo_abb_buscar(arbol->raiz, arbol->comparar, clave) != NULL;
}

size_t abb_cantidad(abb_t *arbol)
{
	return arbol->cantidad;
}

void abb_destruir(abb_t *arbol)
{
	nodo_abb_destruir(arbol->raiz, arbol->destruir_dato, true);
	free(arbol);
}

/******************************************************************
 *           IMPLEMENTACION DE LAS PRIMITIVAS (ITERADORES)
 ******************************************************************/

void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra)
{
	if (!arbol) return;
	nodo_abb_in_order(arbol->raiz, visitar, extra);
}

abb_iter_t *abb_iter_in_crear(const abb_t *arbol)
{
	if (!arbol) return NULL;
	abb_iter_t* abb_iter = malloc(sizeof(abb_iter_t));
	if (!abb_iter) return NULL;
	abb_iter->pila = pila_crear();
	if (!abb_iter->pila) {
		free(abb_iter);
		return NULL;
	}
	// Apilo la raiz y todos los hijos izquierdos
	nodo_abb_t* nodo = arbol->raiz;
	while (nodo) {
		pila_apilar(abb_iter->pila, nodo);
		nodo = nodo->hijoIzq;
	}
	return abb_iter;
}

bool abb_iter_in_avanzar(abb_iter_t *iter)
{
	if (!iter || abb_iter_in_al_final(iter)) return false;
	// Desapilo
	nodo_abb_t* nodo = pila_desapilar(iter->pila);
	// Apilo derecho y todos los hijos izquierdos
	nodo = nodo->hijoDer;
    while (nodo) {
		pila_apilar(iter->pila, nodo);
		nodo = nodo->hijoIzq;
	}
    return true;
}

const char *abb_iter_in_ver_actual(const abb_iter_t *iter)
{
	nodo_abb_t* nodo = pila_ver_tope(iter->pila);
	if (!nodo) return NULL;
	return nodo->clave;
}

bool abb_iter_in_al_final(const abb_iter_t *iter)
{
	return pila_esta_vacia(iter->pila);
}

void abb_iter_in_destruir(abb_iter_t* iter)
{
	pila_destruir(iter->pila);
	free(iter);
}
