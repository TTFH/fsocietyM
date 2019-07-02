#ifndef CLIENTE_H
#define CLIENTE_H

#include <string>

#include "datatypes.h"

class Cliente {
private:
  int telefono;
  string nombre;
  DtDireccion* direccion;
public:
  Cliente(int, string, DtDireccion*);
  ~Cliente();
  int getTelefono();
  string getNombre();
  DtDireccion* getDireccion();
};

#endif
