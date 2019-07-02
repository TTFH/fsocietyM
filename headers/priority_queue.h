/*
  Cola de prioridad de elementos de tipo 'info_t'.
  Puede contener cualquier cantidad de elementos.
  Las prioridades de los elementos son del tipo 'prio_t', deben estar entre 1 y 'size' inclusives.
  Se considera que el elemento prioritario es aquel que fue insertado con un valor menor de 'p'.
  Si más de un elemento cumple esa condición, el prioritario es el que se haya insertado primero.
*/

#ifndef _COLA_PRIORIDAD_H
#define _COLA_PRIORIDAD_H

#include <cstdint>
#include <stdexcept>
#include <initializer_list>

template <class Type>
class array {
private:
  int32_t start; // start index
  int32_t size; // size
  Type* data; // start..start + size - 1
public:
  array(); // null
  array(int32_t); // 0..size-1
  array(int32_t, int32_t); // start..end
  array(const array&); // a(b) copy (same size)
  array(const Type*, int32_t); // copy min(a.size, n)
  ~array(); // auto delete & null

  Type& operator[](int32_t); // a[i] = x
  Type operator[](int32_t) const; // x = a[i]
  array operator=(const array&); // a[] = b[] init min(a.size, b.size)
  array operator=(std::initializer_list<Type>); // a[] = {} init min(a.size, {}.size)

  void zero(); // init with 0, false, '/0', NULL, etc.
  void destroy(); // delete & null
  void resize(int32_t, int32_t); // delete & start..end
  Type* to_c(int32_t&); // copy array<Type> to Type*, return size
};

// array<Type> function(const array<Type> &value, array<Type> &ref, array<Type> copy);

template <class Type>
array<Type>::array() {
  start = 0;
  size = 0;
  data = nullptr;
}

template <class Type>
array<Type>::array(int32_t size) : array(0, size-1) {
  
}

template <class Type>
array<Type>::array(int32_t start, int32_t end) : array() {
  this->resize(start, end);
}

template <class Type>
array<Type>::array(const array& arr) {
  start = arr.start;
  size = arr.size;
  data = new Type[size];
  for (int32_t i = 0; i < size; i++)
    data[i] = arr.data[i];
}

template <class Type>
array<Type>::array(const Type* arr, int32_t n) {
  start = 0;
  size = n;
  data = new Type[size];
  for (int32_t i = 0; i < size && i < n; i++)
    data[i] = arr[i];
}

template <class Type>
array<Type>::~array() {
  this->destroy();
}

template <class Type>
Type& array<Type>::operator[](int32_t pos) {
  if (pos < start || pos > start + size - 1)
    throw std::out_of_range("Position out of range");
  return data[pos - start];
}

template <class Type>
Type array<Type>::operator[](int32_t pos) const {
  if (pos < start || pos > start + size - 1)
    throw std::out_of_range("Position out of range");
  return data[pos - start];
}

template <class Type>
array<Type> array<Type>::operator=(const array& arr) {
  for (int32_t i = 0; i < size && i < arr.size; i++)
    data[i] = arr.data[i];
  return *this;
}

template <class Type>
array<Type> array<Type>::operator=(std::initializer_list<Type> dat) {
  int32_t i = 0;
  typename std::initializer_list<Type>::iterator it;
  for (it = dat.begin(); it != dat.end() && i < size; ++it) {
    data[i] = *it;
    i++;
  }
  return *this;
}

template <class Type>
void array<Type>::zero() {
  for (int32_t i = 0; i < size; i++)
    data[i] = static_cast<Type>(0);
}

template <class Type>
void array<Type>::destroy() {
  if (data != nullptr) {
    delete[] data;
    data = nullptr;
  }
}

template <class Type>
void array<Type>::resize(int32_t start, int32_t end) {
  this->destroy();
  if (end < start)
    throw std::invalid_argument("End index should be lower that start index");
  this->start = start;
  size = end - start + 1;
  data = new Type[size];
}

template <class Type>
Type* array<Type>::to_c(int32_t& n) {
  Type* res = new Type[size];
  for (int32_t i = 0; i < size; i++)
    res[i] = data[i];
  n = size;
  return res;
}

#include <list>

typedef const char* string;

typedef unsigned int uint;
typedef uint prio_t;
typedef string info_t;

struct nodo {
  prio_t prioridad;
  std::list<info_t> datos;
};

class PriorityQueue {
private:
  array<nodo> elem;
  uint tope;
  uint capacidad;
  array<bool> hay_prio;
public:
  PriorityQueue(uint);
  ~PriorityQueue();

  /*
    O(size) si have_priority(p)
    O(log size) si !have_priority(p)
    1 <= p <= size
   */
  void push(const info_t, prio_t);

  /*
    Si es vacia no hace nada
    Elimina el elemento prioritario en O(log size)
   */
  void pop();

  /*
    Devuelve true si es vacia en O(1)
   */
  bool empty();
  
  uint size();

  /*
    Devuelve true si algún elemento de c está asociado al valor p en O(1)
   */
  bool have_priority(const prio_t p);

  /*
    Devuelve la lista de elementos asociados al valor p en O(size)
   */
  std::list<info_t> list_priority(const prio_t p);

  /*
    Devuelve el elemento prioritario de c en O(1)
   */
  info_t top();

  /*
    Devuelve el valor asociado al elemento prioritario en O(1)
   */
  prio_t max_priority();
};
#endif
