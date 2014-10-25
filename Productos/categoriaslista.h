#ifndef CATEGORIASLISTA_H
#define CATEGORIASLISTA_H

/*!
  \class CategoriasLista
  \brief En esta clase se incluye los métodos y atributos utilizados para mostrar la lista seleccionable de las categorías y devolver la información de la categoría seleccionada.
*/

#include <QDialog>

namespace Ui {
    class CategoriasLista;
}

class CategoriasLista : public QDialog {
    Q_OBJECT
public:
    /*!
      \brief Constructor de la clase.
      \param parent Objeto de tipo QWidget que contiene un puntero al objeto padre de la clase. Por defecto es 0.
    */
    CategoriasLista(QWidget *parent = 0);
    /*!
      \brief Destructor de la clase.
    */
    ~CategoriasLista();

    /*!
      \brief Devuelve el código de la categoría seleccionada en la ficha.
      \return Devuelve un QString con el código de la categoría seleccionada.
    */
    int CodCatSeleccionado();

protected:
    /*!
      \brief Controlador de eventos que maneja los cambios de estado.
    */
    void changeEvent(QEvent *e);

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

private:
    Ui::CategoriasLista *ui;
    /*!
      \brief Carga los datos contenidos en la base de datos en el atributo modelConsulta y los muestra por pantalla.
    */
    void CargarLineas();
};

#endif // CATEGORIASLISTA_H
