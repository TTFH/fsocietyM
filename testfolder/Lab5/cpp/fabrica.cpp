#include "../include/fabrica.h"

IProducto* Factory::obtenerIProducto() {
  return ControladorProducto::getInstance();
}

IEmpleado* Factory::obtenerIEmpleado() {
  return ControladorEmpleado::getInstance();
}

ICliente* Factory::obtenerICliente() {
  return ControladorCliente::getInstance();
}

IVenta* Factory::obtenerIVenta() {
  return ControladorVenta::getInstance();
}
