#ifndef MOVIMIENTOSPEDIDO_H
#define MOVIMIENTOSPEDIDO_H

/*!
  \class MovimientosPedido
  \brief En esta clase se incluye los métodos y atributos utilizados para añadir o eliminar productos y kits a un pedido interno, y mostrar o modificar los datos del pedido interno.
*/

#include <QDialog>
#include "Productos/producto.h"
#include "movimientos.h"

namespace Ui {
    class MovimientosPedido;
}

class MovimientosPedido : public QDialog {
    Q_OBJECT
public:
    /*!
      \brief Constructor de la clase. Se asignan a los atributos el valor de los parámetros de entrada.
      \param codigo Parámetro que almacena el número de pedido interno de productos a mostrar en la ficha.
      \param parent Objeto de tipo QWidget que contiene un puntero al objeto padre de la clase. Por defecto es 0.
    */
    MovimientosPedido(int,QWidget *parent = 0);
    /*!
      \brief Destructor de la clase.
    */
    ~MovimientosPedido();

    /*!
      \brief Carga los datos del pedido interno de productos en la ficha
    */
    void CargarDatos();
protected:
    /*!
      \brief Controlador de eventos que maneja los cambios de estado.
    */
    void changeEvent(QEvent *e);

public slots:
    /*!
      \brief Abre el cuadro de dialogo para seleccionar un producto a incluir en el pedido interno.
    */
    void AbrirVentanaProd();
    /*!
      \brief Abre el cuadro de dialogo para seleccionar un kit de productos a incluir en el pedido interno.
    */
    void AbrirVentanaKits();
    /*!
      \brief Elimina el producto seleccionado del pedido interno.
    */
    void EliminarProducto();
    /*!
      \brief Guarda o modifica los datos del pedido interno de productos y los productos añadidas en el pedido interno actual en la base de datos y cierra la ventana.
    */
    void Guardar();
    /*!
      \brief Abre el cuadro de dialogo para crear un envío a partir del pedido interno.
    */
    void Servir();

    void Imprimir();

private:
    Ui::MovimientosPedido *ui;
    /*!
      \brief Clase de tipo Pedido que proporciona los métodos que permiten trabajar con los datos del pedido interno de productos seleccionado.
    */
    clsMovimiento_Pedido *Ped;
    /*!
      \brief Deshabilita los controles dependiendo de si el usuario actual tiene o no permisos de modificación.
    */
    void DeshabilitarControles();
    /*!
      \brief Carga los datos contenidos en la base de datos en el atributo modelConsulta y los muestra por pantalla.
    */
    void CargarDetalles();
    /*!
      \brief Inserta un nuevo kit de productos en el pedido interno.
      \param codKit Parámetro que almacena el codigo del kit de productos a insertar en el pedido interno.
    */
    void InsertarKit(int codKit);
    /*!
      \brief Inserta un nuevo producto en el pedido interno.
      \param CodProd Parámetro que almacena el codigo del producto a insertar en el pedido interno.
      \param Cantidad Parámetro que almacena la cantidad del producto a insertar en el pedido interno.
    */
    void InsertarProducto(QString CodProd,int Cantidad);
};

#endif // MOVIMIENTOSPEDIDO_H
