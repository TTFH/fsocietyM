#include <cstddef>

#include "../include/icliente.h"

ControladorCliente* ControladorCliente::instancia = NULL;

ControladorCliente* ControladorCliente::getInstance() {
  if (instancia == NULL)
    instancia = new ControladorCliente();
  return instancia;
}

ControladorCliente::ControladorCliente() { }

bool ControladorCliente::estaRegistrado(int tel) {
  map<int, Cliente*>::iterator it = clientes.find(tel);
  return it != clientes.end();
}

Cliente* ControladorCliente::obtenerCliente(int tel) {
  map<int, Cliente*>::iterator it = clientes.find(tel);
  return it->second;
}

void ControladorCliente::agregarCliente(int telefono, string nombre, DtDireccion* dir) {
  Cliente* nuevoC = new Cliente(telefono, nombre, dir);
  clientes[telefono] = nuevoC;
}

void ControladorCliente::cargarDatosDePrueba() {
  DtDireccion* dir;
  dir = new DtEdificio("18 de Julio", 2033, "", "", "Mar Austral", 701);
  clientes[98217523] = new Cliente(98217523, "Vladimir", dir);
  dir = new DtDireccion("Cabildo", 500, "", "");
  clientes[91651249] = new Cliente(91651249, "Martin", dir);
  dir = new DtEdificio("Avenida Brasil", 842, "", "", "Atlantis", 302);
  clientes[97321590] = new Cliente(97321590, "Ronaldo", dir);
}
