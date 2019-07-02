#include <list>
#include <ctime>
#include <string>

#include "../include/datatypes.h"

string vehiculoAStr(TipoVehiculo vehiculo) {
  string res;
  if (vehiculo == PIE)
    res = "Pie";
  else if (vehiculo == BICI)
    res = "Bicicleta";
  else if (vehiculo == MOTO)
    res = "Moto";
  return res;
}

string estadoAStr(Estado estado) {
  string res;
  if (estado == Pedido)
    res = "Pedido";
  else if (estado == EnCamino)
    res = "En Camino";
  else if (estado == Recibido)
    res = "Recibido";
  else if (estado == Cancelado)
    res = "Cancelado";
  return res;
}

DtProducto::DtProducto(string cod, string desc) {
  // En algunas listas solo se necesitan codigo y descripcion
  // si se necesita la cantidad se setea con setCantidad(int)
  cantidad = 0;
  codigo = cod;
  descripcion = desc;
}

DtProducto::~DtProducto() { }

void DtProducto::setCantidad(int cant) {
  cantidad = cant;
}

int DtProducto::getCantidad() {
  return cantidad;
}

string DtProducto::getCodigo() {
  return codigo;
}

string DtProducto::getDescripcion() {
  return descripcion;
}

DtComun::DtComun(string cod, string descr, float costo) : DtProducto(cod, descr) {
  precio = costo;
}

float DtComun::getPrecio() {
  return precio;
}

DtMenu::DtMenu(string cod, string descr) : DtProducto(cod, descr) {
  // Los productos se agregan con agregarComun(int, DtComun)
  // y el precio se calcula
}

void DtMenu::agregarComun(int cant, DtComun prod) {
  prod.setCantidad(cant);
  comun.push_back(prod);
}

float DtMenu::getPrecio() {
  float suma = 0;
  float subtotal = 0;
  for (list<DtComun>::iterator j = comun.begin(); j != comun.end(); ++j) {
    subtotal = j->getCantidad() * j->getPrecio();
    suma += subtotal - (subtotal * 10) / 100;
  }
  return suma;
}

list<DtComun> DtMenu::obtenerListaProd() {
  return comun;
}

DtFecha::DtFecha() {
  time_t now = time(NULL);
  tm* timeinfo = localtime(&now);
  dia = timeinfo->tm_mday;
  mes = timeinfo->tm_mon + 1;
  anio = timeinfo->tm_year + 1900;
}

DtFecha::DtFecha(int d, int m, int a) {
  dia = d;
  mes = m;
  anio = a;
}

int DtFecha::getDia() {
  return dia;
}

int DtFecha::getMes() {
  return mes;
}

int DtFecha::getAnio() {
  return anio;
}

bool DtFecha::operator==(DtFecha f) {
  return this->anio == f.anio && this->mes == f.mes && this->dia == f.dia;
}

DtHora::DtHora() {
  time_t now = time(NULL);
  tm* timeinfo = localtime(&now);
  hora = timeinfo->tm_hour;
  minuto = timeinfo->tm_min;
  segundo = timeinfo->tm_sec;
}

DtHora::DtHora(int h, int m, int s) {
  hora = h;
  minuto = m;
  segundo = s;
}

int DtHora::getHora() {
  return hora;
}

int DtHora::getMinuto() {
  return minuto;
}

int DtHora::getSegundo() {
  return segundo;
}

DtEmpleado::DtEmpleado(int ident, string name) {
  id = ident;
  nombre = name;
}

DtEmpleado::~DtEmpleado() { }

int DtEmpleado::getID() {
  return id;
}

string DtEmpleado::getNombre() {
  return nombre;
}

DtMozo::DtMozo(int ident, string name) : DtEmpleado(ident, name) { }

DtRepartidor::DtRepartidor(int ident, string name, string veh) : DtEmpleado(ident, name) {
  vehiculo = veh;
}

string DtRepartidor::getVehiculo() {
  return vehiculo;
}

DtFactura::DtFactura(int cod, DtEmpleado* empl, list<DtProducto*> prods, float subt, int desc) {
  codigo = cod;
  empleado = empl;
  consumidos = prods;
  subtotal = subt;
  descuento = desc;
  // Calcular total y monto con IVA
  total = subtotal - (subtotal * descuento) / 100;
  monto_iva = total + (total * 22) / 100;
}

DtDireccion::DtDireccion(string street, int door, string corner1, string corner2) {
  calle = street;
  puerta = door;
  esquina1 = corner1;
  esquina2 = corner2;
}

string DtDireccion::getCalle() {
  return calle;
}

int DtDireccion::getPuerta() {
  return puerta;
}

string DtDireccion::getEsquina1() {
  return esquina1;
}

string DtDireccion::getEsquina2() {
  return esquina2;
}

DtEdificio::DtEdificio(string street, int door, string corner1, string corner2, string name, int apart) :
            DtDireccion(street, door, corner1, corner2) {
  nombre = name;
  apartamento = apart;
}

string DtEdificio::getNombre() {
  return nombre;
}

int DtEdificio::getApartamento() {
  return apartamento;
}

DtVentasAsig::DtVentasAsig(int tel, Estado status) {
  telefono_cliente = tel;
  estado = status;
}

int DtVentasAsig::getTelCliente() {
  return telefono_cliente;
}

string DtVentasAsig::getEstado() {
  return estadoAStr(estado);
}

DtInfoCliente::DtInfoCliente(int phone, string name, list<DtProducto*> prod, string repartidor, list<DtActualizacion> actu) {
  telefono = phone;
  nombre = name;
  producto = prod;
  nombreRepart = repartidor;
  updates = actu;
}

int DtInfoCliente::getTelefono() {
  return telefono;
}

string DtInfoCliente::getNombre() {
  return nombre;
}

list<DtProducto*> DtInfoCliente::getListaProductos() {
  return producto;
}

string DtInfoCliente::getNombreRepart() {
  return nombreRepart;
}

void DtInfoCliente::agregarActualizacion(DtActualizacion act) {
  updates.push_back(act);
}

list<DtActualizacion> DtInfoCliente::getUpdates() {
  return updates;
}

DtActualizacion::DtActualizacion(Estado status) {
  estado = status;
}

DtActualizacion::DtActualizacion(Estado status, DtFecha date, DtHora hour) {
  estado = status;
  fecha = date;
  hora = hour;
}

DtFecha DtActualizacion::getFecha() {
  return fecha;
}

DtHora DtActualizacion::getHora() {
  return hora;
}

string DtActualizacion::getEstado() {
  return estadoAStr(estado);
}
