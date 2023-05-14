#include "pa2m.h"
#include "src/abb.h"
#include "src/abb_estructura_privada.h"
#include <stdio.h>

typedef struct cosa {
	int clave;
	char contenido[10];
} cosa_t;

cosa_t *crear_cosa(int clave)
{
	cosa_t *cosa = (cosa_t *)malloc(sizeof(cosa_t));
	if (cosa)
		cosa->clave = clave;
	return cosa;
}

void destruir_cosa(cosa_t *c)
{
	free(c);
}

void destructor(void *c)
{
	cosa_t *cosa = c;
	if (cosa)
		destruir_cosa(c);
}

int comparador(void *c1, void *c2)
{
	if (!c1 || !c2)
		return -1;
	cosa_t *cosa1 = c1;
	cosa_t *cosa2 = c2;
	return cosa1->clave - cosa2->clave;
}

bool foo_contadora()
{
	return true;
}

bool foo_contadora_parcial(void *elemento, void *extra)
{
	extra = extra;
	if (elemento) {
		if (((cosa_t *)elemento)->clave == 5)
			return false;
	}
	return true;
}

void crear_un_arbol_con_null_devuelve_null()
{
	pa2m_afirmar(abb_crear(NULL) == NULL,
		     "no puedo crear un arbol con NULL");
}

void creo_un_arbol_y_no_explota()
{
	abb_t *abb = abb_crear(comparador);
	pa2m_afirmar(abb != NULL, "Puedo crear un arbol");
	abb_destruir(abb);
}

void creo_un_arbol_y_su_tamanio_esta_vacio()
{
	abb_t *abb = abb_crear(comparador);
	pa2m_afirmar(abb_tamanio(abb) == 0,
		     "El arbol recien creado esta vacio");
	abb_destruir(abb);
}

void pruebas_de_creacion()
{
	crear_un_arbol_con_null_devuelve_null();
	creo_un_arbol_y_no_explota();
	creo_un_arbol_y_su_tamanio_esta_vacio();
}

void pruebas_insercion_de_un_elemento()
{
	pa2m_afirmar(!abb_insertar(NULL, 0),
		     "No puedo insertar en un arbol inexistente");

	cosa_t *a = crear_cosa(3);

	abb_t *abb = abb_crear(comparador);

	pa2m_afirmar(abb_insertar(abb, a) != NULL,
		     "Puedo insertar un elemento");
	pa2m_afirmar(abb_tamanio(abb) == 1,
		     "El tamanio del arbol es el correcto");
	pa2m_afirmar(abb->nodo_raiz->elemento == a,
		     "El elemento insertado es el correcto");
	pa2m_afirmar(!abb->nodo_raiz->derecha && !abb->nodo_raiz->izquierda,
		     "Un arbol de un solo elemento no tiene hijos");

	abb_destruir_todo(abb, destructor);
}

void pruebas_insercion_varios_elementos()
{
	cosa_t *a = crear_cosa(3);
	cosa_t *b = crear_cosa(5);
	cosa_t *c = crear_cosa(1);
	cosa_t *d = crear_cosa(2);

	abb_t *abb = abb_crear(comparador);
	pa2m_afirmar(abb_vacio(abb), "El arbol recien creado esta vacio");
	printf("Inserto el primer elemento \n");
	abb_insertar(abb, a);
	pa2m_afirmar(
		abb->nodo_raiz->elemento == a,
		"El primer elemento insertado esta en la correcta posicion");
	pa2m_afirmar(abb_tamanio(abb) == 1 && !abb_vacio(abb),
		     "el tamanio del arbol es 1");
	printf("Inserto el segundo elemento \n");
	abb_insertar(abb, b);

	pa2m_afirmar(
		abb->nodo_raiz->derecha->elemento == b,
		"El segundo elemento insertado esta en la correcta posicion");
	pa2m_afirmar(abb_tamanio(abb) == 2, "el tamanio del arbol es 2");
	printf("Inserto el tercer elemento \n");
	abb_insertar(abb, c);
	pa2m_afirmar(
		abb->nodo_raiz->izquierda->elemento == c,
		"El tercer elemento insertado esta en la correcta posicion");
	pa2m_afirmar(abb_tamanio(abb) == 3, "el tamanio del arbol es 3");
	printf("Inserto el cuarto elemento \n");
	abb_insertar(abb, d);
	pa2m_afirmar(
		abb->nodo_raiz->izquierda->derecha->elemento == d,
		"El cuarto elemento insertado esta en la correcta posicion");
	pa2m_afirmar(abb_tamanio(abb) == 4, "el tamanio del arbol es 4");

	abb_destruir_todo(abb, destructor);
}

