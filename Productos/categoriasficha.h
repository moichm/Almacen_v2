#ifndef CATEGORIASFICHA_H
#define CATEGORIASFICHA_H

/*!
  \class CategoriasFicha
  \brief En esta clase se incluye los métodos y atributos utilizados para añadir una nueva categoría, o mostrar o modificar una categoría en concreto.
*/

#include <QDialog>
#include "producto.h"

namespace Ui {
    class CategoriasFicha;
}

class CategoriasFicha : public QDialog {
    Q_OBJECT
public:
    /*!
      \brief Constructor de la clase. Se asignan a los atributos el valor de los parámetros de entrada.
      \param codigo Parámetro que almacena el codigo de la categoría a mostrar en la ficha.
      \param parent Objeto de tipo QWidget que contiene un puntero al objeto padre de la clase. Por defecto es 0.
    */
    CategoriasFicha(int codigo,QWidget *parent = 0);
    /*!
      \brief Destructor de la clase.
    */
    ~CategoriasFicha();

    /*!
      \brief Carga los datos de la categoría en la ficha
    */
    void CargarDatos();
protected:
    /*!
      \brief Controlador de eventos que maneja los cambios de estado.
    */
    void changeEvent(QEvent *e);

public slots:
    /*!
      \brief Guarda o modifica los datos de la categoría actual en la base de datos y cierra la ventana.
    */
    void Guardar();

private:
    Ui::CategoriasFicha *ui;
    /*!
      \brief Inserta la categoría actual en la base de datos.
    */
    void InsertarCategoria();
    /*!
      \brief Modifica los datos de la categoría actual en la base de datos.
    */
    void ModificarCategoria();
    /*!
      \brief Clase de tipo Categorias que proporciona los métodos que permiten trabajar con los datos de la categoría seleccionada.
    */
    clsCategorias *Cat;
    /*!
      \brief Deshabilita los controles dependiendo de si el usuario actual tiene o no permisos de modificación.
    */
    void DeshabilitarControles();
};

#endif // CATEGORIASFICHA_H
