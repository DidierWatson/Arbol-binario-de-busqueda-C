#include "abb.h"
#include "abb_estructura_privada.h"
#include <stddef.h>
#include <stdlib.h>

nodo_abb_t *_nodo_crear(void *elemento)
{
	nodo_abb_t *nodo = calloc(1, sizeof(nodo_abb_t));
	if (!nodo)
		return NULL;
	nodo->elemento = elemento;
	return nodo;
}

abb_t *abb_crear(abb_comparador comparador)
{
	if (!comparador)
		return NULL;
	abb_t *arbol = calloc(1, sizeof(abb_t));
	if (!arbol)
		return NULL;
	arbol->comparador = comparador;
	return arbol;
}

nodo_abb_t *_abb_insertar_recursivo(nodo_abb_t *nodo, void *elemento,
				    abb_comparador comparador)
{
	if (!nodo) {
		if (!(nodo = _nodo_crear(elemento)))
			return NULL;
		return nodo;
	}
	int comparacion = comparador(elemento, nodo->elemento);
	if (comparacion <= 0)
		nodo->izquierda = _abb_insertar_recursivo(nodo->izquierda,
							  elemento, comparador);
	else
		nodo->derecha = _abb_insertar_recursivo(nodo->derecha, elemento,
							comparador);
	return nodo;
}

abb_t *abb_insertar(abb_t *arbol, void *elemento)
{
	if (!arbol)
		return NULL;
	if (!(arbol->nodo_raiz = _abb_insertar_recursivo(
		      arbol->nodo_raiz, elemento, arbol->comparador)))
		return NULL;
	arbol->tamanio++;
	return arbol;
}

nodo_abb_t *_extraer_predecesor_inorden(nodo_abb_t *nodo,
					void **elemento_extraido)
{
	if (!nodo->derecha) {
		*elemento_extraido = nodo->elemento;
		nodo_abb_t *izq = nodo->izquierda;
		free(nodo);
		return izq;
	}

	nodo->derecha =
		_extraer_predecesor_inorden(nodo->derecha, elemento_extraido);

	return nodo;
}

nodo_abb_t *_abb_quitar_recursivo(nodo_abb_t *nodo, void *elemento,
				  abb_comparador comparador,
				  void **elemento_extraido)
{
	if (!nodo)
		return NULL;
	int comparacion = comparador(elemento, nodo->elemento);
	if (comparacion == 0) {
		*elemento_extraido = nodo->elemento;
		nodo_abb_t *hijo_izq = nodo->izquierda;
		nodo_abb_t *hijo_der = nodo->derecha;

		if (hijo_izq && hijo_der) {
			void *elemento_derecho = NULL;
			nodo->izquierda = _extraer_predecesor_inorden(
				nodo->izquierda, &elemento_derecho);
			nodo->elemento = elemento_derecho;
			return nodo;
		} else {
			free(nodo);
			if (!hijo_izq)
				return hijo_der;
			return hijo_izq;
		}
	}
	if (comparacion < 0)
		nodo->izquierda = _abb_quitar_recursivo(nodo->izquierda,
							elemento, comparador,
							elemento_extraido);
	else
		nodo->derecha = _abb_quitar_recursivo(
			nodo->derecha, elemento, comparador, elemento_extraido);
	return nodo;
}

void *abb_quitar(abb_t *arbol, void *elemento)
{
	if (!arbol)
		return NULL;

	void *elemento_extraido = NULL;
	if (elemento != abb_buscar(arbol, elemento))
		return NULL;

	arbol->nodo_raiz = _abb_quitar_recursivo(arbol->nodo_raiz, elemento,
						 arbol->comparador,
						 &elemento_extraido);
	arbol->tamanio--;

	return elemento_extraido;
}

void *_abb_buscar_recursivo(nodo_abb_t *nodo, abb_comparador comparador,
			    void *elemento)
{
	if (!nodo)
		return NULL;
	int comparacion = comparador(elemento, nodo->elemento);
	if (comparacion == 0)
		return nodo->elemento;
	else if (comparacion < 0)
		return _abb_buscar_recursivo(nodo->izquierda, comparador,
					     elemento);
	else if (comparacion > 0)
		return _abb_buscar_recursivo(nodo->derecha, comparador,
					     elemento);

	return NULL;
}

void *abb_buscar(abb_t *arbol, void *elemento)
{
	return (!arbol) ? NULL :
			  _abb_buscar_recursivo(arbol->nodo_raiz,
						arbol->comparador, elemento);
}

bool abb_vacio(abb_t *arbol)
{
	return (!arbol || arbol->tamanio == 0) ? true : false;
}

size_t abb_tamanio(abb_t *arbol)
{
	return (!arbol) ? 0 : arbol->tamanio;
}

void abb_destruir(abb_t *arbol)
{
	if (!arbol)
		return;
	abb_destruir_todo(arbol, NULL);
}

void _abb_destruir_todo_recursivo(nodo_abb_t *nodo, void (*destructor)(void *))
{
	if (!nodo)
		return;
	if (nodo->izquierda)
		_abb_destruir_todo_recursivo(nodo->izquierda, destructor);
	if (nodo->derecha)
		_abb_destruir_todo_recursivo(nodo->derecha, destructor);
	if (destructor)
		destructor(nodo->elemento);
	free(nodo);
}