void pruebas_quitar_hoja()
{
	cosa_t *a = crear_cosa(3);
	cosa_t *b = crear_cosa(5);

	abb_t *abb = abb_crear(comparador);
	abb_insertar(abb, a);
	pa2m_afirmar(
		abb_quitar(abb, a) == a,
		"Quito la raiz de un arbol de un solo elemento y me devuelve el elemento correcto");
	pa2m_afirmar(abb_tamanio(abb) == 0,
		     "El arbol esta vacio y su tamanio es cero");
	abb_insertar(abb, a);
	abb_insertar(abb, b);
	pa2m_afirmar(
		abb_quitar(abb, b) == b,
		"Quito una hoja del arbol y me devuelve el elemento correcto");

	destruir_cosa(b);
	abb_destruir_todo(abb, destructor);
}

void pruebas_quitar_nodo_con_un_hijo()
{
	cosa_t *a = crear_cosa(3);
	cosa_t *c = crear_cosa(5);
	cosa_t *d = crear_cosa(7);

	abb_t *abb = abb_crear(comparador);
	abb_insertar(abb, a);
	abb_insertar(abb, c);
	abb_insertar(abb, d);

	pa2m_afirmar(abb_quitar(abb, c) == c,
		     "El elemento removido es el correcto");
	pa2m_afirmar(abb_quitar(abb, a) == a,
		     "El elemento removido es el correcto (otra vez)");

	destruir_cosa(c);
	destruir_cosa(a);

	abb_destruir_todo(abb, destructor);
}

void pruebas_quitar_nodo_con_dos_hijos()
{
	cosa_t *a = crear_cosa(3);
	cosa_t *b = crear_cosa(10);
	cosa_t *c = crear_cosa(5);
	cosa_t *d = crear_cosa(6);
	cosa_t *e = crear_cosa(4);
	cosa_t *f = crear_cosa(3);
	cosa_t *g = crear_cosa(20);
	cosa_t *h = crear_cosa(30);
	cosa_t *i = crear_cosa(15);

	abb_t *abb = abb_crear(comparador);
	abb_insertar(abb, a);
	abb_insertar(abb, b);
	abb_insertar(abb, c);
	abb_insertar(abb, d);
	abb_insertar(abb, e);
	abb_insertar(abb, f);
	abb_insertar(abb, g);
	abb_insertar(abb, h);
	abb_insertar(abb, i);

	pa2m_afirmar(abb_quitar(abb, b) == b,
		     "el elemento retirado es el correcto");
	pa2m_afirmar(abb->nodo_raiz->derecha->izquierda->elemento == c &&
			     abb->nodo_raiz->derecha->derecha->elemento == g,
		     "los nodos enlazados al nuevo nodo son correctos");

	destruir_cosa(b);
	abb_destruir_todo(abb, destructor);
}

void pruebas_borrado_solo_raiz()
{
	cosa_t *a = crear_cosa(3);
	cosa_t *b = crear_cosa(2);
	cosa_t *c = crear_cosa(10);
	cosa_t *d = crear_cosa(5);
	cosa_t *e = crear_cosa(6);

	abb_t *abb = abb_crear(comparador);
	abb_insertar(abb, a);
	abb_insertar(abb, b);
	abb_insertar(abb, c);
	abb_insertar(abb, d);
	abb_insertar(abb, e);

	abb_quitar(abb, a);
	pa2m_afirmar(abb->nodo_raiz->elemento == b &&
			     abb->nodo_raiz->izquierda == NULL &&
			     abb->nodo_raiz->derecha->elemento == c,
		     "Quito la raiz y los hijos son los correctos");

	abb_quitar(abb, b);
	pa2m_afirmar(abb->nodo_raiz->elemento == c &&
			     abb->nodo_raiz->izquierda->elemento == d &&
			     abb->nodo_raiz->derecha == NULL,
		     "Quito la raiz y los hijos son los correctos");

	abb_quitar(abb, c);
	pa2m_afirmar(abb->nodo_raiz->elemento == d &&
			     abb->nodo_raiz->izquierda == NULL &&
			     abb->nodo_raiz->derecha->elemento == e,
		     "Quito la raiz y los hijos son los correctos");
	destruir_cosa(a);
	destruir_cosa(b);
	destruir_cosa(c);
	abb_destruir_todo(abb, destructor);
}

