#include <list>
#include <cstddef>

#include "../include/venta.h"
#include "../include/cliente.h"
#include "../include/fabrica.h"
#include "../include/iventa.h"
#include "../include/icliente.h"
#include "../include/iempleado.h"

ControladorVenta* ControladorVenta::instancia = NULL;

ControladorVenta* ControladorVenta::getInstance() {
  if (instancia == NULL)
    instancia = new ControladorVenta();
  return instancia;
}

ControladorVenta::ControladorVenta() {
  num_venta = 10;
}

void ControladorVenta::crearVentaLocal(int id_mozo, list<int> id_mesas) {
  Factory* fabrica = new Factory();
  IEmpleado* ie = fabrica->obtenerIEmpleado();
  Mozo* mozo = ie->obtenerMozo(id_mozo);

  num_venta++;
  Local* nuevaV = new Local(num_venta, mozo);

  for (list<int>::iterator j = id_mesas.begin(); j != id_mesas.end(); ++j) {
    Mesa* mesa = mozo->obtenerMesa(*j);
    nuevaV->agregarMesa(mesa);
  }

  ventas.push_back(nuevaV);
  delete fabrica;
}

Local* ControladorVenta::VentaLocalNoFact(int num_mesa) {
  for (list<Venta*>::iterator i = ventas.begin(); i != ventas.end(); ++i) {
    Local* venta = dynamic_cast<Local*>(*i);
    if (venta != NULL && !venta->estaFacturada() && venta->contieneMesa(num_mesa))
       return venta;
  }
  return NULL;
}

A_Domicilio* ControladorVenta::VentaADNoFact(int telefono) {
  for (list<Venta*>::iterator i = ventas.begin(); i != ventas.end(); ++i) {
    A_Domicilio* venta = dynamic_cast<A_Domicilio*>(*i);
    if (venta != NULL && !venta->estaFacturada() && venta->esDelCliente(telefono))
      return venta;
  }
  return NULL;
}

void ControladorVenta::agregarProducto(int num_mesa, string cod_prod, int cant) {
  Local* venta = this->VentaLocalNoFact(num_mesa);
  if (venta != NULL)
    venta->agregarProducto(cod_prod, cant);
}

void ControladorVenta::agregarProductoAD(int telefono, string cod_prod, int cant) {
  A_Domicilio* venta = this->VentaADNoFact(telefono);
  if (venta != NULL)
    venta->agregarProducto(cod_prod, cant);
}

void ControladorVenta::cambiarEstado(int telefono, Estado estado) {
  for (list<Venta*>::iterator i = ventas.begin(); i != ventas.end(); ++i) {
    A_Domicilio* venta = dynamic_cast<A_Domicilio*>(*i);
    if (venta != NULL && venta->esDelCliente(telefono) && venta->getEstado() != Recibido && venta->getEstado() != Cancelado)
      venta->setEstado(estado);
  }
}

void ControladorVenta::asignarRepartidor(int telefono, int id_rep) {
  Factory* fabrica = new Factory();
  IEmpleado* ie = fabrica->obtenerIEmpleado();
  Repartidor* rep = ie->obtenerRepartidor(id_rep);

  A_Domicilio* venta = this->VentaADNoFact(telefono);
  if (venta != NULL)
    venta->setRepartidor(rep);
  delete fabrica;
}

list<DtProducto*> ControladorVenta::listarProductos(int num_mesa) {
  list<DtProducto*> res;

  Local* venta = this->VentaLocalNoFact(num_mesa);
  if (venta != NULL)
    res = venta->listarProductos();

  return res;
}

void ControladorVenta::quitarProducto(int num_mesa, string cod_prod, int cant) {
  Local* venta = this->VentaLocalNoFact(num_mesa);
  if (venta != NULL)
    venta->quitarProducto(cod_prod, cant);
}

DtFactura* ControladorVenta::facturarVenta(int num_mesa, int descuento) {
  Local* venta = this->VentaLocalNoFact(num_mesa);
  venta->facturar(descuento);
  return venta->obtenerFactura();
}

DtFactura* ControladorVenta::facturarVentaAD(int telefono) {
  A_Domicilio* venta = this->VentaADNoFact(telefono);
  venta->facturar();
  return venta->obtenerFactura();
}

void ControladorVenta::crearVentaDom(int tel) {
  num_venta++;
  Factory* fabrica = new Factory();
  ICliente* ic = fabrica->obtenerICliente();
  Cliente* cliente = ic->obtenerCliente(tel);
  A_Domicilio* nuevaV = new A_Domicilio(num_venta, cliente);
  ventas.push_back(nuevaV);
  delete fabrica;
}

list<DtFactura*> ControladorVenta::obtenerFacturas(DtFecha fecha) {
  list<DtFactura*> res;
  for (list<Venta*>::iterator i = ventas.begin(); i != ventas.end(); ++i) {
    DtFactura* fact = (*i)->obtenerFactura();
    if (fact != NULL && fact->fecha == fecha)
      res.push_back(fact);
  }
  return res;
}

