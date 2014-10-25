#ifndef CENTROSFICHA_H
#define CENTROSFICHA_H

/*!
  \class CentrosFicha
  \brief En esta clase se incluye los métodos y atributos utilizados para añadir un nuevo centro, o mostrar o modificar un centro en concreto.
*/

#include <QDialog>
#include "centro.h"

namespace Ui {
    class CentrosFicha;
}

class CentrosFicha : public QDialog {
    Q_OBJECT
public:
    /*!
      \brief Constructor de la clase. Se asignan a los atributos el valor de los parámetros de entrada.
      \param _Codigo Parámetro que almacena el codigo del centro a mostrar en la ficha.
      \param parent Objeto de tipo QWidget que contiene un puntero al objeto padre de la clase. Por defecto es 0.
    */
    CentrosFicha(int _Codigo,QWidget *parent = 0);
    /*!
      \brief Destructor de la clase.
    */
    ~CentrosFicha();

protected:
    /*!
      \brief Controlador de eventos que maneja los cambios de estado.
    */
    void changeEvent(QEvent *e);

private slots:
    /*!
      \brief Guarda o modifica los datos del centro actual en la base de datos.
    */
    void Guardar();

private:
    Ui::CentrosFicha *ui;
    /*!
      \brief Deshabilita los controles dependiendo de si el usuario actual tiene o no permisos de modificación.
    */
    void DeshabilitarControles();
    /*!
      \brief Clase de tipo Centro que proporciona los métodos que permiten trabajar con los datos del centro seleccionado.
    */
    clsCentros *Centro;
    /*!
      \brief Carga los datos del centro en la ficha
    */
    void CargarDatos();
};

#endif // CENTROSFICHA_H
