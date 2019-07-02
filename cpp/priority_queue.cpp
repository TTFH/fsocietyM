#include <list>
#include <cassert>

#include "../headers/priority_queue.h"

PriorityQueue::PriorityQueue(uint size) {
  elem.resize(1, size);
  tope = 0;
  capacidad = size;
  hay_prio.resize(1, size);
  for (uint i = 1; i < size; i++)
    hay_prio[i] = false;
}

void PriorityQueue::push(const info_t i, prio_t p) {
  uint j;
  if (have_priority(p)) {
    j = 1;
    while (elem[j].prioridad != p)
      j++;
    elem[j].datos.push_back(i);
  } else {
    hay_prio[p] = true;
    tope++;
    elem[tope].prioridad = p;
    elem[tope].datos.push_back(i);
    j = tope;
    while (j > 1 && elem[j/2].prioridad > p) {
      nodo aux = elem[j];
      elem[j] = elem[j/2];
      elem[j/2] = aux;
      j = j/2;
    }
  }
}

void PriorityQueue::pop() {
  if (!empty()) {
    elem[1].datos.pop_front();
    if (elem[1].datos.empty()) {
      hay_prio[max_priority()] = false;
      nodo mover = elem[tope];
      tope--;
      if (tope == 1) elem[1] = mover;
      else if (tope > 1) {
        uint i = 1;
        uint hijo = 2*i;
        while ( (hijo <= tope && mover.prioridad >= elem[hijo].prioridad) ||
              (hijo+1 <= tope && mover.prioridad >= elem[hijo+1].prioridad) ) {
          if (hijo+1 <= tope && elem[hijo].prioridad > elem[hijo+1].prioridad) hijo++;
          elem[i] = elem[hijo];
          i = hijo;
          hijo = 2*i;
        }
        elem[i] = mover;
      }
    }
  }
}

PriorityQueue::~PriorityQueue() {
  while (!empty())
    pop();
  hay_prio.destroy();
}

bool PriorityQueue::empty() {
  return tope == 0;
}

uint PriorityQueue::size() {
  uint res = 0;
  for (uint i = 1; i <= tope; i++)
    res += elem[i].datos.size();
  return res;
}

bool PriorityQueue::have_priority(const prio_t p) {
  assert(p >= 1 && p <= capacidad);
  return hay_prio[p];
}

std::list<info_t> PriorityQueue::list_priority(const prio_t p) {
  std::list<info_t> res;
  if (have_priority(p)) {
    uint j = 1;
    while (elem[j].prioridad != p)
      j++;
    res = elem[j].datos;
  }
  return res;
}

info_t PriorityQueue::top() {
  return elem[1].datos.front();
}

prio_t PriorityQueue::max_priority() {
  return elem[1].prioridad;
}
