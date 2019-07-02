#ifndef IPRODUCTO_H
#define IPRODUCTO_H

#include <map>
#include <string>

#include "producto.h"
#include "datatypes.h"

using namespace std;

class IProducto {
public:
  //Devuelve la cantidad de productos comunes del sistema
  virtual int cantProdComun() = 0;
  virtual void agregarComun(string, string, float) = 0;
  virtual void agregarMenu(string, string) = 0;
  virtual void quitarMenu(string) = 0;
  // Agrega producto comun al menu
  virtual void agregarAlMenu(string, int) = 0;
  virtual list<DtComun> listarComunes() = 0;
  virtual list<DtMenu> listarMenus() = 0;
  // Devuelve la cantidad de productos vendidos
  virtual DtProducto* obtenerProducto(string, int&) = 0;
  virtual Producto* obtenerProducto(string) = 0;
  virtual void borrarProducto(string) = 0;
  virtual void cargarDatosDePrueba() = 0;
};

class ControladorProducto: public IProducto {
private:
  map<string, Comun*> comunes;
  map<string, Menu*> menus;
  Menu* ultimo;
  static ControladorProducto* instancia;
  ControladorProducto();
public:
  static ControladorProducto* getInstance();
  virtual int cantProdComun();
  virtual void agregarComun(string, string, float);
  virtual void agregarMenu(string, string);
  virtual void quitarMenu(string);
  virtual void agregarAlMenu(string, int);
  virtual list<DtComun> listarComunes();
  virtual list<DtMenu> listarMenus();
  virtual DtProducto* obtenerProducto(string, int&);
  virtual Producto* obtenerProducto(string);
  virtual void borrarProducto(string);
  virtual void cargarDatosDePrueba();
};

#endif
