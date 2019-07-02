#ifndef DATATYPES_H
#define DATATYPES_H

#include <list>
#include <string>

using namespace std;

enum TipoVehiculo { PIE, BICI, MOTO };

enum Estado {Pedido, EnCamino, Recibido, Cancelado};

// Funciones auxiliares que convierten enumerado a string
string vehiculoAStr(TipoVehiculo vehiculo);
string estadoAStr(Estado estado);

class DtProducto {
private:
  int cantidad;
  string codigo;
  string descripcion;
public:
  DtProducto(string, string);
  virtual ~DtProducto();
  virtual float getPrecio() = 0;
  void setCantidad(int);
  int getCantidad();
  string getCodigo();
  string getDescripcion();
};

class DtComun: public DtProducto {
private:
  float precio;
public:
  DtComun(string, string, float);
  virtual float getPrecio();
};

class DtMenu: public DtProducto {
private:
  list<DtComun> comun;
public:
  DtMenu(string, string);
  // Agregar DtComun con cantidad
  void agregarComun(int, DtComun);
  // La suma de los precios de los productos por cantidad de cada uno
  // Menos un 10% de descuento
  virtual float getPrecio();
  list<DtComun> obtenerListaProd();
};

class DtFecha {
private:
  int dia, mes, anio;
public:
  DtFecha(); // Crea una fecha con el dia actual
  DtFecha(int, int, int);
  int getDia();
  int getMes();
  int getAnio();
  bool operator==(DtFecha); // Compara dos fechas
};

class DtHora {
private:
  int hora, minuto, segundo;
public:
  DtHora(); // Crea DtHora con la hora actual
  DtHora(int, int, int);
  int getHora();
  int getMinuto();
  int getSegundo();
};

class DtEmpleado {
private:
  int id;
  string nombre;
public:
  DtEmpleado(int, string);
  virtual ~DtEmpleado();
  int getID();
  string getNombre();
};

class DtMozo : public DtEmpleado {
public:
  DtMozo(int, string);
};

class DtRepartidor : public DtEmpleado {
private:
  string vehiculo;
public:
  DtRepartidor(int, string, string);
  string getVehiculo();
};

class DtFactura {
public:
  int codigo;
  DtFecha fecha;
  DtHora hora;
  DtEmpleado* empleado;
  list<DtProducto*> consumidos;
  float subtotal;
  int descuento; // Porcentaje
  float total, monto_iva; // 22%

  DtFactura(int, DtEmpleado*, list<DtProducto*>, float, int);
};

class DtDireccion {
private:
  string calle;
  int puerta;
  string esquina1;
  string esquina2;
public:
  DtDireccion(string, int, string, string);
  string getCalle();
  int getPuerta();
  string getEsquina1();
  string getEsquina2();
};

class DtEdificio: public DtDireccion {
private:
  string nombre;
  int apartamento;
public:
  DtEdificio(string, int, string, string, string, int);
  string getNombre();
  int getApartamento();
};

// Ventas asignadas a un repartidor
class DtVentasAsig {
private:
  Estado estado;
  int telefono_cliente;
public:
  DtVentasAsig(int, Estado);
  int getTelCliente();
  string getEstado();
};

class DtActualizacion {
private:
  DtFecha fecha;
  DtHora hora;
  Estado estado;
public:
  DtActualizacion(Estado); // Se crea con la fecha y hora actual
  DtActualizacion(Estado, DtFecha, DtHora);
  DtFecha getFecha();
  DtHora getHora();
  string getEstado();
};

class DtInfoCliente {
private:
  int telefono;
  string nombre;
  list<DtProducto*> producto;
  string nombreRepart;
  list<DtActualizacion> updates;
public:
  DtInfoCliente(int, string, list<DtProducto*>, string, list<DtActualizacion>);
  void agregarActualizacion(DtActualizacion);
  int getTelefono();
  string getNombre();
  list<DtProducto*> getListaProductos();
  string getNombreRepart();
  list<DtActualizacion> getUpdates();
};

#endif
