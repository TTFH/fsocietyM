#ifndef FABRICA_H
#define FABRICA_H

#include "iproducto.h"
#include "iempleado.h"
#include "icliente.h"
#include "iventa.h"

class Factory {
public:
  IProducto* obtenerIProducto();
  IEmpleado* obtenerIEmpleado();
  ICliente* obtenerICliente();
  IVenta* obtenerIVenta();
};

#endif
