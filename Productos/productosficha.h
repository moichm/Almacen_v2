#ifndef PRODUCTOSFICHA_H
#define PRODUCTOSFICHA_H

/*!
  \class PRODUCTOSFICHA
  \brief En esta clase se incluye los métodos y atributos utilizados para añadir un nuevo producto, o mostrar o modificar un producto en concreto.
*/

#include <QDialog>
#include <QMap>
#include "producto.h"

namespace Ui {
    class ProductosFicha;
}

class ProductosFicha : public QDialog {
    Q_OBJECT
public:
    /*!
      \brief Constructor de la clase. Se asignan a los atributos el valor de los parámetros de entrada.
      \param codigo Parámetro que almacena el código del producto a mostrar en la ficha.
      \param parent Objeto de tipo QWidget que contiene un puntero al objeto padre de la clase. Por defecto es 0.
    */
    ProductosFicha(QString codigo,QWidget *parent = 0);
    /*!
      \brief Destructor de la clase.
    */
    ~ProductosFicha();

    /*!
      \brief Carga los datos del producto en la ficha
    */
    void CargarDatos();
protected:
    /*!
      \brief Controlador de eventos que maneja los cambios de estado.
    */
    void changeEvent(QEvent *e);
public slots:
    /*!
      \brief Abre el cuadro de diálogo para para certificar el producto.
    */
    void AbrirVentanaFichaCertif();
    /*!
      \brief Guarda o modifica los datos del producto actual en la base de datos y cierra la ventana.
    */
    void Guardar();
    /*!
      \brief Guarda o modifica los datos del producto actual en la base de datos.
    */
    void Aplicar();
    void Almacenar();
    /*!
      \brief Abre el cuadro de diálogo para seleccionar el tipo del producto.
    */
    void AbrirVentanaCategoria();
    /*!
      \brief Abre el cuadro de diálogo para seleccionar el tipo del producto.
    */
    void AbrirVentanaFamilia();
    /*!
      \brief Abre el cuadro de dialogo para seleccionar la fotografía del producto.
    */
    void AbrirImagenNueva();

    /*!
      \brief Imprime la ficha del producto que se muestra en la ficha.
    */
    void Imprimir();
private slots:
    /*!
      \brief Activa o desactiva los campos relativos a los productos de certificación, dependiendo de si el producto pertenece a una familia certificable o no.
    */
    void ActivarCampos();

private:
    /*!
      \brief Clase de tipo Producto que proporciona los métodos que permiten trabajar con los datos de la producto seleccionado.
    */
    clsProductos *Prod;
    /*!
      \brief Deshabilita los controles dependiendo de si el usuario actual tiene o no permisos de modificación.
    */
    void DeshabilitarControles();
    Ui::ProductosFicha *ui;
};

#endif // PRODUCTOSFICHA_H
