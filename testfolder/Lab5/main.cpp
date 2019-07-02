#include <cstdio>
#include <string>
#include <climits>
#include <iostream>
#include <stdexcept>

#include "include/fabrica.h"
#include "include/datatypes.h"

using namespace std;

void MostrarMenuPrincipal() {
  cout << "\n\t---- MENU PRINCIPAL ----\n";
  cout << "\t1) Administrador\n";
  cout << "\t2) Mozo\n";
  cout << "\t3) Repartidor\n";
  cout << "\t4) Cliente\n";
  cout << "\t5) Cargar datos de prueba\n";
  cout << "\t0) Salir\n";
}

void leerEntero(int& numero, int max) {
  cin >> numero;
  while (cin.fail()) {
    cin.clear();
    cin.ignore(INT_MAX, '\n');
    cout << "No es un numero. Vuelve a intentarlo: ";
    cin >> numero;
  }
  if (numero > max) {
    cout << "El numero no debe ser mayor que " << max << ". Vuelve a intentarlo: ";
    leerEntero(numero, max);
  } else
    cin.ignore(INT_MAX, '\n'); // Limpiar el Enter despues del numero por si se lee un string despues
}

void leerReal(float& numero) {
  cin >> numero;
  while (cin.fail()) {
    cin.clear();
    cin.ignore(INT_MAX, '\n');
    cout << "No es un numero. Vuelve a intentarlo: ";
    cin >> numero;
  }
}

void leerString(string& nombre) {
  bool valido;
  do {
    valido = true;
    getline(cin, nombre);
    if (nombre.length() == 0) {
      cout << "La cadena no puede ser vacia. Vuelve a intentarlo: ";
      valido = false;
    }
  } while (!valido);
}

#define VERDE  "\x1b[92m"
#define AZUL   "\x1b[94m"
#define NORMAL "\x1b[0m"

