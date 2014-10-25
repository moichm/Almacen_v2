#ifndef MOVIMIENTOSSALIDAS_H
#define MOVIMIENTOSSALIDAS_H

/*!
  \class MovimientosSalidas
  \brief En esta clase se incluye los métodos y atributos utilizados para añadir o eliminar productos y kits a la salida, y mostrar o modificar los datos de la salida.
*/

#include <QDialog>
#include "movimientos.h"

namespace Ui {
    class MovimientosSalidas;
}

class MovimientosSalidas : public QDialog {
    Q_OBJECT
public:
    /*!
      \brief Constructor de la clase. Se asignan a los atributos el valor de los parámetros de entrada.
      \param codigo Parámetro que almacena el número de salida de productos a mostrar en la ficha.
      \param parent Objeto de tipo QWidget que contiene un puntero al objeto padre de la clase. Por defecto es 0.
    */
    MovimientosSalidas(int codigo,QWidget *parent = 0);
    /*!
      \brief Destructor de la clase.
    */
    ~MovimientosSalidas();

    /*!
      \brief Carga los datos de la salida de productos en la ficha
    */
    void CargarDatos();
protected:
    /*!
      \brief Controlador de eventos que maneja los cambios de estado.
    */
    void changeEvent(QEvent *e);

public slots:
    /*!
      \brief Abre el cuadro de dialogo para seleccionar un producto a incluir en la salida.
    */
    void AbrirVentanaProd();
    /*!
      \brief Abre el cuadro de dialogo para seleccionar un kit de productos a incluir en la salida.
    */
    void AbrirVentanaKits();
    /*!
      \brief Abre el cuadro de dialogo para seleccionar un operario como responsable de la salida.
    */
    void AbrirVentanaOperarios();
    /*!
      \brief Elimina el producto seleccionado de la salida.
    */
    void EliminarProducto();
    /*!
      \brief Guarda o modifica los datos de la salida de productos y los productos añadidos en la salida actual en la base de datos y cierra la ventana.
    */
    void Guardar();
    /*!
      \brief Guarda o modifica los datos de la salida de productos actual en la base de datos y cierra la ventana.
    */
    void GuardarDatos();

    void Imprimir();

private:
    Ui::MovimientosSalidas *ui;
    /*!
      \brief Clase de tipo Movimiento_Salida que proporciona los métodos que permiten trabajar con los datos de la salida de productos seleccionada.
    */
    clsMovimiento_Salida *Sal;
    /*!
      \brief Deshabilita los controles dependiendo de si el usuario actual tiene o no permisos de modificación.
    */
    void DeshabilitarControles();
    /*!
      \brief Carga los datos contenidos en la base de datos en el atributo modelConsulta y los muestra por pantalla.
    */
    void CargarDetalles();
    /*!
      \brief Inserta un nuevo kit de productos en la salida.
      \param codKit Parámetro que almacena el codigo del kit de productos a insertar en la salida.
    */
    void InsertarKit(int codKit);
    /*!
      \brief Inserta un nuevo producto en la salida.
      \param CodProd Parámetro que almacena el codigo del producto a insertar en la salida.
      \param Cantidad Parámetro que almacena la cantidad del producto a insertar en la salida.
    */
    void InsertarProducto(QString CodProd,int Cantidad);
    bool InsertarSalida();
    bool ModificarSalida();
};

#endif // MOVIMIENTOSSALIDAS_H
