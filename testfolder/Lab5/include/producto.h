#ifndef PRODUCTO_H
#define PRODUCTO_H

#include <list>
#include <string>

#include "datatypes.h"

using namespace std;

class Producto {
private:
  string codigo;
  string descripcion;
public:
  Producto(string, string);
  virtual ~Producto();
  string getCodigo();
  string getDescripcion();
};

class Comun: public Producto {
private:
  float precio;
public:
  Comun(string, string, float);
  DtComun obtenerComun();
};

class C_M {
private:
  int cantidad;
  Comun* prod_comun;
public:
  C_M(int, Comun*);
  int obtenerCantidad();
  Comun* obtenerProducto();
};

class Menu: public Producto {
private:
  list<C_M*> comun;
public:
  Menu(string, string);
  ~Menu();
  void agregarComun(int, Comun*);
  // Si el producto identificado por codigo existe entonces
  // quita la asociacion con el C_M (de ese producto) del menu y la destruye
  void quitarComun(string);
  DtMenu obtenerMenu();
};

class V_P {
private:
  int cantidad;
  Producto* producto;
public:
  V_P(Producto*, int);
  string getCodigo();
  int getCantidad();
  void incrementarCant(int); // Puede recibir un valor negativo
  DtProducto* obtenerProducto();
};

#endif
