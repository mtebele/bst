#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include "abb.h"

/*
 * NOMBRE: Matias Tebele | PADRON: 95372
 * NOMBRE: Lucas Perea | PADRON: 95368
 */

/* ******************************************************************
 *                      FUNCIONES AUXILIARES
 * *****************************************************************/

char *strdupl(const char *s)
{
    char *n = malloc(strlen(s) + 1);
    if (n == NULL) return NULL;
    strcpy(n, s);
    return n;
}

/* Función auxiliar para imprimir si estuvo OK o no. */
void print_test(char* name, bool result)
{
    printf("* %s: %s\n", name, result? "OK" : "ERROR");
}

// Extra contiene la dirección de la variable 'suma'
// Devuelve true para seguir iterando
bool sumar_elementos(const char* clave, void* dato, void* extra)
{
	int *suma = (int*)extra;
	(*suma) += atoi(clave);
	return true;
}

/* ******************************************************************
 *                        PRUEBAS UNITARIAS
 * *****************************************************************/

/* Prueba que las primitivas del abb funcionen correctamente. */
void pruebas_abb_vacio()
{
	abb_t* abb = abb_crear(strcmp, NULL);
	char *clave1 = "abd"; 
	
	printf("-- INICIO PRUEBAS ABB VACIO --\n");
	print_test("El abb fue creado", abb);
	print_test("Borrar clave1 del abb es NULL", !abb_borrar(abb, clave1));
	print_test("Obtener clave1 del abb es NULL", !abb_obtener(abb, clave1));	
	print_test("La cantidad del abb es 0", abb_cantidad(abb) == 0);
	print_test("clave1 no pertenece al abb", !abb_pertenece(abb, clave1));

	abb_iter_t * it = abb_iter_in_crear(abb);
	print_test("El iterador fue creado", it);
	print_test("Ver actual es NULL", !abb_iter_in_ver_actual(it));
	print_test("Avanzar es false", !abb_iter_in_avanzar(it));
	print_test("El iterador está al final", abb_iter_in_al_final(it));

	abb_iter_in_destruir(it);
	print_test("El iterador fue destruido", true);
	abb_destruir(abb);
	print_test("El arbol fue destruido", true);
}

void pruebas_abb_iter_automatico()
{
	/*Iterador automático
	 *Creo un nuevo arbol con claves numéricas*/
	abb_t* abb_nuevo = abb_crear(strcmp, NULL);
	char *clave4 = "5", *dato4 = "boca";
	char *clave5 = "3", *dato5 = "river";
	char *clave6 = "7", *dato6 = "velez";

	printf("-- INICIO PRUEBAS ABB ITER AUTOMATICO --\n");
	/*Inserto todos los elementos de manera ordenada*/
	print_test("Guardo dato4-clave4", abb_guardar(abb_nuevo, clave4, dato4));
	print_test("Cantidad de abb es 1", abb_cantidad(abb_nuevo) == 1);
	print_test("Guardo dato5-clave5", abb_guardar(abb_nuevo, clave5, dato5));
	print_test("Cantidad de abb es 2", abb_cantidad(abb_nuevo) == 2);
	print_test("Guardo dato6-clave6", abb_guardar(abb_nuevo, clave6, dato6));
	print_test("Cantidad de abb es 3", abb_cantidad(abb_nuevo) == 3);
	
	int cuenta = 0;
	abb_in_order(abb_nuevo, sumar_elementos, &cuenta);
	
	print_test("La suma de las claves es 15", cuenta == 15);
	
	abb_destruir(abb_nuevo);
	print_test("El arbol fue destruido", true);
}

