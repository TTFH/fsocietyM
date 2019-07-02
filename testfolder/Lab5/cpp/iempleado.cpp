#include <list>
#include <cstddef>

#include "../include/empleado.h"
#include "../include/iempleado.h"
#include "../include/fabrica.h"
#include "../include/iventa.h"

ControladorEmpleado* ControladorEmpleado::instancia = NULL;

ControladorEmpleado* ControladorEmpleado::getInstance() {
  if (instancia == NULL)
    instancia = new ControladorEmpleado();
  return instancia;
}

ControladorEmpleado::ControladorEmpleado() {
  id = 10;
}

void ControladorEmpleado::agregarMozo(string nombre) {
  id++;
  Mozo* mozo = new Mozo(nombre, id);
  mozos[id] = mozo;
}

void ControladorEmpleado::agregarRepartidor(string nombre, TipoVehiculo vehiculo) {
  id++;
  Repartidor* rep = new Repartidor(nombre, id, vehiculo);
  repartidores[id] = rep;
}

void ControladorEmpleado::asignarMesas() {
  int cant_mozos = mozos.size();
  if (cant_mozos == 0)
    throw invalid_argument("No hay mozos registrados en el sistema.");

  Factory* fabrica = new Factory();
  IVenta* iv = fabrica->obtenerIVenta();
  if (!iv->estanVentasLocalesFact()) {
    delete fabrica;
    throw invalid_argument("Hay ventas locales sin facturar.");
  }

  // Si ya se ha hecho una asignacion, restablecer todas las mesas.
  map<int, Mozo*>::iterator it;
  for (it = mozos.begin(); it != mozos.end(); ++it)
    it->second->quitarTodasLasMesas();

  int id_mesa = 1;
  for (it = mozos.begin(); it != mozos.end(); ++it) {
    for (int j = 0; j < MAX_MESAS / cant_mozos; j++) {
      it->second->agregarMesa(id_mesa);
      id_mesa++;
    }
  }
  it = mozos.begin();
  for (int i = 0; i < MAX_MESAS % cant_mozos; i++) {
    it->second->agregarMesa(id_mesa);
    id_mesa++;
    ++it;
  }
  delete fabrica;
}

list<int> ControladorEmpleado::listarMozos() {
  list<int> res;
  for (map<int, Mozo*>::iterator it = mozos.begin(); it != mozos.end(); ++it)
    res.push_back(it->second->getID());
  return res;
}

list<DtRepartidor> ControladorEmpleado::listarRepartidores() {
  list<DtRepartidor> res;
  for (map<int, Repartidor*>::iterator it = repartidores.begin(); it != repartidores.end(); ++it)
    res.push_back(it->second->obtenerRepartidor());
  return res;
}

list<int> ControladorEmpleado::obtenerMesas(int id_mozo) {
  // Agrega todas las mesas asignadas al mozo
  list<int> res = mozos[id_mozo]->obtenerMesas();

  Factory* fabrica = new Factory();
  IVenta* iv = fabrica->obtenerIVenta();

  // Quita las que tiene una venta en curso
  list<int>::iterator it = res.begin();
  while (it != res.end()) {
    if (iv->tieneVentaEnCurso(*it)) {
      it = res.erase(it); // Ir al siguente del que quite
    } else
      ++it;
  }

  delete fabrica;
  return res;
}

Mozo* ControladorEmpleado::obtenerMozo(int id_mozo) {
  return mozos[id_mozo];
}

Repartidor* ControladorEmpleado::obtenerRepartidor(int id_rep) {
  return repartidores[id_rep];
}

void ControladorEmpleado::cargarDatosDePrueba() {
  mozos[1] = new Mozo("Juan Pedro", 1);
  mozos[2] = new Mozo("Johnny", 2);
  mozos[3] = new Mozo("José", 3);

  repartidores[4] = new Repartidor("Tito", 4, BICI);
  repartidores[5] = new Repartidor("Raúl", 5, MOTO);
  repartidores[6] = new Repartidor("Tiki Gelana", 6, PIE);
}