void abb_destruir_todo(abb_t *arbol, void (*destructor)(void *))
{
	if (!arbol)
		return;
	_abb_destruir_todo_recursivo(arbol->nodo_raiz, destructor);
	free(arbol);
	arbol = NULL;
}

bool _abb_recorrido_inorden_con_cada_elemento(nodo_abb_t *nodo,
					      bool (*funcion)(void *, void *),
					      void *aux, size_t *contador)
{
	if (!nodo)
		return true;
	bool continuar = _abb_recorrido_inorden_con_cada_elemento(
		nodo->izquierda, funcion, aux, contador);

	if (!continuar)
		return false;

	(*contador)++;
	if (!funcion(nodo->elemento, aux))
		return false;

	return _abb_recorrido_inorden_con_cada_elemento(nodo->derecha, funcion,
							aux, contador);
}

bool _abb_recorrido_preorden_con_cada_elemento(nodo_abb_t *nodo,
					       bool (*funcion)(void *, void *),
					       void *aux, size_t *contador)
{
	if (!nodo)
		return true;

	(*contador)++;
	if (!funcion(nodo->elemento, aux))
		return false;

	bool continuar = _abb_recorrido_preorden_con_cada_elemento(
		nodo->izquierda, funcion, aux, contador);
	if (!continuar)
		return false;

	return _abb_recorrido_preorden_con_cada_elemento(nodo->derecha, funcion,
							 aux, contador);
}

bool _abb_recorrido_postorden_con_cada_elemento(nodo_abb_t *nodo,
						bool (*funcion)(void *, void *),
						void *aux, size_t *contador)
{
	if (!nodo)
		return true;

	bool continuar = _abb_recorrido_postorden_con_cada_elemento(
		nodo->izquierda, funcion, aux, contador);
	if (!continuar)
		return false;

	if (!_abb_recorrido_postorden_con_cada_elemento(nodo->derecha, funcion,
							aux, contador))
		return false;

	(*contador)++;
	if (!funcion(nodo->elemento, aux))
		return false;
	return true;
}

size_t abb_con_cada_elemento(abb_t *arbol, abb_recorrido recorrido,
			     bool (*funcion)(void *, void *), void *aux)
{
	if (!arbol || !arbol->nodo_raiz || !funcion)
		return 0;
	size_t contador = 0;
	switch (recorrido) {
	case INORDEN:
		_abb_recorrido_inorden_con_cada_elemento(
			arbol->nodo_raiz, funcion, aux, &contador);
		break;

	case PREORDEN:
		_abb_recorrido_preorden_con_cada_elemento(
			arbol->nodo_raiz, funcion, aux, &contador);
		break;

	case POSTORDEN:
		_abb_recorrido_postorden_con_cada_elemento(
			arbol->nodo_raiz, funcion, aux, &contador);
		break;

	default:
		break;
	}

	return contador;
}

bool _abb_recorrer_inorden(nodo_abb_t *nodo, void **array, size_t tamanio_array,
			   size_t *guardados)
{
	if (!nodo)
		return true;

	bool continuar = _abb_recorrer_inorden(nodo->izquierda, array,
					       tamanio_array, guardados);
	if (!continuar)
		return false;

	if ((*guardados) >= tamanio_array)
		return true;
	array[*guardados] = nodo->elemento;
	(*guardados)++;

	return _abb_recorrer_inorden(nodo->derecha, array, tamanio_array,
				     guardados);
}

bool _abb_recorrer_preorden(nodo_abb_t *nodo, void **array,
			    size_t tamanio_array, size_t *guardados)
{
	if (!nodo)
		return true;

	if ((*guardados) >= tamanio_array)
		return true;
	array[*guardados] = nodo->elemento;
	(*guardados)++;

	bool continuar = _abb_recorrer_preorden(nodo->izquierda, array,
						tamanio_array, guardados);
	if (!continuar)
		return false;

	return _abb_recorrer_preorden(nodo->derecha, array, tamanio_array,
				      guardados);
}
bool _abb_recorrer_postorden(nodo_abb_t *nodo, void **array,
			     size_t tamanio_array, size_t *guardados)
{
	if (!nodo)
		return true;

	bool continuar = _abb_recorrer_postorden(nodo->izquierda, array,
						 tamanio_array, guardados);
	if (!continuar)
		return false;

	if (!_abb_recorrer_postorden(nodo->derecha, array, tamanio_array,
				     guardados))
		return false;

	if ((*guardados) >= tamanio_array)
		return true;
	array[*guardados] = nodo->elemento;
	(*guardados)++;

	return true;
}

size_t abb_recorrer(abb_t *arbol, abb_recorrido recorrido, void **array,
		    size_t tamanio_array)
{
	if (!arbol || !tamanio_array)
		return 0;

	size_t guardados = 0;

	switch (recorrido) {
	case INORDEN:
		_abb_recorrer_inorden(arbol->nodo_raiz, array, tamanio_array,
				      &guardados);
		break;
	case PREORDEN:
		_abb_recorrer_preorden(arbol->nodo_raiz, array, tamanio_array,
				       &guardados);
		break;
	case POSTORDEN:
		_abb_recorrer_postorden(arbol->nodo_raiz, array, tamanio_array,
					&guardados);
	default:
		break;
	}

	return guardados;
}
