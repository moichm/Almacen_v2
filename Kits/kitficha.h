#ifndef KITFICHA_H
#define KITFICHA_H

/*!
  \class KitFicha
  \brief En esta clase se incluye los métodos y atributos utilizados para añadir un nuevo kit, o mostrar o modificar un kit en concreto.
*/

#include <QDialog>
#include "Productos/producto.h"
#include "Kits/kit.h"

namespace Ui {
    class KitFicha;
}

class KitFicha : public QDialog {
    Q_OBJECT
public:
    /*!
      \brief Constructor de la clase. Se asignan a los atributos el valor de los parámetros de entrada.
      \param codigo Parámetro que almacena el codigo del kit a mostrar en la ficha.
      \param parent Objeto de tipo QWidget que contiene un puntero al objeto padre de la clase. Por defecto es 0.
    */
    KitFicha(int codigo,QWidget *parent = 0);
    /*!
      \brief Destructor de la clase.
    */
    ~KitFicha();

    /*!
      \brief Carga los datos del kit en la ficha
    */
    void CargarDatos();
public slots:
    /*!
      \brief Abre el cuadro de diálogo para seleccionar un producto a incluir en el kit.
    */
    void AbrirVentanaProd();
    /*!
      \brief Elimina el producto seleccionado del kit.
    */
    void EliminarProducto();
    /*!
      \brief Guarda o modifica los datos del kit actual en la base de datos y cierra la ventana.
    */
    void GuardarKit();

    void Imprimir();

protected:
    void changeEvent(QEvent *e);

private:
    /*!
      \brief Deshabilita los controles dependiendo de si el usuario actual tiene o no permisos de modificación.
    */
    void DeshabilitarControles();
    Ui::KitFicha *ui;
    /*!
      \brief Carga los datos contenidos en la base de datos en el atributo modelConsulta y los muestra por pantalla.
    */
    void CargarDetalles();
    /*!
      \brief Inserta un nuevo producto en el kit.
      \param CodProd Parámetro que almacena el código del producto a insertar en el kit.
      \param Cantidad Parámetro que almacena la cantidad del producto a insertar en el kit.
      \param guardado Parámetro que indica si el producto ha sido guardado como parte del kit en la base de datos.
    */
    void InsertarProducto(QString CodProd,int Cantidad);
    /*!
      \brief Clase de tipo Kits que proporciona los métodos que permiten trabajar con los datos del kit seleccionado.
    */
    clsKits *Kit;
};

#endif // KITFICHA_H