void pruebas_abb_pocos_elementos()
{
	abb_t* abb = abb_crear(strcmp, NULL);
	char *clave1 = "grande", *dato1 = "boca", *datoA = "peñarol";
	char *clave2 = "chico", *dato2 = "river", *datoB = "nacional";
	char *clave3 = "solo", *dato3 = "velez";

	printf("-- INICIO PRUEBAS ABB POCOS ELEMENTOS --\n");
	/*Inserto todos los elementos de manera ordenada*/
	print_test("Guardo dato2-clave2", abb_guardar(abb, clave2, dato2));
	print_test("Cantidad de abb es 1", abb_cantidad(abb) == 1);
	print_test("Guardo dato1-clave1", abb_guardar(abb, clave1, dato1));
	print_test("Cantidad de abb es 2", abb_cantidad(abb) == 2);
	print_test("Guardo dato3-clave3", abb_guardar(abb, clave3, dato3));
	print_test("Cantidad de abb es 3", abb_cantidad(abb) == 3);
	print_test("clave1 pertenece al abb", abb_pertenece(abb, clave1));
	print_test("clave2 pertenece al abb", abb_pertenece(abb, clave2));
	print_test("clave3 pertenece al abb", abb_pertenece(abb, clave3));

	/*Consulto por ellos*/
	print_test("Obtener dato de clave1 es dato1", abb_obtener(abb, clave1) == dato1);
	print_test("Obtener dato de clave2 es dato2", abb_obtener(abb, clave2) == dato2);
	print_test("Obtener dato de clave2 es dato3", abb_obtener(abb, clave3) == dato3);
	
	/*Reemplazo - Borro*/
	print_test("Guardo datoA-clave1", abb_guardar(abb, clave1, datoA));
	print_test("Cantidad de abb es 3", abb_cantidad(abb) == 3);
	print_test("Obtener dato de clave1 es datoA", abb_obtener(abb, clave1) == datoA);
	print_test("Borro clave2", abb_borrar(abb, clave2));
	print_test("clave2 no pertenece al abb", !abb_pertenece(abb, clave2));
	print_test("Obtener dato de clave2 es NULL", !abb_obtener(abb, clave2));
	print_test("Cantidad de abb es 2", abb_cantidad(abb) == 2);
	print_test("Guardo datoB-clave2", abb_guardar(abb, clave2, datoB));
	print_test("Cantidad de abb es 3", abb_cantidad(abb) == 3);

	/*Iterador manual*/	
	abb_iter_t *it = abb_iter_in_crear(abb);
	
	print_test("Crear iterador", it != NULL);
	print_test("Actual es clave2", strcmp(abb_iter_in_ver_actual(it), clave2) == 0);
	print_test("El iterador no está al final", !abb_iter_in_al_final(it));
	print_test("El iterador avanza", abb_iter_in_avanzar(it));
	print_test("Actual es clave1", strcmp(abb_iter_in_ver_actual(it), clave1) == 0);
	print_test("El iterador no está al final", !abb_iter_in_al_final(it));
	print_test("El iterador avanza", abb_iter_in_avanzar(it));
	print_test("Actual es clave3", strcmp(abb_iter_in_ver_actual(it), clave3) == 0);
	print_test("El iterador no está al final", !abb_iter_in_al_final(it));
	print_test("El iterador avanza", abb_iter_in_avanzar(it));
	print_test("Actual es NULL", !abb_iter_in_ver_actual(it));
	print_test("El iterador está al final", abb_iter_in_al_final(it));
	print_test("El iterador no avanza", !abb_iter_in_avanzar(it));
	
	abb_iter_in_destruir(it);
	print_test("El iterador fue destruido", true);
	abb_destruir(abb);	
	print_test("El arbol fue destruido", true);
}

void pruebas_abb_valor_nulo()
{
	printf("-- INICIO PRUEBAS ABB VALOR NULO --\n");

	abb_t* abb = abb_crear(strcmp, NULL);
	char *clave1 = "abc", *dato = NULL;

	print_test("Guardo NULL-clave1", abb_guardar(abb, clave1, dato));
	print_test("Cantidad de abb es 1", abb_cantidad(abb) == 1);
	print_test("Obtener clave1 es NULL", !abb_obtener(abb, clave1));
	print_test("clave1 pertenece al abb", abb_pertenece(abb, clave1));
	print_test("Borrar clave1 devuelve NULL", !abb_borrar(abb, clave1));
	abb_destruir(abb);
	print_test("El arbol fue destruido", true);
}

