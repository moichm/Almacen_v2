#ifndef MOVIMIENTOSDEV_H
#define MOVIMIENTOSDEV_H

/*!
  \class MovimientosDevoluciones
  \brief En esta clase se incluye los métodos y atributos utilizados para devolver un producto de una salida.
*/

#include <QDialog>
#include "Productos/producto.h"
#include "movimientos.h"

namespace Ui {
    class MovimientosDevoluciones;
}

class MovimientosDevoluciones : public QDialog {
    Q_OBJECT
public:
    /*!
      \brief Constructor de la clase. Se asignan a los atributos el valor de los parámetros de entrada.
      \param NSalida Parámetro que almacena el número de la salida de productos a mostrar en la ficha.
      \param codProd Parámetro de tipo QString que contiene el código del producto a devolver de la salida.
      \param parent Parámetro de tipo QWidget que contiene un puntero al objeto padre de la clase. Por defecto es 0.
    */
    MovimientosDevoluciones(int NSalida,QString codProd,QDate _Fecha,QWidget *parent = 0);
    /*!
      \brief Destructor de la clase.
    */
    ~MovimientosDevoluciones();

    /*!
      \brief Devuelve la cantidad perdida del producto en la salida.
      \return Devuelve un int con la cantidad perdida del producto en la salida.
    */
    int CantidadPerdida();
    /*!
      \brief Devuelve la cantidad devuelta del producto en la salida.
      \return Devuelve un int la cantidad devuelta del producto en la salida.
    */
    int CantidadDevuelta();
protected:
    /*!
      \brief Controlador de eventos que maneja los cambios de estado.
    */
    void changeEvent(QEvent *e);

private slots:
    /*!
      \brief Comprueba si la cantidad introducida como perdida y/o devuelta es correcta, en caso de ser correcta cierra la ventana, y en caso de no serlo muestra un mensaje de error.
    */
    void Aceptar();

private:
    Ui::MovimientosDevoluciones *ui;
    /*!
      \brief Carga los datos de la línea de la salida en la ficha
      \param Sal Parámetro que almacena el número de la salida de productos a mostrar en la ficha.
      \param _CodProd Parámetro de tipo QString que contiene el código del producto a devolver de la salida.
    */
    void CargarDatos(int Sal,QString _CodProd,QDate _FechaSal);
    /*!
      \brief Carga los datos contenidos en la base de datos en el atributo modelConsulta y los muestra por pantalla.
    */
    void CargarDetalles();
    clsMovimiento_DetSal *SalDet;
};

#endif // MOVIMIENTOSDEV_H
