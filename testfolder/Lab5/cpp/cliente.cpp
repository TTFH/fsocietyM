#include <string>

#include "../include/cliente.h"
#include "../include/datatypes.h"

Cliente::Cliente(int tel, string name, DtDireccion* dir) {
  telefono = tel;
  nombre = name;
  direccion = dir;
}

Cliente::~Cliente() {
  delete direccion;
}

int Cliente::getTelefono() {
  return telefono;
}

string Cliente::getNombre() {
  return nombre;
}

DtDireccion* Cliente::getDireccion() {
  return direccion;
}
