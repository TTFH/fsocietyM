#include <list>
#include <string>

#include "../include/empleado.h"
#include "../include/datatypes.h"

Mesa::Mesa(int num) {
  numero = num;
}

int Mesa::obtenerIdMesa() {
  return numero;
}

Empleado::Empleado(string name, int ident) {
  nombre = name;
  id = ident;
}

Empleado::~Empleado() { }

int Empleado::getID() {
  return id;
}

string Empleado::getNombre() {
  return nombre;
}

Repartidor::Repartidor(string name, int ident, TipoVehiculo veh) : Empleado(name, ident) {
  vehiculo = veh;
}

DtRepartidor Repartidor::obtenerRepartidor() {
  return DtRepartidor(this->getID(), this->getNombre(), vehiculoAStr(vehiculo));
}

Mozo::Mozo(string name, int ident) : Empleado(name, ident) { }

void Mozo::agregarMesa(int id) {
  Mesa* mesa = new Mesa(id);
  mesas.push_back(mesa);
}

Mesa* Mozo::obtenerMesa(int id_mesa) {
  Mesa* res = NULL;
  for (list<Mesa*>::iterator it = mesas.begin(); it != mesas.end(); ++it)
    if ((*it)->obtenerIdMesa() == id_mesa)
      res = *it;
  return res;
}

list<int> Mozo::obtenerMesas() {
  list<int> res;
  for (list<Mesa*>::iterator it = mesas.begin(); it != mesas.end(); ++it)
    res.push_back((*it)->obtenerIdMesa());
  return res;
}

void Mozo::quitarTodasLasMesas() {
  for (list<Mesa*>::iterator it = mesas.begin(); it != mesas.end(); ++it)
    delete *it;
  mesas.clear();
}