list<DtVentasAsig> ControladorVenta::obtenerVentas(int id_rep) {
  list<DtVentasAsig> res;
  for (list<Venta*>::iterator i = ventas.begin(); i != ventas.end(); ++i) {
    A_Domicilio* venta = dynamic_cast<A_Domicilio*>(*i);
    if (venta != NULL && venta->getIdRepartidor() == id_rep) {
      Estado est = venta->getEstado();
      if (est != Recibido && est != Cancelado) {
        DtVentasAsig datos(venta->getTelCliente(), est);
        res.push_back(datos);
      }
    }
  }
  return res;
}

Estado ControladorVenta::getEstadoVentaAD(int telefono) {
  for (list<Venta*>::iterator i = ventas.begin(); i != ventas.end(); ++i) {
    A_Domicilio* venta = dynamic_cast<A_Domicilio*>(*i);
    if (venta != NULL && venta->esDelCliente(telefono))
      return venta->getEstado();
  }
  return Cancelado;
}

int ControladorVenta::cantidadProdsVendidos(string codigo) {
  int cant_vendidos = 0;
  for (list<Venta*>::iterator i = ventas.begin(); i != ventas.end(); ++i) {
    Venta* venta = *i;
    if (venta->estaFacturada())
      cant_vendidos += venta->cantidadProds(codigo);
  }
  return cant_vendidos;
}

bool ControladorVenta::estanFacturadas(string codigo) {
  bool res = true; // Si no hay ninguna venta retorna true
  for (list<Venta*>::iterator i = ventas.begin(); i != ventas.end() && res; ++i) {
    Venta* venta = *i;
    if (!venta->estaFacturada() && venta->contieneProd(codigo))
      res = false;
  }
  return res;
}

bool ControladorVenta::estanVentasLocalesFact() {
  bool res = true; // Si no hay ninguna venta retorna true
  for (list<Venta*>::iterator i = ventas.begin(); i != ventas.end() && res; ++i) {
    Venta* venta = *i;
    if (!venta->estaFacturada())
      res = false;
  }
  return res;
}

bool ControladorVenta::tieneVentaEnCurso(int num_mesa) {
  Local* venta = this->VentaLocalNoFact(num_mesa);
  return venta != NULL;
}

list<DtInfoCliente> ControladorVenta::obtenerActualCliente(int telefono) {
  list<DtInfoCliente> res;
  for (list<Venta*>::iterator i = ventas.begin(); i != ventas.end(); ++i) {
    A_Domicilio* venta = dynamic_cast<A_Domicilio*>(*i);
    if (venta != NULL && venta->esDelCliente(telefono))
      res.push_back(venta->getInfoCliente());
  }
  return res;
}

list<DtInfoCliente> ControladorVenta::obtenerActualAdmin() {
  list<DtInfoCliente> res;
  for (list<Venta*>::iterator i = ventas.begin(); i != ventas.end(); ++i) {
    A_Domicilio* venta = dynamic_cast<A_Domicilio*>(*i);
    if (venta != NULL)
      res.push_back(venta->getInfoCliente());
  }
  return res;
}

bool ControladorVenta::contieneAlgunMenuVL(int num_mesa) {
  bool res = false;
  Local* venta = this->VentaLocalNoFact(num_mesa);
  if (venta != NULL)
    res = venta->contieneAlgunMenu();
  return res;
}

void ControladorVenta::cargarDatosDePrueba() {
  Factory* fabrica = new Factory();
  ICliente* ic = fabrica->obtenerICliente();
  IEmpleado* ie = fabrica->obtenerIEmpleado();

  Repartidor* rep1 = ie->obtenerRepartidor(4);
  Repartidor* rep2 = ie->obtenerRepartidor(5);
  DtFecha fecha(16, 06, 2019);

  Cliente* cliente1 = ic->obtenerCliente(91651249);

  A_Domicilio* venta2 = new A_Domicilio(5, cliente1); // Martin
  venta2->agregarProducto("2", 2);
  venta2->setRepartidor(rep2);
  venta2->facturar();
  venta2->setEstado(Pedido, fecha, DtHora(15, 02, 00));
  venta2->setEstado(EnCamino, fecha, DtHora(16, 15, 00));
  venta2->setEstado(Recibido, fecha, DtHora(16, 37, 00));
  ventas.push_back(venta2);

  A_Domicilio* venta1 = new A_Domicilio(4, cliente1); // Martin
  venta1->agregarProducto("7", 5);
  venta1->setRepartidor(rep1);
  venta1->facturar();
  venta1->setEstado(Pedido, fecha, DtHora(21, 13, 00));
  venta1->setEstado(EnCamino, fecha, DtHora(21, 27, 00));
  ventas.push_back(venta1);

  Cliente* cliente2 = ic->obtenerCliente(98217523);
  A_Domicilio* venta3 = new A_Domicilio(6, cliente2); // Vladimir
  venta3->agregarProducto("6", 1);
  venta3->setRepartidor(rep1);
  venta3->facturar();
  venta3->setEstado(Pedido, fecha, DtHora(21, 14, 00));
  venta3->setEstado(EnCamino, fecha, DtHora(21, 27, 00));
  ventas.push_back(venta3);

  delete fabrica;
}
