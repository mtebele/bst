#ifndef ABB_H
#define ABB_H

#include <stdbool.h>
#include <stddef.h>
#include "pila.h"

/*
 * NOMBRE: Matias Tebele | PADRON: 95372
 * NOMBRE: Lucas Perea | PADRON: 95368
 */

/******************************************************************
 *                DECLARACIÓN DE LOS TIPOS DE DATOS
 ******************************************************************/

typedef struct abb abb_t;
typedef struct abb_iter abb_iter_t;

// Determinará el criterio de comparación entre claves.
// Pre: las cadenas fueron creadas.
// Post: devuelve un valor negativo si la primera clave es mayor, cero si
// son iguales y positivo si el segundo es mayor.
typedef int (*abb_comparar_clave_t) (const char *, const char *);

// Destruye un dato.
// Pre: el dato fue creado.
// Post: Destruye el dato.
typedef void (*abb_destruir_dato_t) (void *);

/******************************************************************
 *                DECLARACIÓN DE LAS PRIMITIVAS
 ******************************************************************/

// Crea un abb.
// Post: Se creó un abb con la funcion comparadora y destructora pasadas a la
// función.
abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato);

// Guarda información en el abb.
// Pre: El abb fue creado.
// Post: Devuelve true si se agrega exitosamente la clave y su dato
// asociado al abb. False en caso contrario.
bool abb_guardar(abb_t *arbol, const char *clave, void *dato);

// Borra un elemento del abb.
// Pre: El abb fue creado.
// Post: Devuelve el dato asociado a la clave recibida (NULL si no se borró o no
// existe clave en el abb). Borra el elemento del abb.
void *abb_borrar(abb_t *arbol, const char *clave);

// Muestra el dato asociado a la clave en un abb.
// Pre: el abb fue creado.
// Post: Devuelve el dato asociado a la clave recibida (NULL si no se borró o no
// existe clave en el abb).
void *abb_obtener(const abb_t *arbol, const char *clave);

// Determina si una clave pertenece al abb.
// Pre: el abb fue creado.
// Post: Devuelve true si la clave está en el abb, false en caso contrario.
bool abb_pertenece(const abb_t *arbol, const char *clave);

// Obtiene la cantidad de elementos en el abb.
// Pre: el abb fue creado.
// Post: Devuelve la cantidad de elementos del abb.
size_t abb_cantidad(abb_t *arbol);

// Destruye el abb.
// Pre: el abb fue creado.
// Post: el abb y todos sus elementos fueron destruidos.
void abb_destruir(abb_t *arbol);

/*************** DECLARACIÓN DE PRIMITIVAS (ITERADORES) ****************/

// Itera automáticamente sobre el abb, operando a medida que recorre el mismo.
// Pre: el abb fue creado.
// Post: Itera sobre el abb aplicando la función visitar por cada elemento visitado.
void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra);

// Crea un iterador del abb.
// Pre: el abb fue creado.
// Post: Devuelve un iterador de abb.
abb_iter_t *abb_iter_in_crear(const abb_t *arbol);

// Avanza el iterador.
// Pre: el iterador fue creado.
// Post: Devuelve true si logra avanzar, false en caso contrario.
bool abb_iter_in_avanzar(abb_iter_t *iter);

// Obtiene la clave actual en la que está el iterador.
// Pre: El iterador fue creado.
// Post: Devuelve la clave actual en la que está el iterador.
const char *abb_iter_in_ver_actual(const abb_iter_t *iter);

// Determina si el iterador está al final.
// Pre: El iterador fue creado.
// Post: Devuelve true si el iterador está al final, false en caso contrario.
bool abb_iter_in_al_final(const abb_iter_t *iter);

// Destruye el iterador.
// Pre: el iterador fue creado.
// Post: el iterador fue destruido.
void abb_iter_in_destruir(abb_iter_t* iter);

#endif // ABB_H
