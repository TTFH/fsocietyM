#include <string>

#include "../include/producto.h"
#include "../include/datatypes.h"

Producto::Producto(string cod, string descr) {
  codigo = cod;
  descripcion = descr;
}

Producto::~Producto() { }

string Producto::getCodigo() {
  return codigo;
}

string Producto::getDescripcion() {
  return descripcion;
}

Comun::Comun(string cod, string descr, float costo) : Producto(cod, descr) {
  precio = costo;
}

DtComun Comun::obtenerComun() {
  return DtComun(this->getCodigo(), this->getDescripcion(), precio);
}

Menu::Menu(string cod, string descr) : Producto(cod, descr) { }

Menu::~Menu() {
  for (list<C_M*>::iterator it = comun.begin(); it != comun.end(); ++it)
    delete *it;
}

DtMenu Menu::obtenerMenu() {
  DtMenu menu(this->getCodigo(), this->getDescripcion());
  for (list<C_M*>::iterator it = comun.begin(); it != comun.end(); ++it) {
    Comun* comun = (*it)->obtenerProducto();
    menu.agregarComun((*it)->obtenerCantidad(), comun->obtenerComun());
  }
  return menu;
}

void Menu::agregarComun(int cant, Comun* prod) {
  C_M* cm = new C_M(cant, prod);
  comun.push_back(cm);
}

void Menu::quitarComun(string codigo) {
  list<C_M*>::iterator it = comun.begin();

  while (it != comun.end()) {
    Comun* prod_comun = (*it)->obtenerProducto();
    if (prod_comun->getCodigo() == codigo) {
      C_M* borrar = *it;
      it = comun.erase(it); // Ir al siguente del que borre
      delete borrar;
    } else
      ++it;
  }
}

C_M::C_M(int cant, Comun* prod) {
  cantidad = cant;
  prod_comun = prod;
}

int C_M::obtenerCantidad() {
  return cantidad;
}

Comun* C_M::obtenerProducto() {
  return prod_comun;
}

V_P::V_P(Producto* prod, int cant) {
  producto = prod;
  cantidad = cant;
}

void V_P::incrementarCant(int cant) {
  cantidad += cant;
}

string V_P::getCodigo() {
  return producto->getCodigo();
}

int V_P::getCantidad() {
  return cantidad;
}

DtProducto* V_P::obtenerProducto() {
  DtProducto* res = NULL;
  if (dynamic_cast<Comun*>(producto) != NULL) {
    Comun* comun = dynamic_cast<Comun*>(producto);
    res = new DtComun(comun->obtenerComun());
    res->setCantidad(cantidad);
  } else {
    Menu* menu = dynamic_cast<Menu*>(producto);
    res = new DtMenu(menu->obtenerMenu());
    res->setCantidad(cantidad);
  }
  return res;
}