void pruebas_abb_pidomem()
{
	abb_t* abb = abb_crear(strcmp, free);

	char *dato1 = strdupl("No");
	char *dato2 = strdupl("te olvides");
	char *dato3 = strdupl("de liberarme");
	char *datoA = strdupl("jamas");

	char *clave1 = "02", *clave2 = "03", *clave3 = "06";

	printf("-- INICIO PRUEBAS PIDIENDO MEMORIA PARA LOS DATOS --\n");

	print_test("Guardo dato1-clave1", abb_guardar(abb, clave1, dato1));
	print_test("Guardo dato2-clave2", abb_guardar(abb, clave2, dato2));
	print_test("Guardo dato3-clave3", abb_guardar(abb, clave3, dato3));
	print_test("Reemplazo datoA-clave1", abb_guardar(abb, clave1, datoA));
	print_test("Cantidad de abb es 3", abb_cantidad(abb) == 3);
	print_test("Borrar clave1 devuelve datoA", abb_borrar(abb, clave1) == datoA);
	abb_destruir(abb);
	print_test("El arbol fue destruido", true);
}

void pruebas_abb_borrar_cantidad()
{
	abb_t* abb = abb_crear(strcmp, NULL);
	
	char *clave1 = "5", *dato1 = "dato1";
	char *clave2 = "3", *dato2 = "dato2";
	char *clave3 = "7", *dato3 = "dato3";

	printf("-- INICIO PRUEBAS ABB BORRAR Y CANTIDAD --\n");
	
	/*Prueba de un nodo solo*/
	print_test("Cantidad de abb es 0", abb_cantidad(abb) == 0);
	print_test("Guardo dato1-clave1", abb_guardar(abb, clave1, dato1));
	print_test("Cantidad de abb es 1", abb_cantidad(abb) == 1);
	print_test("Borrar clave1 devuelve dato1", abb_borrar(abb, clave1) == dato1);
	print_test("Cantidad de abb es 0", abb_cantidad(abb) == 0);
	
	/*Prueba de un nodo con un hijo borrando primero la raiz*/
	print_test("Guardo dato1-clave1", abb_guardar(abb, clave1, dato1));
	print_test("Guardo dato2-clave2", abb_guardar(abb, clave2, dato2));
	print_test("Cantidad de abb es 2", abb_cantidad(abb) == 2);
	print_test("Borrar clave1 devuelve dato1", abb_borrar(abb, clave1) == dato1);
	print_test("Cantidad de abb es 1", abb_cantidad(abb) == 1);
	print_test("Borrar clave2 devuelve dato2", abb_borrar(abb, clave2) == dato2);
	print_test("Cantidad de abb es 0", abb_cantidad(abb) == 0);
	
	/*Prueba de un nodo con un hijo borrando primero el hijo*/
	print_test("Guardo dato1-clave1", abb_guardar(abb, clave1, dato1));
	print_test("Guardo dato2-clave2", abb_guardar(abb, clave2, dato2));
	print_test("Cantidad de abb es 2", abb_cantidad(abb) == 2);
	print_test("Borrar clave2 devuelve dato2", abb_borrar(abb, clave2) == dato2);
	print_test("Cantidad de abb es 1", abb_cantidad(abb) == 1);
	print_test("Borrar clave1 devuelve dato1", abb_borrar(abb, clave1) == dato1);
	print_test("Cantidad de abb es 0", abb_cantidad(abb) == 0);
	
	/*Prueba de un nodo con dos hijos borrando primero la raiz*/
	print_test("Guardo dato1-clave1", abb_guardar(abb, clave1, dato1));
	print_test("Guardo dato2-clave2", abb_guardar(abb, clave2, dato2));
	print_test("Guardo dato3-clave3", abb_guardar(abb, clave3, dato3));
	print_test("Cantidad de abb es 3", abb_cantidad(abb) == 3);
	print_test("Borrar clave1 devuelve dato1", abb_borrar(abb, clave1) == dato1);
	print_test("Cantidad de abb es 2", abb_cantidad(abb) == 2);
	print_test("Borrar clave3 devuelve dato3", abb_borrar(abb, clave3) == dato3);
	print_test("Cantidad de abb es 1", abb_cantidad(abb) == 1);
	print_test("Borrar clave2 devuelve dato2", abb_borrar(abb, clave2) == dato2);
	print_test("Cantidad de abb es 0", abb_cantidad(abb) == 0);
	
	/*Prueba de un nodo con dos hijos borrando primero los hijos*/
	print_test("Guardo dato1-clave1", abb_guardar(abb, clave1, dato1));
	print_test("Guardo dato2-clave2", abb_guardar(abb, clave2, dato2));
	print_test("Guardo dato3-clave3", abb_guardar(abb, clave3, dato3));
	print_test("Cantidad de abb es 3", abb_cantidad(abb) == 3);
	print_test("Borrar clave3 devuelve dato3", abb_borrar(abb, clave3) == dato3);
	print_test("Cantidad de abb es 2", abb_cantidad(abb) == 2);
	print_test("Borrar clave2 devuelve dato2", abb_borrar(abb, clave2) == dato2);
	print_test("Cantidad de abb es 1", abb_cantidad(abb) == 1);
	print_test("Borrar clave1 devuelve dato1", abb_borrar(abb, clave1) == dato1);
	print_test("Cantidad de abb es 0", abb_cantidad(abb) == 0);	
	
	abb_destruir(abb);
	print_test("El arbol fue destruido", true);
}

