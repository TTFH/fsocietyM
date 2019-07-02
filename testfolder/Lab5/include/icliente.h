#ifndef ICLIENTE_H
#define ICLIENTE_H

#include <map>
#include <string>

#include "cliente.h"
#include "datatypes.h"

class ICliente {
public:
  virtual bool estaRegistrado(int) = 0;
  virtual Cliente* obtenerCliente(int) = 0;
  virtual void agregarCliente(int, string, DtDireccion*) = 0;
  virtual void cargarDatosDePrueba() = 0;
};

class ControladorCliente: public ICliente {
private:
  map<int, Cliente*> clientes;
  static ControladorCliente* instancia;
  ControladorCliente();
public:
  static ControladorCliente* getInstance();
  virtual bool estaRegistrado(int);
  virtual Cliente* obtenerCliente(int);
  virtual void agregarCliente(int, string, DtDireccion*);
  virtual void cargarDatosDePrueba();
};

#endif