void buscar_en_arbol_inexistente_devuelve_null()
{
	cosa_t *cosa = crear_cosa(3);
	pa2m_afirmar(!abb_buscar(NULL, cosa),
		     "buscar en un arbol inexistente devuelve NULL");
	destruir_cosa(cosa);
}

void buscar_elemento_me_devuelve_el_elemento()
{
	cosa_t *a = crear_cosa(2);
	cosa_t *b = crear_cosa(3);
	cosa_t *aux = crear_cosa(0);

	abb_t *arbol = abb_crear(comparador);
	abb_insertar(arbol, a);
	abb_insertar(arbol, b);
	aux->clave = 2;
	pa2m_afirmar(abb_buscar(arbol, aux) == a,
		     "El elemento buscado fue encontrado y devuelto");
	aux->clave = 3;
	pa2m_afirmar(
		abb_buscar(arbol, aux) == b,
		"El elemento buscado fue encontrado y devuelto (otra vez)");

	destruir_cosa(aux);
	abb_destruir_todo(arbol, destructor);
}

void buscar_elemento_inexistente_devuelve_null()
{
	cosa_t *a = crear_cosa(2);
	cosa_t *b = crear_cosa(3);
	cosa_t *aux = crear_cosa(0);

	abb_t *arbol = abb_crear(comparador);
	abb_insertar(arbol, a);
	abb_insertar(arbol, b);
	aux->clave = 5;
	pa2m_afirmar(
		!abb_buscar(arbol, aux),
		"Buscar un elemento que no existe en el arbol devuelve NULL");
	destruir_cosa(aux);
	abb_destruir_todo(arbol, destructor);
}

void buscar_elemento_en_arbol_vacio_devuelve_null()
{
	cosa_t *aux = crear_cosa(0);
	abb_t *arbol = abb_crear(comparador);
	pa2m_afirmar(!abb_buscar(arbol, aux),
		     "Buscar en un arbol vacio devuelve NULL");

	destruir_cosa(aux);
	abb_destruir_todo(arbol, destructor);
}

void pruebas_de_busqueda()
{
	buscar_en_arbol_inexistente_devuelve_null();
	buscar_elemento_me_devuelve_el_elemento();
	buscar_elemento_inexistente_devuelve_null();
	buscar_elemento_en_arbol_vacio_devuelve_null();
}
void iterar_un_arbol_inexistente_devuelve_null()
{
	pa2m_afirmar(!abb_con_cada_elemento(NULL, INORDEN, foo_contadora, NULL),
		     "No se puede iterar un arbol inexistente");
}

void iterar_un_arbol_vacio_devuelve_cero()
{
	abb_t *arbol = abb_crear(comparador);

	pa2m_afirmar(!abb_con_cada_elemento(arbol, INORDEN, foo_contadora,
					    NULL),
		     "Iterar inorden un arbol vacio devuelve cero elementos");
	pa2m_afirmar(!abb_con_cada_elemento(arbol, PREORDEN, foo_contadora,
					    NULL),
		     "Iterar preorden un arbol vacio devuelve cero elementos");
	pa2m_afirmar(!abb_con_cada_elemento(arbol, POSTORDEN, foo_contadora,
					    NULL),
		     "Iterar postorden un arbol vacio devuelve cero elementos");
	abb_destruir(arbol);
}

void iterar_con_funcion_nula_devuelve_null()
{
	abb_t *arbol = abb_crear(comparador);
	cosa_t *cosa = crear_cosa(1);
	abb_insertar(arbol, cosa);
	pa2m_afirmar(
		!abb_con_cada_elemento(arbol, INORDEN, NULL, NULL),
		"Iterar inorden con una funcion nula devuelve cero elementos");
	pa2m_afirmar(
		!abb_con_cada_elemento(arbol, PREORDEN, NULL, NULL),
		"Iterar preorden con una funcion nula devuelve cero elementos");
	pa2m_afirmar(
		!abb_con_cada_elemento(arbol, POSTORDEN, NULL, NULL),
		"Iterar postorden con una funcion nula devuelve cero elementos");
	abb_destruir_todo(arbol, destructor);
}

