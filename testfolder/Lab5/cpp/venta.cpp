#include <list>
#include <string>

#include "venta.h"
#include "fabrica.h"
#include "producto.h"
#include "iproducto.h"
#include "datatypes.h"

Venta::Venta(int num) {
  numero = num;
  factura = NULL; // No esta facturada
}

Venta::~Venta() { }

int Venta::getNumero() {
  return numero;
}

float Venta::getSubtotal() {
  float subtotal = 0;
  for (list<V_P*>::iterator i = productos.begin(); i != productos.end(); ++i) {
    DtProducto* prod = (*i)->obtenerProducto();
    subtotal += (*i)->getCantidad() * prod->getPrecio();
    delete prod;
  }
  return subtotal;
}

bool Venta::estaFacturada() {
  return factura != NULL;
}

void Venta::setFactura(DtFactura* fact) {
  factura = fact;
}

DtFactura* Venta::obtenerFactura() {
  return factura;
}

void Venta::agregarProducto(string cod_prod, int cant) {
  bool encontrado = false;
  for (list<V_P*>::iterator i = productos.begin(); i != productos.end() && !encontrado; ++i)
    if ((*i)->getCodigo() == cod_prod) {
      (*i)->incrementarCant(cant);
      encontrado = true;
    }
  if (!encontrado) {
    Factory* fabrica = new Factory();
    IProducto* ip = fabrica->obtenerIProducto();
    V_P* vp = new V_P(ip->obtenerProducto(cod_prod), cant);
    productos.push_back(vp);
    delete fabrica;
  }
}

void Venta::quitarProducto(string cod_prod, int cant) {
  list<V_P*>::iterator it = productos.begin();

  while (it != productos.end()) {
    if ((*it)->getCodigo() == cod_prod) {
      (*it)->incrementarCant(-cant);

      if ((*it)->getCantidad() <= 0) {
        V_P* borrar = *it;
        it = productos.erase(it); // Ir al siguente del que borre
        delete borrar;
      } else
        ++it;

    } else
      ++it;
  }
}

list<DtProducto*> Venta::listarProductos() {
  list<DtProducto*> res;
  for (list<V_P*>::iterator i = productos.begin(); i != productos.end(); ++i)
    res.push_back((*i)->obtenerProducto());
  return res;
}

int Venta::cantidadProds(string cod_prod) {
  int cant = 0;
  if (factura != NULL) {
    list<DtProducto*> prods = factura->consumidos;
    for (list<DtProducto*>::iterator i = prods.begin(); i != prods.end(); ++i)
      if ((*i)->getCodigo() == cod_prod)
        cant += (*i)->getCantidad();
  }
  return cant;
}

// Retorna true si y solo si contiene al producto con codigo cod_prod
bool Venta::contieneProd(string cod_prod) {
  int res = false; // Si no tiene productos retorna false
  for (list<V_P*>::iterator i = productos.begin(); i != productos.end() && !res; ++i)
    if ((*i)->getCodigo() == cod_prod)
      res = true;
  return res;
}

bool Venta::contieneAlgunMenu() {
  bool contiene = false;
  for (list<V_P*>::iterator i = productos.begin(); i != productos.end(); ++i) {
    DtProducto* prod = (*i)->obtenerProducto();
    if (dynamic_cast<DtMenu*>(prod) != NULL) {
      contiene = true;
    }
    delete prod;
  }
  return contiene;
}

A_Domicilio::A_Domicilio(int num, Cliente* client) : Venta(num) {
  cliente = client;
  estado = Pedido;
  repartidor = NULL;
}

void A_Domicilio::setEstado(Estado state) {
  estado = state;
  DtActualizacion actu(estado);
  cambiosEstado.push_back(actu);  
}

void A_Domicilio::setEstado(Estado state, DtFecha fecha, DtHora hora) {
  estado = state;
  DtActualizacion actu(estado, fecha, hora);
  cambiosEstado.push_back(actu);  
}

bool A_Domicilio::esDelCliente(int telefono) {
  return cliente->getTelefono() == telefono;
}

Estado A_Domicilio::getEstado() {
  return estado;
}

int A_Domicilio::getIdRepartidor() {
  return repartidor->getID();
}

int A_Domicilio::getTelCliente() {
  return cliente->getTelefono();
}

void A_Domicilio::setRepartidor(Repartidor* rep) {
  repartidor = rep;
}

DtInfoCliente A_Domicilio::getInfoCliente() {
  string nombre_rep = "No tiene.";
  if (repartidor != NULL)
    nombre_rep = repartidor->getNombre();
  // Obtiene los productos de la factura ya que podrian haber sido borrados del sistema
  return DtInfoCliente(this->getTelCliente(), cliente->getNombre(), this->obtenerFactura()->consumidos, nombre_rep, cambiosEstado);
}

Local::Local(int num, Mozo* mozo_asig) : Venta(num) {
  mozo = mozo_asig;
}

void Local::agregarMesa(Mesa* mesa) {
  mesas.push_back(mesa);
}

bool Local::contieneMesa(int num_mesa) {
  bool encontrado = false;
  for (list<Mesa*>::iterator i = mesas.begin(); i != mesas.end() && !encontrado; ++i)
    encontrado = (*i)->obtenerIdMesa() == num_mesa;
  return encontrado;
}

void Local::facturar(int desc) {
  DtMozo* empl = new DtMozo(mozo->getID(), mozo->getNombre());
  float subtotal = this->getSubtotal();

  DtFactura* fact = new DtFactura(this->getNumero(), empl, this->listarProductos(), subtotal, desc);
  
  this->setFactura(fact);
}

void A_Domicilio::facturar() {
  DtRepartidor* empl;
  if (repartidor != NULL)
    empl = new DtRepartidor(repartidor->obtenerRepartidor());
  else
    empl = NULL;

  float subtotal = this->getSubtotal();
  DtFactura* fact = new DtFactura(this->getNumero(), empl, this->listarProductos(), subtotal, 0);
  
  this->setFactura(fact);
}
