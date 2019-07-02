#ifndef IEMPLEADO_H
#define IEMPLEADO_H

#include <map>
#include <list>
#include <string>

#include "empleado.h"
#include "datatypes.h"

const int MAX_MESAS = 7;

class IEmpleado {
public:
  virtual void agregarMozo(string) = 0;
  virtual void agregarRepartidor(string, TipoVehiculo) = 0;
  virtual void asignarMesas() = 0;
  virtual list<int> listarMozos() = 0;
  virtual list<DtRepartidor> listarRepartidores() = 0;
  // Devuelve las mesas asignadas al mozo sin una venta en curso
  virtual list<int> obtenerMesas(int) = 0;
  virtual Mozo* obtenerMozo(int) = 0;
  virtual Repartidor* obtenerRepartidor(int) = 0;
  virtual void cargarDatosDePrueba() = 0;
};

class ControladorEmpleado: public IEmpleado {
private:
  int id;
  map<int, Mozo*> mozos;
  map<int, Repartidor*> repartidores;
  static ControladorEmpleado* instancia;
  ControladorEmpleado();
public:
  static ControladorEmpleado* getInstance();
  virtual void agregarMozo(string);
  virtual void agregarRepartidor(string, TipoVehiculo);
  virtual void asignarMesas();
  virtual list<int> listarMozos();
  virtual list<DtRepartidor> listarRepartidores();
  virtual list<int> obtenerMesas(int);
  virtual Mozo* obtenerMozo(int);
  virtual Repartidor* obtenerRepartidor(int);
  virtual void cargarDatosDePrueba();
};

#endif
