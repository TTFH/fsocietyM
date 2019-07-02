#ifndef EMPLEADO_H
#define EMPLEADO_H

#include <list>
#include <string>

#include "datatypes.h"

using namespace std;

class Mesa {
private:
  int numero;
public:
  Mesa(int);
  int obtenerIdMesa();
};

class Empleado {
private:
  int id;
  string nombre;
public:
  Empleado(string, int);
  virtual ~Empleado();
  int getID();
  string getNombre();
};

class Repartidor: public Empleado {
private:
  TipoVehiculo vehiculo;
public:
  Repartidor(string, int, TipoVehiculo);
  DtRepartidor obtenerRepartidor();
};

class Mozo: public Empleado {
private:
  list<Mesa*> mesas;
public:
  Mozo(string, int);
  // Crea una mesa y agrega la asociacion con mozo
  void agregarMesa(int);
  // Devuelve la mesa con id
  Mesa* obtenerMesa(int);
  // Devuelve una lista con los ID de las mesas que tiene asignadas
  list<int> obtenerMesas();
  // Destruye todas las mesas que tiene asignadas para luego hacer una reasignacion
  void quitarTodasLasMesas();
};

#endif
