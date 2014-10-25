#ifndef MOVIMIENTOSRECEPCION_H
#define MOVIMIENTOSRECEPCION_H

/*!
  \class MovimientosRecepcion
  \brief En esta clase se incluye los métodos y atributos utilizados para recibir un producto de un envío.
*/

#include <QDialog>
#include "../Centros/centro.h"
#include "movimientos.h"
#include <QSet>

namespace Ui {
    class MovimientosRecepcion;
}

class MovimientosRecepcion : public QDialog {
    Q_OBJECT
public:
    /*!
      \brief Constructor de la clase. Se asignan a los atributos el valor de los parámetros de entrada.
      \param codigo Parámetro que almacena el número del envío de productos a mostrar en la ficha.
      \param parent Parámetro de tipo QWidget que contiene un puntero al objeto padre de la clase. Por defecto es 0.
    */
    MovimientosRecepcion(int codigo,QWidget *parent = 0);
    /*!
      \brief Destructor de la clase.
    */
    ~MovimientosRecepcion();

protected:
    /*!
      \brief Controlador de eventos que maneja los cambios de estado.
    */
    void changeEvent(QEvent *e);

public slots:
    void Imprimir();
    void Recibir();
    void RecibirTodo();
private:
    Ui::MovimientosRecepcion *ui;
    /*!
      \brief Carga los datos del envío en la ficha
    */
    void CargarDatos();
    /*!
      \brief Clase de tipo Movimiento_Envio que proporciona los métodos que permiten trabajar con los datos de los envíos.
    */
    clsMovimiento_Envio *Envio;
    /*!
      \brief Clase de tipo Centro que proporciona los métodos que permiten trabajar con los datos de los centro.
    */
    clsCentros *Centro;
    /*!
      \brief Deshabilita los controles dependiendo de si el usuario actual tiene o no permisos de modificación.
    */
    void DeshabilitarControles();
    /*!
      \brief Carga los datos contenidos en la base de datos en el atributo modelConsulta y los muestra por pantalla.
    */
    void CargarDetalles();
};

#endif // MOVIMIENTOSRECEPCION_H
