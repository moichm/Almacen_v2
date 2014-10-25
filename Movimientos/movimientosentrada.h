#ifndef MOVIMIENTOSENTRADA_H
#define MOVIMIENTOSENTRADA_H

/*!
  \class MovimientosEntrada
  \brief En esta clase se incluye los métodos y atributos utilizados para dar entrada a productos incluidos en una salida.
*/

#include <QDialog>
#include "Productos/producto.h"
#include "movimientos.h"

namespace Ui {
    class MovimientosEntrada;
}

class MovimientosEntrada : public QDialog {
    Q_OBJECT
public:
    /*!
      \brief Constructor de la clase. Se asignan a los atributos el valor de los parámetros de entrada.
      \param codigo Parámetro que almacena el número de salida de productos a mostrar en la ficha.
      \param parent Objeto de tipo QWidget que contiene un puntero al objeto padre de la clase. Por defecto es 0.
    */
    MovimientosEntrada(int codigo,QWidget *parent = 0);
    /*!
      \brief Destructor de la clase.
    */
    ~MovimientosEntrada();

protected:
    /*!
      \brief Controlador de eventos que maneja los cambios de estado.
    */
    void changeEvent(QEvent *e);

private slots:
    /*!
      \brief Devuelve toda la cantidad que no ha sido previamente entregada o perdida del producto seleccionado.
    */
    void DevolverTodo();
    /*!
      \brief Abre el cuadro de dialogo para dar entrada al producto seleccionado.
    */
    void AbrirVentanaDevolucion();

    void Imprimir();

private:
    Ui::MovimientosEntrada *ui;
    /*!
      \brief Carga los datos de la salida de productos en la ficha
    */
    void CargarDatos();
    /*!
      \brief Clase de tipo Salida que proporciona los métodos que permiten trabajar con los datos de la salida de productos seleccionado.
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
};

#endif // MOVIMIENTOSENTRADA_H