void pruebas_abb_volumen(size_t largo) 
{
	printf("-- INICIO PRUEBAS ABB DE VOLUMEN --\n");
	
	int largo_lista = largo * 2;
	abb_t* abb = abb_crear(strcmp, NULL);

	const size_t largo_clave = 10;
	char (*claves)[largo_clave] = malloc(largo * largo_clave);

	int* valores[largo];
	int lista[largo_lista];
	int cant_repetidos = 0;
	
	for (int i = 0; i < largo_lista; i++)
		lista[i] = 0;
	srand(time(NULL));

	/* Inserta 'largo' parejas en el abb */
	bool ok = true;
	for (int i = 0; i < largo; i++) {
		valores[i] = malloc(sizeof(int));
		int rand_clave = rand() % (largo * 2);
		int rand_dato = rand() % (largo * 2);
		
		while (lista[rand_clave] != 0) {
			rand_clave = rand() % (largo * 2);
			cant_repetidos++;
		}
		
		sprintf(claves[i], "%04d", rand_clave);
		*valores[i] = rand_dato;
		ok = abb_guardar(abb, claves[i], valores[i]);
		lista[rand_clave] = 1;		
		if (!ok) break;
	}

	print_test("Prueba abb almacenar muchos elementos", ok);
	print_test("Prueba abb la cantidad de elementos es correcta", abb_cantidad(abb) == largo);
	
	/* Verifica que devuelva los valores correctos */	
	for (size_t i = 0; i < largo; i++) {
		ok = abb_pertenece(abb, claves[i]);
		if (!ok) break;
		ok = abb_obtener(abb, claves[i]) == valores[i];
		if (!ok) break;
	}

	print_test("Prueba abb pertenece y obtener muchos elementos", ok);
	print_test("Prueba abb la cantidad de elementos es correcta", abb_cantidad(abb) == largo);

	/* Verifica que borre y devuelva los valores correctos */
	for (size_t i = 0; i < largo; i++) {
		ok = abb_borrar(abb, claves[i]) == valores[i];
		if (!ok) break;
	}

	print_test("Prueba abb borrar muchos elementos", ok);
	print_test("Prueba abb la cantidad de elementos es 0", abb_cantidad(abb) == 0);

	/* Destruye el abb y crea uno nuevo que sí libera */
	abb_destruir(abb);
	
	abb = abb_crear(strcmp, free);

	/* Inserta 'largo' parejas en el abb */
	ok = true;
	for (size_t i = 0; i < largo; i++) {
		ok = abb_guardar(abb, claves[i], valores[i]);
		if (!ok) break;
	}

	free(claves);
	
	/* Destruye el abb - debería liberar los enteros */
	abb_destruir(abb);
	print_test("El arbol fue destruido", true);
}

/* ******************************************************************
 *                        PRINCIPAL
 * *****************************************************************/

/* Programa principal. */
int main(void)
{
	pruebas_abb_vacio();
	pruebas_abb_iter_automatico();
	pruebas_abb_pocos_elementos();
	pruebas_abb_valor_nulo();
	pruebas_abb_pidomem();
	pruebas_abb_borrar_cantidad();
	pruebas_abb_volumen(5000);
	return 0;
}
