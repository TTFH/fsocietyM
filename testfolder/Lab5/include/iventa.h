#ifndef IVENTA_H
#define IVENTA_H

#include <list>
#include <string>

#include "venta.h"
#include "datatypes.h"

class IVenta {
public:
  virtual void crearVentaLocal(int, list<int>) = 0;
  virtual void agregarProducto(int, string, int) = 0; // A Venta Local
  virtual void agregarProductoAD(int, string, int) = 0;
  virtual list<DtProducto*> listarProductos(int) = 0;
  virtual void quitarProducto(int, string, int) = 0; // De Venta Local
  virtual DtFactura* facturarVenta(int, int) = 0; // Venta Local
  virtual DtFactura* facturarVentaAD(int) = 0;
  virtual void crearVentaDom(int) = 0;
  virtual void cambiarEstado(int, Estado) = 0;
  virtual void asignarRepartidor(int, int) = 0;
  virtual list<DtFactura*> obtenerFacturas(DtFecha) = 0;
  virtual list<DtVentasAsig> obtenerVentas(int) = 0;
  virtual Estado getEstadoVentaAD(int) = 0;
  virtual int cantidadProdsVendidos(string) = 0;
  // Retorna True si todas las ventas que contienen ese producto estan facturadas
  virtual bool estanFacturadas(string) = 0;
  // Retorna True si todas las ventas estan facturadas (las ventas a domicilio siempre estan facturadas)
  virtual bool estanVentasLocalesFact() = 0;
  virtual bool tieneVentaEnCurso(int) = 0;
  virtual list<DtInfoCliente> obtenerActualCliente(int) = 0;
  virtual list<DtInfoCliente> obtenerActualAdmin() = 0;
  virtual bool contieneAlgunMenuVL(int) = 0; // nro de mesa
  virtual void cargarDatosDePrueba() = 0;
};

class ControladorVenta: public IVenta {
private:
  int num_venta;
  list<Venta*> ventas;
  static ControladorVenta* instancia;
  ControladorVenta();
  Local* VentaLocalNoFact(int); // num_mesa
  A_Domicilio* VentaADNoFact(int); // telefono
public:
  static ControladorVenta* getInstance();
  virtual void crearVentaLocal(int, list<int>);
  virtual void agregarProducto(int, string, int);
  virtual void agregarProductoAD(int, string, int);
  virtual list<DtProducto*> listarProductos(int);
  virtual void quitarProducto(int, string, int);
  virtual DtFactura* facturarVenta(int, int);
  virtual DtFactura* facturarVentaAD(int);
  virtual void crearVentaDom(int);
  virtual void cambiarEstado(int, Estado);
  virtual void asignarRepartidor(int, int);
  virtual list<DtFactura*> obtenerFacturas(DtFecha);
  virtual list<DtVentasAsig> obtenerVentas(int);
  virtual Estado getEstadoVentaAD(int);
  virtual int cantidadProdsVendidos(string);
  virtual bool estanFacturadas(string);
  virtual bool estanVentasLocalesFact();
  virtual bool tieneVentaEnCurso(int);
  virtual list<DtInfoCliente> obtenerActualCliente(int);
  virtual list<DtInfoCliente> obtenerActualAdmin();
  virtual bool contieneAlgunMenuVL(int);
  virtual void cargarDatosDePrueba();
};

#endif
