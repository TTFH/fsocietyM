#ifndef VENTA_H
#define VENTA_H

#include <list>
#include <string>

#include "cliente.h"
#include "empleado.h"
#include "producto.h"
#include "datatypes.h"

using namespace std;

class Venta {
private:
  int numero;
  list<V_P*> productos;
  DtFactura* factura;
public:
  Venta(int);
  virtual ~Venta();
  int getNumero();
  // Devuelve el subtotal de la venta (sin descuento ni IVA)
  float getSubtotal();

  bool estaFacturada();
  void setFactura(DtFactura*);
  DtFactura* obtenerFactura();

  // Agrega un producto a la venta, si ya esta se incrementa la cantidad
  void agregarProducto(string, int);
  // Quita cant productos de la venta, si queda en cero se elimina
  void quitarProducto(string, int);
  list<DtProducto*> listarProductos();
  // Devuelve la cantidad de productos en la venta
  // PreCondicion: La venta esta facturada
  int cantidadProds(string);
  bool contieneProd(string);
  bool contieneAlgunMenu();
};

class Local: public Venta {
private:
  Mozo* mozo;
  list<Mesa*> mesas;
public:
  Local(int, Mozo*);
  void agregarMesa(Mesa*);
  bool contieneMesa(int);
  void facturar(int);
};

class A_Domicilio: public Venta {
private:
  Cliente* cliente;
  Repartidor* repartidor;
  Estado estado; // Estado actual
  list<DtActualizacion> cambiosEstado;
public:
  A_Domicilio(int, Cliente*);
  bool esDelCliente(int);
  int getTelCliente();
  Estado getEstado();
  int getIdRepartidor();
  void setRepartidor(Repartidor*);
  void setEstado(Estado); // Con fecha y hora actual
  void setEstado(Estado, DtFecha, DtHora);
  void facturar();
  // PreCondicion: La venta esta facturada
  DtInfoCliente getInfoCliente();
};

#endif
