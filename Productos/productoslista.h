#ifndef PRODUCTOSLISTA_H
#define PRODUCTOSLISTA_H

/*!
  \class ProductosLista
  \brief En esta clase se incluye los métodos y atributos utilizados para mostrar la lista seleccionable de los productos y devolver la información del producto seleccionado.
*/

#include <QDialog>

namespace Ui {
    class ProductosLista;
}

class ProductosLista : public QDialog {
    Q_OBJECT
public:
    /*!
      \brief Constructor de la clase.
      \param parent Objeto de tipo QWidget que contiene un puntero al objeto padre de la clase. Por defecto es 0.
    */
    ProductosLista(QWidget *parent = 0);
    /*!
      \brief Destructor de la clase.
    */
    ~ProductosLista();

    /*!
      \brief Devuelve el código del producto seleccionado en la ficha.
      \return Devuelve un QString con el código del producto seleccionado.
    */
    QString CodProdSeleccionado();
private slots:
    /*!
      \brief Comprueba el criterio seleccionado y en caso de ser distinto de TODOS ejecuta la función Filtrar().
      \see Filtrar();
    */
    void ActivarFiltro();
    /*!
      \brief Filtra el contenido de la tabla dependiendo del criterio seleccionado.
    */
    void Filtrar();
    /*!
      \brief Carga la imagen del producto.
    */
    void CargarImagen();

protected:
    /*!
      \brief Controlador de eventos que maneja los cambios de estado.
    */
    void changeEvent(QEvent *e);

private:
    Ui::ProductosLista *ui;
    /*!
      \brief Carga los datos contenidos en la base de datos en el atributo modelConsulta y los muestra por pantalla.
    */
    void CargarLineas();
};

#endif // PRODUCTOSLISTA_H
