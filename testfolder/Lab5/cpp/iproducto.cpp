#include <map>
#include <cstddef>

#include "../include/fabrica.h"
#include "../include/iventa.h"
#include "../include/iproducto.h"

using namespace std;

ControladorProducto* ControladorProducto::instancia = NULL;

ControladorProducto::ControladorProducto() {
  ultimo = NULL;
}

ControladorProducto* ControladorProducto::getInstance() {
  if (instancia == NULL)
    instancia = new ControladorProducto();
  return instancia;
}

int ControladorProducto::cantProdComun() {
  return comunes.size();
}

void ControladorProducto::agregarComun(string codigo, string descripcion, float precio) {
  map<string, Comun*>::iterator it = comunes.find(codigo);
  if (it != comunes.end())
    throw invalid_argument("Existe un producto comun registrado con ese codigo.");

  map<string, Menu*>::iterator jt = menus.find(codigo);
  if (jt != menus.end())
    throw invalid_argument("Existe un menu registrado con ese codigo.");

  Comun* nuevoP = new Comun(codigo, descripcion, precio);
  comunes[codigo] = nuevoP;
}

void ControladorProducto::agregarMenu(string codigo, string descripcion) {
  map<string, Comun*>::iterator it = comunes.find(codigo);
  if (it != comunes.end())
    throw invalid_argument("Existe un producto comun registrado con ese codigo.");

  map<string, Menu*>::iterator jt = menus.find(codigo);
  if (jt != menus.end())
    throw invalid_argument("Existe un menu registrado con ese codigo.");

  Menu* nuevoM = new Menu(codigo, descripcion);
  menus[codigo] = nuevoM;
  ultimo = nuevoM;
}

void ControladorProducto::quitarMenu(string codigo) {
  map<string, Menu*>::iterator it = menus.find(codigo);
  if (it != menus.end()) {
    Menu* menu = it->second;
    menus.erase(it);
    delete menu;
  }
}

void ControladorProducto::agregarAlMenu(string codigo, int cantidad) {
  ultimo->agregarComun(cantidad, comunes[codigo]);
}

list<DtComun> ControladorProducto::listarComunes() {
  list<DtComun> res;
  for (map<string, Comun*>::iterator it = comunes.begin(); it != comunes.end(); ++it)
    res.push_back(it->second->obtenerComun());
  return res;
}

list<DtMenu> ControladorProducto::listarMenus() {
  list<DtMenu> res;
  for (map<string, Menu*>::iterator it = menus.begin(); it != menus.end(); ++it)
    res.push_back(it->second->obtenerMenu());
  return res;
}

DtProducto* ControladorProducto::obtenerProducto(string codigo, int& cant_vendidos) {
  Factory* fabrica = new Factory();
  IVenta* iv = fabrica->obtenerIVenta();
  cant_vendidos = iv->cantidadProdsVendidos(codigo);

  DtProducto* res;
  map<string, Comun*>::iterator it = comunes.find(codigo);
  if (it != comunes.end())
    res = new DtComun(it->second->obtenerComun());
  else {
    map<string, Menu*>::iterator j = menus.find(codigo);
    if (j != menus.end())
      res = new DtMenu(j->second->obtenerMenu());
    else
      throw invalid_argument("No hay ningun producto registrado con ese codigo.");
  }
  delete fabrica;
  return res;
}

Producto* ControladorProducto::obtenerProducto(string codigo) {
  Producto* res;
  map<string, Comun*>::iterator it = comunes.find(codigo);
  if (it != comunes.end())
    res = it->second;
  else {
    map<string, Menu*>::iterator j = menus.find(codigo);
    if (j != menus.end())
      res = j->second;
    else
      res = NULL;
  }
  return res;
}

void ControladorProducto::borrarProducto(string codigo) {
  Factory* fabrica = new Factory();
  IVenta* iv = fabrica->obtenerIVenta();
  if (!iv->estanFacturadas(codigo)) {
    delete fabrica;
    throw invalid_argument("Hay ventas sin facturar con ese producto.");
  }

  map<string, Comun*>::iterator it = comunes.find(codigo);
  if (it != comunes.end()) {
      for (map<string, Menu*>::iterator j = menus.begin(); j != menus.end(); ++j)
        j->second->quitarComun(codigo);
      Comun* comun = it->second;
      comunes.erase(it);
      delete comun;
  } else
    this->quitarMenu(codigo);
  delete fabrica;
}

void ControladorProducto::cargarDatosDePrueba() {
  comunes["1"] = new Comun("1", "Pizza con muzzarella", 130);
  comunes["2"] = new Comun("2", "Milanesa con guarnición", 230);
  comunes["3"] = new Comun("3", "Coca Cola 1.25L", 95);
  comunes["4"] = new Comun("4", "Empanada Capresse", 72);
  comunes["5"] = new Comun("5", "Flan con dulce de leche", 110);

  Menu* menuP6 = new Menu("6", "Combo Pizza");
  menus["6"] = menuP6;
  menuP6->agregarComun(1, comunes["1"]);
  menuP6->agregarComun(1, comunes["3"]);

  Menu* menuP7 = new Menu("7", "Combo Empanadas");
  menus["7"] = menuP7;
  menuP7->agregarComun(1, comunes["3"]);
  menuP7->agregarComun(3, comunes["4"]);
}
