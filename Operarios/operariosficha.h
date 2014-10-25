#ifndef OPERARIOSFICHA_H
#define OPERARIOSFICHA_H

/*!
  \class OperariosFicha
  \brief En esta clase se incluye los métodos y atributos utilizados para añadir un nuevo operario, o mostrar o modificar un operario en concreto.
*/

#include <QDialog>
#include <QAbstractButton>
#include "operario.h"
#include "../Centros/centro.h"

namespace Ui {
    class OperariosFicha;
}

class OperariosFicha : public QDialog {
    Q_OBJECT
public:
    /*!
      \brief Constructor de la clase. Se asignan a los atributos el valor de los parámetros de entrada.
      \param _NIF Parámetro que almacena el NIF del operario a mostrar en la ficha.
      \param parent Objeto de tipo QWidget que contiene un puntero al objeto padre de la clase. Por defecto es 0.
    */
    OperariosFicha(QString _NIF,QWidget *parent = 0);
    /*!
      \brief Destructor de la clase.
    */
    ~OperariosFicha();

protected:
    /*!
      \brief Controlador de eventos que maneja los cambios de estado.
    */
    void changeEvent(QEvent *e);

private slots:
    /*!
      \brief Comprueba que el NIF introducido es correcto.
    */
    bool ValidaNIF();
    /*!
      \brief Abre el cuadro de dialogo para seleccionar la fotografía del operario.
    */
    void AbrirImagenNueva();
    /*!
      \brief Abre la ventana de selección de centros.
    */
    void AbrirVentanaCentros();
    /*!
      \brief Guarda o modifica los datos del centro actual en la base de datos.
    */
    void Guardar();

    void Imprimir();

private:
    Ui::OperariosFicha *ui;
    /*!
      \brief Deshabilita los controles dependiendo de si el usuario actual tiene o no permisos de modificación.
    */
    void DeshabilitarControles();
    /*!
      \brief Clase de tipo Operario que proporciona los métodos que permiten trabajar con los datos del operario seleccionado.
    */
    clsOperarios *Op;
    /*!
      \brief Carga los datos del operario en la ficha
    */
    void CargarDatos();
    /*!
      \brief Clase de tipo Centro que proporciona los métodos que permiten trabajar con los datos del centro donde se encuentra el operario.
    */
    clsCentros *Centro;
};

#endif // OPERARIOSFICHA_H
