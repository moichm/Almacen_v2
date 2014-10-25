#ifndef FAMILIASFICHA_H
#define FAMILIASFICHA_H

/*!
  \class FamiliasFicha
  \brief En esta clase se incluye los métodos y atributos utilizados para añadir una nueva familia, o mostrar o modificar una familia en concreto.
*/

#include <QDialog>
#include "producto.h"

namespace Ui {
    class FamiliasFicha;
}

class FamiliasFicha : public QDialog {
    Q_OBJECT
public:
    /*!
      \brief Constructor de la clase. Se asignan a los atributos el valor de los parámetros de entrada.
      \param codigo Parámetro que almacena el codigo de la familia a mostrar en la ficha.
      \param parent Objeto de tipo QWidget que contiene un puntero al objeto padre de la clase. Por defecto es 0.
    */
    FamiliasFicha(int codigo,QWidget *parent = 0);
    /*!
      \brief Destructor de la clase.
    */
    ~FamiliasFicha();

    /*!
      \brief Carga los datos de la familia en la ficha
    */
    void CargarDatos();
protected:
    /*!
      \brief Controlador de eventos que maneja los cambios de estado.
    */
    void changeEvent(QEvent *e);

public slots:
    /*!
      \brief Activa o desactiva el campo donde se introducen el límite de entregas, y la frecuencia de certificación de la familia, según sea una familia con entrega limitada, y certificable o no.
    */
    void ActivarCampo();
    /*!
      \brief Guarda o modifica los datos de la familia actual en la base de datos y cierra la ventana.
    */
    void Guardar();

private:
    Ui::FamiliasFicha *ui;
    /*!
      \brief Inserta la familia actual en la base de datos.
    */
    void InsertarFamilia();
    /*!
      \brief Modifica los datos de la familia actual en la base de datos.
    */
    void ModificarFamilia();
    /*!
      \brief Clase de tipo Familias que proporciona los métodos que permiten trabajar con los datos de la familia seleccionada.
    */
    clsFamilias *Fam;
    /*!
      \brief Deshabilita los controles dependiendo de si el usuario actual tiene o no permisos de modificación.
    */
    void DeshabilitarControles();
};

#endif // FAMILIASFICHA_H