int main() {
  printf(VERDE "                                                     " AZUL ",,\n");
  printf(VERDE "`7MM\"\"\"Mq.                    mm          " AZUL "`7MM\"\"\"YMM db\n");
  printf(VERDE "  MM   `MM.                   MM            " AZUL "MM    `7\n");
  printf(VERDE "  MM   ,M9  .gP\"Ya  ,pP\"Ybd mmMMmm ,pW\"Wq." AZUL "  MM   d `7MM  `7MMpMMMb.  .P\"Ybmmm\n");
  printf(VERDE "  MMmmdM9  ,M'   Yb 8I   `\"   MM  6W'   `Wb " AZUL "MM\"\"MM   MM    MM    MM :MI  I8\n");
  printf(VERDE "  MM  YM.  8M\"\"\"\"\"\" `YMMMa.   MM  8M     M8 " AZUL "MM   Y   MM    MM    MM  WmmmP\"\n");
  printf(VERDE "  MM   `Mb.YM.    , L.   I8   MM  YA.   ,A9 " AZUL "MM       MM    MM    MM 8M\n");
  printf(VERDE ".JMML. .JMM.`Mbmmd' M9mmmP'   `Mbmo`Ybmd9'" AZUL ".JMML.   .JMML..JMML  JMML.YMMMMMb\n");
  printf(VERDE "                                                                    " AZUL "6'     dP\n");
  printf(VERDE "                                                                    " AZUL "Ybmmmd'\n");
  printf(NORMAL "");

  DtFecha fecha;
  DtHora hora;
  printf("%02d/%02d/%d %02d:%02d\n", fecha.getDia(), fecha.getMes(), fecha.getAnio(), hora.getHora(), hora.getMinuto());

  Factory* fabrica = new Factory();
  IProducto* ip = fabrica->obtenerIProducto();
  IEmpleado* ie = fabrica->obtenerIEmpleado();
  ICliente* ic = fabrica->obtenerICliente();
  IVenta* iv = fabrica->obtenerIVenta();

  int opcion = 0;
  int subopcion = 0;
  bool salir = false;
  MostrarMenuPrincipal();

  do {
    cout << "\nOpcion: ";
    leerEntero(opcion, 5); // Hay 5 opciones en el menu principal

    switch (opcion) {
    case 1: {
      do {
        cout << "\n\t---- MENU DEL ADMINISTRADOR ----\n";
        cout << "\t1) Alta producto\n";
        cout << "\t2) Informacion de un producto\n";
        cout << "\t3) Baja producto\n";
        cout << "\t4) Alta empleado\n";
        cout << "\t5) Asignar mozos a mesas\n";
        cout << "\t6) Resumen facturacion de 1 dia dada la fecha\n";
        cout << "\t7) Venta a domicilio\n";
        cout << "\t8) Consultar actualizaciones de pedidos a domicilio (Administrador)\n";
        cout << "\t0) Volver al menu principal\n";
        cout << "\nOpcion: ";
        leerEntero(subopcion, 8);

        switch (subopcion) {
        case 1: {
          int subsubopcion;
          do {
            cout << "\nElija una opcion: \n";
            if (ip->cantProdComun() > 0)
              cout << "\t1) Agregar Menu\n"; // Solo se muestra si hay productos comunes registrados.
            cout << "\t2) Agregar Comun\n";
            cout << "\t0) Volver al menu anterior\n";
            cout << "\nOpcion: ";
            leerEntero(subsubopcion, 2);

            if (subsubopcion == 1 && ip->cantProdComun() > 0) { // Checkea que no ingrese una opcion oculta
              // Agregar Menu
              string codigoMenu, descripcion;
              cout << "Ingrese el codigo del menu: ";
              leerString(codigoMenu);
              cout << "Ingrese la descripcion del menu: ";
              leerString(descripcion);

              try {
                // Crear Menu
                ip->agregarMenu(codigoMenu, descripcion);

                // Listar Productos Comunes
                cout << "\nLos productos registrados son:\n";
                list<DtComun> productos = ip->listarComunes();
                for (list<DtComun>::iterator it = productos.begin(); it != productos.end(); ++it)
                  cout << "\t[" << it->getCodigo() << "] \"" << it->getDescripcion() << "\" $" << it->getPrecio() << "\n";
                cout << "\n";

                string codigo;
                bool agregarMas;
                do {
                  int cantidad;
                  cout << "Ingrese el codigo del producto: ";
                  leerString(codigo);
                  cout << "Ingrese la cantidad del producto: ";
                  leerEntero(cantidad, INT_MAX);

                  // Agregar producto al Menu
                  ip->agregarAlMenu(codigo, cantidad);

                  cout << "Quiere agregar mas productos? 1) Si 0) No\n";
                  cin >> agregarMas;
                  cin.ignore(INT_MAX, '\n');
                } while (agregarMas);

                bool confirma;
                cout << "Quiere confirmar? 1) Confirma 0) Cancela\n";
                cin >> confirma;
                cin.ignore(INT_MAX, '\n');

                if (!confirma)
                  ip->quitarMenu(codigoMenu); // Si cancela se quita el menu que se creo
              } catch (const std::invalid_argument& e) {
                cout << "No se ha agregado el menu, porque:\n";
                printf("%s\n", e.what());
              }
            } else if (subsubopcion == 2) {
              // Agregar Productos Comunes
              string codigo, descripcion;
              float precio;
              cout << "Ingrese el codigo del producto: ";
              leerString(codigo);
              cout << "Ingrese la descripcion del producto: ";
              leerString(descripcion);
              cout << "Ingrese el precio del producto: ";
              leerReal(precio);

              bool confirma;
              cout << "Quiere confirmar? 1) Confirma 0) Cancela\n";
              cin >> confirma;
              cin.ignore(INT_MAX, '\n');

              if (confirma) {
                try {
                  ip->agregarComun(codigo, descripcion, precio);
                } catch (const std::invalid_argument& e) {
                  cout << "No se ha agregado el producto, porque:\n";
                  printf("%s\n", e.what());
                }
              }
            }
          } while (subsubopcion != 0);
          break;
        }
        case 2: {
          cout << "\nLos productos registrados son:\n";
          list<DtComun> productos = ip->listarComunes();
          for (list<DtComun>::iterator it = productos.begin(); it != productos.end(); ++it)
            cout << "\t[" << it->getCodigo() << "] \"" << it->getDescripcion() << "\"\n";
          list<DtMenu> menus = ip->listarMenus();
          for (list<DtMenu>::iterator it = menus.begin(); it != menus.end(); ++it)
            cout << "\t[" << it->getCodigo() << "] \"" << it->getDescripcion() << "\" Menu\n";

          string codigo;
          bool error = false;
          bool continuar = false;
          int cant_vendidos;

          do {
            cout << "\nIngrese codigo del producto a consular: ";
            leerString(codigo);

            try {
              DtProducto* prod = ip->obtenerProducto(codigo, cant_vendidos);
              if (dynamic_cast<DtComun*>(prod) != NULL) {
                DtComun* comun = dynamic_cast<DtComun*>(prod);
                cout << "\t[" << comun->getCodigo() << "] \"" << comun->getDescripcion() << "\" $" << comun->getPrecio() << "\n";
              } else {
                DtMenu* menu = dynamic_cast<DtMenu*>(prod);
                cout << "\t[" << menu->getCodigo() << "] \"" << menu->getDescripcion() << "\" $" << menu->getPrecio() << " Contiene:\n";
                list<DtComun> productos = menu->obtenerListaProd();
                for (list<DtComun>::iterator j = productos.begin(); j != productos.end(); ++j)
                  cout << "\t\t[" << j->getCodigo() << "] \"" << j->getDescripcion() << "\" $" << j->getPrecio() << " Cantidad: " << j->getCantidad() << "\n";
              }
              cout << "Se han vendido: " << cant_vendidos << " unidades\n";
              delete prod;
            } catch (const std::invalid_argument& e) {
              cout << "Se ha detectado una excepcion: \n";
              printf("%s\n", e.what());
              error = true;
              cout << "Quiere probar otro codigo? 1) Si 0) No\n";
              cin >> continuar;
              cin.ignore(INT_MAX, '\n');
            }
          } while (error && continuar);
          break;
        }
        case 3: {
          // Baja producto
          cout << "\nLos productos registrados son:\n";
          list<DtComun> productos = ip->listarComunes();
          for (list<DtComun>::iterator it = productos.begin(); it != productos.end(); ++it)
            cout << "\t[" << it->getCodigo() << "] \"" << it->getDescripcion() << "\"\n";
          list<DtMenu> menus = ip->listarMenus();
          for (list<DtMenu>::iterator it = menus.begin(); it != menus.end(); ++it)
            cout << "\t[" << it->getCodigo() << "] \"" << it->getDescripcion() << "\" Menu\n";

          string codigo;
          cout << "\nIngrese codigo del producto a eliminar: ";
          leerString(codigo);

          try {
            ip->borrarProducto(codigo);
            cout << "Se ha borrado el producto.\n";
          } catch (const std::invalid_argument& e) {
            cout << "No se ha borrado el producto, porque:\n";
            printf("%s\n", e.what());
          }
          break;
        }
        case 4: {
          bool agregarMas;
          do {
            // Alta empleado
            string nombre;
            cout << "\nIngrese el nombre del empleado: ";
            leerString(nombre);

            int subsubopcion;
            cout << "\nElija una opcion: \n";
            cout << "\t1) Agregar Mozo\n";
            cout << "\t2) Agregar Repartidor\n";
            cout << "\nOpcion: ";
            leerEntero(subsubopcion, 2);

            int transporte = 0;
            if (subsubopcion == 2) {
              cout << "\nElija el medio de transporte: \n";
              cout << "\t1) Pie\n";
              cout << "\t2) Bicicleta\n";
              cout << "\t3) Moto\n";
              cout << "\nOpcion: ";
              leerEntero(transporte, 3);
            }

            bool confirma;
            cout << "Quiere confirmar? 1) Confirma 0) Cancela\n";
            cin >> confirma;
            cin.ignore(INT_MAX, '\n');

            if (confirma) {
              if (transporte == 1)
                ie->agregarRepartidor(nombre, PIE);
              else if (transporte == 2)
                ie->agregarRepartidor(nombre, BICI);
              else if (transporte == 3)
                ie->agregarRepartidor(nombre, MOTO);
              else
                ie->agregarMozo(nombre);
            }
            cout << "Quiere agregar mas empleados? 1) Si 0) No\n";
            cin >> agregarMas;
            cin.ignore(INT_MAX, '\n');
          } while (agregarMas);
          break;
        }
        case 5: {
          try {
            // Asignar mozos a mesas
            ie->asignarMesas();
            // Mostrar mesas asignadas a cada mozo
            list<int> lista_mozos = ie->listarMozos();
            for (list<int>::iterator it = lista_mozos.begin(); it != lista_mozos.end(); ++it) {
              list<int> lista_mesas = ie->obtenerMesas(*it);
              cout << "Mozo " << *it << ":\n";
              for (list<int>::iterator j = lista_mesas.begin(); j != lista_mesas.end(); ++j) {
                cout << "\tMesa " << *j << "\n";
              }
            }
          } catch (const std::invalid_argument& e) {
            cout << "No se ha podido asignar mesas, porque:\n";
            printf("%s\n", e.what());
          }
          break;
        }
        case 6: {
          // Resumen facturacion de 1 dia dada la fecha
          cout << "\nIngrese la fecha a consular (Separada por Espacio o Enter): ";
          int dia, mes, anio;
          scanf("%d %d %d", &dia, &mes, &anio);
          DtFecha fecha(dia, mes, anio);
          printf("Se ingreso: %02d/%02d/%d\n", dia, mes, anio);
          
          // Obtener todas las facturas en esa fecha
          list<DtFactura*> facturas = iv->obtenerFacturas(fecha);
          float ganancia = 0;
          for (list<DtFactura*>::iterator f = facturas.begin(); f != facturas.end(); ++f) { 
            // Mostrar factura
            DtFactura* factura = *f;
            printf("\nVenta con codigo: %d", factura->codigo);
            printf(" en la Fecha: %02d/%02d/%d ", factura->fecha.getDia(), factura->fecha.getMes(), factura->fecha.getAnio());
            printf("%02d:%02d:%02d\n", factura->hora.getHora(), factura->hora.getMinuto(), factura->hora.getSegundo());

            DtEmpleado* empl = factura->empleado;
            DtRepartidor* repart = dynamic_cast<DtRepartidor*>(empl);
            if (repart != NULL)
              cout << "\nVendida por: " << repart->getNombre() << " en " << repart->getVehiculo() << "\n";
            else if (empl != NULL)
              cout << "\nVendida por: " << factura->empleado->getNombre() << "\n";
            else
              cout << "\nEl Cliente pidio para llevar\n";

            list<DtProducto*> productos = factura->consumidos;
            for (list<DtProducto*>::iterator it = productos.begin(); it != productos.end(); ++it) {
              DtProducto* prod = *it;
              if (dynamic_cast<DtComun*>(prod) != NULL) {
                DtComun* comun = dynamic_cast<DtComun*>(prod);
                cout << comun->getCantidad() << "\t[" << comun->getCodigo() << "] \"" << comun->getDescripcion() << "\" $" << comun->getPrecio() << "\n";
              } else {
                DtMenu* menu = dynamic_cast<DtMenu*>(prod);
                cout << menu->getCantidad() << "\t[" << menu->getCodigo() << "] \"" << menu->getDescripcion() << "\" $" << menu->getPrecio() << " Contiene:\n";
                list<DtComun> productos = menu->obtenerListaProd();
                for (list<DtComun>::iterator j = productos.begin(); j != productos.end(); ++j)
                  cout << "\t\t[" << j->getCodigo() << "] \"" << j->getDescripcion() << "\" $" << j->getPrecio() << " Cantidad: " << j->getCantidad() << "\n";
                }
            }
            printf("\nSubtotal: $%g\nDescuento: %d%%\nTotal: $%g\nMonto (IVA 22%%): $%g\n\n", factura->subtotal, factura->descuento, factura->total, factura->monto_iva);
            ganancia += factura->monto_iva;
          }
          cout << "Se facturo $" << ganancia << " el ";
          printf("%02d/%02d/%d\n", dia, mes, anio);
          break;
        }
        case 7: {
          // Venta a domicilio
          int telefono;
          cout << "\nIngrese el telefono del cliente: ";
          leerEntero(telefono, INT_MAX);

          bool confirma_cliente = true; // Si el cliente esta registrado

          if (!ic->estaRegistrado(telefono)) {
            // REGISTRAR
            string nombre, calle, calleEsq1, calleEsq2;
            int nro_puerta;
            bool edificio;
            cout << "El cliente no esta registrado. Ingrese el nombre del mismo: ";
            leerString(nombre);
            cout << "Ingrese la calle: ";
            leerString(calle);
            cout << "Ingrese el numero de puerta: ";
            leerEntero(nro_puerta, INT_MAX);
            cout << "Ingrese las calles entre las que se ubica.\n";
            cout << "Calle uno: ";
            leerString(calleEsq1);
            cout << "Calle dos: ";
            leerString(calleEsq2);

            cout << "Es un edificio? 1) Si 0) No\n";
            cin >> edificio;
            cin.ignore(INT_MAX, '\n');
            
            DtDireccion* direccion;
            string nombre_edif;
            int nro_apartamento;
            if (edificio) {
              cout << "Ingrese nombre del edificio: ";
              leerString(nombre_edif);
              cout << "Ingrese numero de apartamento: ";
              leerEntero(nro_apartamento, INT_MAX);

              direccion = new DtEdificio(calle, nro_puerta, calleEsq1, calleEsq2, nombre_edif, nro_apartamento);
            } else
              direccion = new DtDireccion(calle, nro_puerta, calleEsq1, calleEsq2);

            cout << "Datos del cliente:\n";
            cout << "\tNombre: " << nombre << "\n";
            cout << "\tTelefono: " << telefono << "\n";
            cout << "\tCalle: " << calle << "\n";
            cout << "\tPuerta: " << nro_puerta << "\n";
            cout << "\tEntre: " << calleEsq1 << " y " << calleEsq2 << "\n";
            if (edificio) {
              cout << "\tEdificio: " << nombre_edif << "\n";
              cout << "\tApartamento: " << nro_apartamento << "\n";
            }

            cout << "Quiere agregar al cliente? 1) Si 0) No\n";
            cin >> confirma_cliente;
            cin.ignore(INT_MAX, '\n');

            if (confirma_cliente)
              ic->agregarCliente(telefono, nombre, direccion);
          }

          // Si el cliente no esta registrado y no se confima para agregarlo se omite la parte de crear venta a domicilio
          if (confirma_cliente) {
            cout << "\nLos productos registrados son:\n";
            list<DtComun> prods = ip->listarComunes();
            for (list<DtComun>::iterator it = prods.begin(); it != prods.end(); ++it)
              cout << "\t[" << it->getCodigo() << "] \"" << it->getDescripcion() << "\"\n";
            list<DtMenu> menus = ip->listarMenus();
            for (list<DtMenu>::iterator it = menus.begin(); it != menus.end(); ++it)
              cout << "\t[" << it->getCodigo() << "] \"" << it->getDescripcion() << "\" Menu\n";

            // Crear venta a domicilio
            iv->crearVentaDom(telefono);

            string codigo;
            bool agregarMas;
            do {
              int cantidad;
              cout << "Ingrese el codigo del producto: ";
              leerString(codigo);
              cout << "Ingrese la cantidad del producto: ";
              leerEntero(cantidad, INT_MAX);

              // Agregar producto a venta a domicilio
              iv->agregarProductoAD(telefono, codigo, cantidad);

              cout << "Quiere agregar mas productos? 1) Si 0) No\n";
              cin >> agregarMas;
              cin.ignore(INT_MAX, '\n');
            } while (agregarMas);

             bool repartir;
            cout << "Elija tipo de venta: 1) Para Llevar 0) Entrega A Domicilio\n";
            cin >> repartir;
            cin.ignore(INT_MAX, '\n');
            if (repartir) {
              iv->cambiarEstado(telefono, Recibido);
            } else {
              list<DtRepartidor> repart = ie->listarRepartidores();
              for (list<DtRepartidor>::iterator it = repart.begin(); it != repart.end(); ++it)
                cout << it->getID() << " | " << it->getNombre() << " | " << it->getVehiculo() << "\n";
              int id_rep;
              cout << "Ingrese el id del repartidor: ";
              leerEntero(id_rep, INT_MAX);
              iv->asignarRepartidor(telefono, id_rep);
              iv->cambiarEstado(telefono, Pedido);
            }

            /*bool confirma;
            cout << "Quiere confirmar? 1) Si 0) No\n";
            cin >> confirma;
            cin.ignore(INT_MAX, '\n');

            if (confirma) {*/

            // Mostrar factura de la venta a domicilio
            DtFactura* factura = iv->facturarVentaAD(telefono);
            printf("Venta con codigo: %d", factura->codigo);
            printf(" en la Fecha: %02d/%02d/%d ", factura->fecha.getDia(), factura->fecha.getMes(), factura->fecha.getAnio());
            printf("%02d:%02d:%02d\n", factura->hora.getHora(), factura->hora.getMinuto(), factura->hora.getSegundo());

            DtEmpleado* empl = factura->empleado;
            DtRepartidor* repart = dynamic_cast<DtRepartidor*>(empl);
            if (repart != NULL) // Si tiene repartidor asignado
              cout << "\nVendida por: " << repart->getNombre() << " en " << repart->getVehiculo() << "\n";

            list<DtProducto*> productos = factura->consumidos;
            for (list<DtProducto*>::iterator it = productos.begin(); it != productos.end(); ++it) {
              DtProducto* prod = *it;
              if (dynamic_cast<DtComun*>(prod) != NULL) {
                DtComun* comun = dynamic_cast<DtComun*>(prod);
                cout << comun->getCantidad() << "\t[" << comun->getCodigo() << "] \"" << comun->getDescripcion() << "\" $" << comun->getPrecio() << "\n";
              } else {
                DtMenu* menu = dynamic_cast<DtMenu*>(prod);
                cout << menu->getCantidad() << "\t[" << menu->getCodigo() << "] \"" << menu->getDescripcion() << "\" $" << menu->getPrecio() << " Contiene:\n";
                list<DtComun> productos = menu->obtenerListaProd();
                for (list<DtComun>::iterator j = productos.begin(); j != productos.end(); ++j)
                  cout << "\t\t[" << j->getCodigo() << "] \"" << j->getDescripcion() << "\" $" << j->getPrecio() << " Cantidad: " << j->getCantidad() << "\n";
              }
            }
            printf("\nSubtotal: $%g\nDescuento: %d%%\nTotal: $%g\nMonto (IVA 22%%): $%g\n", factura->subtotal, factura->descuento, factura->total, factura->monto_iva);
          }
          break;
        }
        case 8: {
          // Consultar actualizaciones de pedidos a domicilio (ADMIN)

          // Obtener las actualizaciones de todos los usuarios
          list<DtInfoCliente> lstIC = iv->obtenerActualAdmin();

          for (list<DtInfoCliente>::iterator it = lstIC.begin(); it != lstIC.end(); ++it) {
            cout << "\nTelefono: " << it->getTelefono() << "\n";
            cout << "Nombre: " << it->getNombre() << "\n";
            cout << "Repartidor: " << it->getNombreRepart() << "\n";

            list<DtProducto*> productos = it->getListaProductos();
            for (list<DtProducto*>::iterator k = productos.begin(); k != productos.end(); ++k) {
              DtProducto* prod = *k;
              if (dynamic_cast<DtComun*>(prod) != NULL) {
                DtComun* comun = dynamic_cast<DtComun*>(prod);
                cout << comun->getCantidad() << "\t[" << comun->getCodigo() << "] \"" << comun->getDescripcion() << "\" $" << comun->getPrecio() << "\n";
              } else {
                DtMenu* menu = dynamic_cast<DtMenu*>(prod);
                cout << menu->getCantidad() << "\t[" << menu->getCodigo() << "] \"" << menu->getDescripcion() << "\" $" << menu->getPrecio() << " Contiene:\n";
                list<DtComun> productos = menu->obtenerListaProd();
                for (list<DtComun>::iterator j = productos.begin(); j != productos.end(); ++j)
                  cout << "\t\t[" << j->getCodigo() << "] \"" << j->getDescripcion() << "\" $" << j->getPrecio() << " Cantidad: " << j->getCantidad() << "\n";
              }
              //delete prod;
            }
            productos.clear();
            list<DtActualizacion> actus = it->getUpdates();
            for (list<DtActualizacion>::iterator j = actus.begin(); j != actus.end(); ++j) {
              DtFecha fecha = j->getFecha();
              DtHora hora = j->getHora();
              printf("%02d/%02d/%d %02d:%02d:%02d\n", fecha.getDia(), fecha.getMes(), fecha.getAnio(), hora.getHora(), hora.getMinuto(), hora.getSegundo());
              cout << "Estado: " << j->getEstado() << "\n\n";
            }
          }
          break;
        }
        } // sub-switch
      } while (subopcion != 0);
      MostrarMenuPrincipal();
      break;
    }
    case 2: {
      do {
        cout << "\n\t---- MENU DEL MOZO ----\n";
        cout << "\t1) Iniciar venta en mesas\n";
        cout << "\t2) Agregar producto a una venta\n";
        cout << "\t3) Quitar producto a una venta\n";
        cout << "\t4) Facturacion de una venta\n";
        cout << "\t0) Volver al menu principal\n";
        cout << "\nOpcion: ";

        leerEntero(subopcion, 4);
        switch (subopcion) {
        case 1: {
          list<int> lista_mozos = ie->listarMozos();
          for (list<int>::iterator it = lista_mozos.begin(); it != lista_mozos.end(); ++it)
            cout << "Mozo ID: " << *it << "\n";

          int id_mozo;
          cout << "\nIngrese su identificador: ";
          leerEntero(id_mozo, INT_MAX);
          list<int> lista_mesas = ie->obtenerMesas(id_mozo);
          for (list<int>::iterator j = lista_mesas.begin(); j != lista_mesas.end(); ++j)
            cout << "\tMesa " << *j << "\n";

          int mesa;
          list<int> mesas;
          bool agregarMas;

          do {
            cout << "\nIngrese el numero de la mesa: ";
            leerEntero(mesa, INT_MAX);
            mesas.push_back(mesa);
            cout << "Quiere agregar mas mesas? 1) Si 0) No\n";
            cin >> agregarMas;
            cin.ignore(INT_MAX, '\n');
          } while (agregarMas);

          bool confirma;
          cout << "Quiere confirmar? 1) Confirma 0) Cancela\n";
          cin >> confirma;
          cin.ignore(INT_MAX, '\n');

          if (confirma)
            iv->crearVentaLocal(id_mozo, mesas);
          break;
        }
        case 2: {
          // Agregar producto a una venta
          int num_mesa;
          cout << "\nIngrese el numero de mesa: ";
          leerEntero(num_mesa, INT_MAX);

          cout << "\nLos productos registrados son:\n";
          list<DtComun> productos = ip->listarComunes();
          for (list<DtComun>::iterator it = productos.begin(); it != productos.end(); ++it)
            cout << "\t[" << it->getCodigo() << "] \"" << it->getDescripcion() << "\"\n";
          list<DtMenu> menus = ip->listarMenus();
          for (list<DtMenu>::iterator it = menus.begin(); it != menus.end(); ++it)
            cout << "\t[" << it->getCodigo() << "] \"" << it->getDescripcion() << "\" Menu\n";

          string codigo;
          bool agregarMas;
          do {
            int cantidad;
            cout << "Ingrese el codigo del producto: ";
            leerString(codigo);
            cout << "Ingrese la cantidad del producto: ";
            leerEntero(cantidad, INT_MAX);

            iv->agregarProducto(num_mesa, codigo, cantidad);

            cout << "Quiere agregar mas productos? 1) Si 0) No\n";
            cin >> agregarMas;
            cin.ignore(INT_MAX, '\n');
          } while (agregarMas);
          break;
        }
        case 3: {
          // Quitar producto a una venta
          int num_mesa;
          cout << "\nIngrese el numero de mesa: ";
          leerEntero(num_mesa, INT_MAX);

          list<DtProducto*> productos = iv->listarProductos(num_mesa);
          for (list<DtProducto*>::iterator it = productos.begin(); it != productos.end(); ++it) {
            DtProducto* prod = *it;
            if (dynamic_cast<DtComun*>(prod) != NULL) {
              DtComun* comun = dynamic_cast<DtComun*>(prod);
              cout << comun->getCantidad() << "\t[" << comun->getCodigo() << "] \"" << comun->getDescripcion() << "\" $" << comun->getPrecio() << "\n";
            } else {
              DtMenu* menu = dynamic_cast<DtMenu*>(prod);
              cout << menu->getCantidad() << "\t[" << menu->getCodigo() << "] \"" << menu->getDescripcion() << "\" $" << menu->getPrecio() << " Contiene:\n";
              list<DtComun> productos = menu->obtenerListaProd();
              for (list<DtComun>::iterator j = productos.begin(); j != productos.end(); ++j)
                cout << "\t\t[" << j->getCodigo() << "] \"" << j->getDescripcion() << "\" $" << j->getPrecio() << " Cantidad: " << j->getCantidad() << "\n";
            }
            delete prod;
          }
          productos.clear();

          string codigo;
          bool quitarMas;
          do {
            int cantidad;
            cout << "Ingrese el codigo del producto a quitar: ";
            leerString(codigo);
            cout << "Ingrese la cantidad del producto a quitar: ";
            leerEntero(cantidad, INT_MAX);

            bool confirma;
            cout << "Quiere confirmar? 1) Confirma 0) Cancela\n";
            cin >> confirma;
            cin.ignore(INT_MAX, '\n');

            if (confirma)
              iv->quitarProducto(num_mesa, codigo, cantidad);

            cout << "Quiere quitar mas productos? 1) Si 0) No\n";
            cin >> quitarMas;
            cin.ignore(INT_MAX, '\n');
          } while (quitarMas);
          break;
        }
        case 4: {
          // Facturacion de una venta
          int num_mesa;
          cout << "\nIngrese el numero de mesa: ";
          leerEntero(num_mesa, INT_MAX);

          int descuento = 0; // Si la venta contiene un menu
          if (!iv->contieneAlgunMenuVL(num_mesa)) {
            cout << "\nIngrese el porcentaje de descuento: ";
            leerEntero(descuento, 100);
          }

          DtFactura* factura = iv->facturarVenta(num_mesa, descuento);

          // Mostrar factura venta local
          printf("Venta con codigo: %d", factura->codigo);
          printf(" en la Fecha: %02d/%02d/%d ", factura->fecha.getDia(), factura->fecha.getMes(), factura->fecha.getAnio());
          printf("%02d:%02d:%02d\n", factura->hora.getHora(), factura->hora.getMinuto(), factura->hora.getSegundo());
          cout << "\nVendida por: " << factura->empleado->getNombre() << "\n";
          list<DtProducto*> productos = factura->consumidos;
          for (list<DtProducto*>::iterator it = productos.begin(); it != productos.end(); ++it) {
            DtProducto* prod = *it;
            if (dynamic_cast<DtComun*>(prod) != NULL) {
              DtComun* comun = dynamic_cast<DtComun*>(prod);
              cout << comun->getCantidad() << "\t[" << comun->getCodigo() << "] \"" << comun->getDescripcion() << "\" $" << comun->getPrecio() << "\n";
            } else {
              DtMenu* menu = dynamic_cast<DtMenu*>(prod);
              cout << menu->getCantidad() << "\t[" << menu->getCodigo() << "] \"" << menu->getDescripcion() << "\" $" << menu->getPrecio() << " Contiene:\n";
              list<DtComun> productos = menu->obtenerListaProd();
              for (list<DtComun>::iterator j = productos.begin(); j != productos.end(); ++j)
                cout << "\t\t[" << j->getCodigo() << "] \"" << j->getDescripcion() << "\" $" << j->getPrecio() << " Cantidad: " << j->getCantidad() << "\n";
            }
          }
          printf("\nSubtotal: $%g\nDescuento: %d%%\nTotal: $%g\nMonto (IVA 22%%): $%g\n", factura->subtotal, factura->descuento, factura->total, factura->monto_iva);
          break;
        }
        } // sub-switch
      } while (subopcion != 0);
      MostrarMenuPrincipal();
      break;
    }
    case 3: {
      do {
        cout << "\n\t---- MENU DEL REPARTIDOR ----\n";
        cout << "\t1) Modificar estado pedido por parte del repartidor\n";
        cout << "\t0) Volver al menu principal\n";
        cout << "\nOpcion: ";

        leerEntero(subopcion, 1);
        switch (subopcion) {
        case 1: {
          // Modificar estado pedido
          list<DtRepartidor> repart = ie->listarRepartidores();
          for (list<DtRepartidor>::iterator it = repart.begin(); it != repart.end(); ++it)
            cout << it->getID() << " | " << it->getNombre() << " | " << it->getVehiculo() << "\n";

          int id_rep;
          cout << "\nIngrese su identificador: ";
          leerEntero(id_rep, INT_MAX);

          list<DtVentasAsig> ventas_asig = iv->obtenerVentas(id_rep);

          cout << "Telefono Cliente\t| Estado\n";
          for (list<DtVentasAsig>::iterator it = ventas_asig.begin(); it != ventas_asig.end(); ++it)
            cout << it->getTelCliente() << "\t\t| " << it->getEstado() << "\n";

          int telefono;
          cout << "\nIngrese el telefono del cliente cuya venta quiere cambiar de estado: ";
          leerEntero(telefono, INT_MAX);

          Estado estado = iv->getEstadoVentaAD(telefono);
          cout << "\n\t---- CAMBIAR DE ESTADO ----\n";
          if (estado == Pedido)
            cout << "\t1) Cambiar estado a En Camino\n";
          if (estado == EnCamino)
            cout << "\t2) Cambiar estado a Recibido\n";
          if (estado == Pedido || estado == EnCamino)
            cout << "\t3) Cambiar estado a Cancelado\n";
          cout << "\t0) Mantener el estado actual\n";

          int opcion_est;
          cout << "Elija una opcion: ";
          leerEntero(opcion_est, INT_MAX);
          switch (opcion_est) {
          case 1: {
            iv->cambiarEstado(telefono, EnCamino);
            break;
          }
          case 2: {
            iv->cambiarEstado(telefono, Recibido);
            break;
          }
          case 3: {
            iv->cambiarEstado(telefono, Cancelado);
            break;
          }
          }
          break;
        }
        } // sub-switch
      } while (subopcion != 0);
      MostrarMenuPrincipal();
      break;
    }
    case 4: {
      do {
        cout << "\n\t---- MENU DEL CLIENTE ----\n";
        cout << "\t1) Consultar actualizaciones de pedidos a domicilio (Cliente)\n";
        cout << "\t0) Volver al menu principal\n";
        cout << "\nOpcion: ";

        leerEntero(subopcion, 1);
        switch (subopcion) {
        case 1: {
          // Consultar actualizaciones de pedidos a domicilio (Cliente)
          int telefono;
          cout << "\nIngrese su telefono: ";
          leerEntero(telefono, INT_MAX);

          // Obtener actualizaciones de pedidos del cliente
          list<DtInfoCliente> lstIC = iv->obtenerActualCliente(telefono);

          for (list<DtInfoCliente>::iterator it = lstIC.begin(); it != lstIC.end(); ++it) {
            cout << "\nTelefono: " << it->getTelefono() << "\n";
            cout << "Nombre: " << it->getNombre() << "\n";
            cout << "Repartidor: " << it->getNombreRepart() << "\n";

            list<DtProducto*> productos = it->getListaProductos();
            for (list<DtProducto*>::iterator k = productos.begin(); k != productos.end(); ++k) {
              DtProducto* prod = *k;
              if (dynamic_cast<DtComun*>(prod) != NULL) {
                DtComun* comun = dynamic_cast<DtComun*>(prod);
                cout << comun->getCantidad() << "\t[" << comun->getCodigo() << "] \"" << comun->getDescripcion() << "\" $" << comun->getPrecio() << "\n";
              } else {
                DtMenu* menu = dynamic_cast<DtMenu*>(prod);
                cout << menu->getCantidad() << "\t[" << menu->getCodigo() << "] \"" << menu->getDescripcion() << "\" $" << menu->getPrecio() << " Contiene:\n";
                list<DtComun> productos = menu->obtenerListaProd();
                for (list<DtComun>::iterator j = productos.begin(); j != productos.end(); ++j)
                  cout << "\t\t[" << j->getCodigo() << "] \"" << j->getDescripcion() << "\" $" << j->getPrecio() << " Cantidad: " << j->getCantidad() << "\n";
              }
              //delete prod;
            }
            productos.clear();
            list<DtActualizacion> actus = it->getUpdates();
            for (list<DtActualizacion>::iterator j = actus.begin(); j != actus.end(); ++j) {
              DtFecha fecha = j->getFecha();
              DtHora hora = j->getHora();
              printf("%02d/%02d/%d %02d:%02d:%02d\n", fecha.getDia(), fecha.getMes(), fecha.getAnio(), hora.getHora(), hora.getMinuto(), hora.getSegundo());
              cout << "Estado: " << j->getEstado() << "\n\n";
            }
          }
          break;
        }
        } // sub-switch
      } while (subopcion != 0);
      MostrarMenuPrincipal();
      break;
    }
    case 5: {
      cout << "\tCargando datos de prueba...\n";
      ip->cargarDatosDePrueba();
      ie->cargarDatosDePrueba();
      ic->cargarDatosDePrueba();
      iv->cargarDatosDePrueba();

      MostrarMenuPrincipal();
      break;
    }
    case 0: {
      salir = true;
      break;
    }
    default: {
      MostrarMenuPrincipal();
      break;
    }
    } // switch
  } while (!salir);

  // Liberar memoria
  delete fabrica;

  return 0;
}