void iterar_todos_los_elementos_devuelve_la_cantidad_correcta()
{
	abb_t *arbol = abb_crear(comparador);
	cosa_t *c1 = crear_cosa(10);
	cosa_t *c2 = crear_cosa(2);
	cosa_t *c3 = crear_cosa(11);
	cosa_t *c4 = crear_cosa(6);
	abb_insertar(arbol, c1);
	abb_insertar(arbol, c2);
	abb_insertar(arbol, c3);
	abb_insertar(arbol, c4);
	pa2m_afirmar(
		abb_con_cada_elemento(arbol, INORDEN, foo_contadora, NULL) == 4,
		"Se iteran inorden todos los elementos del arbol y devuelve la ctdad correcta");
	pa2m_afirmar(
		abb_con_cada_elemento(arbol, PREORDEN, foo_contadora, NULL) ==
			4,
		"Se iteran preorden todos los elementos del arbol y devuelve la ctdad correcta");
	pa2m_afirmar(
		abb_con_cada_elemento(arbol, POSTORDEN, foo_contadora, NULL) ==
			4,
		"Se iteran postorden todos los elementos del arbol y devuelve la ctdad correcta");
	abb_destruir_todo(arbol, destructor);
}

void iterar_parcialmente_un_arbol_devuelve_la_cantidad_correcta()
{
	abb_t *arbol = abb_crear(comparador);
	cosa_t *c1 = crear_cosa(10);
	cosa_t *c2 = crear_cosa(2);
	cosa_t *c3 = crear_cosa(11);
	cosa_t *c4 = crear_cosa(6);
	cosa_t *c5 = crear_cosa(5);
	cosa_t *c6 = crear_cosa(3);
	cosa_t *c7 = crear_cosa(1);
	abb_insertar(arbol, c1);
	abb_insertar(arbol, c2);
	abb_insertar(arbol, c3);
	abb_insertar(arbol, c4);
	abb_insertar(arbol, c5);
	abb_insertar(arbol, c6);
	abb_insertar(arbol, c7);
	pa2m_afirmar(
		abb_con_cada_elemento(arbol, INORDEN, foo_contadora_parcial,
				      NULL) == 4,
		"Se iteran inorden parcialmente los elementos del arbol y devuelve la ctdad correcta");
	pa2m_afirmar(
		abb_con_cada_elemento(arbol, PREORDEN, foo_contadora_parcial,
				      NULL) == 5,
		"Se iteran preorden parcialmente los elementos del arbol y devuelve la ctdad correcta");
	pa2m_afirmar(
		abb_con_cada_elemento(arbol, POSTORDEN, foo_contadora_parcial,
				      NULL) == 3,
		"Se iteran postorden parcialmente los elementos del arbol y devuelve la ctdad correcta");
	abb_destruir_todo(arbol, destructor);
}
void pruebas_de_recorrido()
{
	iterar_un_arbol_inexistente_devuelve_null();
	iterar_un_arbol_vacio_devuelve_cero();
	iterar_con_funcion_nula_devuelve_null();
	iterar_todos_los_elementos_devuelve_la_cantidad_correcta();
	iterar_parcialmente_un_arbol_devuelve_la_cantidad_correcta();
}

int main()
{
	pa2m_nuevo_grupo("Pruebas de creacion");
	pruebas_de_creacion();

	pa2m_nuevo_grupo("Pruebas de insercion");
	pa2m_nuevo_grupo("Pruebas de insercion con un elemento");
	pruebas_insercion_de_un_elemento();
	pa2m_nuevo_grupo("Pruebas de insercion con varios elementos");
	pruebas_insercion_varios_elementos();
	pa2m_nuevo_grupo("Pruebas de quitar");
	pa2m_nuevo_grupo("Pruebas de quitar una hoja");
	pruebas_quitar_hoja();
	pa2m_nuevo_grupo("Pruebas de quitar nodo con un hijo");
	pruebas_quitar_nodo_con_un_hijo();
	pa2m_nuevo_grupo("Pruebas de quitar con dos hijos");
	pruebas_quitar_nodo_con_dos_hijos();
	pa2m_nuevo_grupo("Pruebas de borrado desde la raiz");
	pruebas_borrado_solo_raiz();
	pa2m_nuevo_grupo("Pruebas de busqueda");
	pruebas_de_busqueda();
	pa2m_nuevo_grupo("Pruebas de iterador interno");
	pruebas_de_recorrido();
	return pa2m_mostrar_reporte();
}
