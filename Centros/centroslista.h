#ifndef CENTROSLISTA_H
#define CENTROSLISTA_H

/*!
  \class CentrosLista
  \brief En esta clase se incluye los métodos y atributos utilizados para mostrar la lista seleccionable de los centros y devolver la información del centro seleccionado.
*/

#include <QDialog>

namespace Ui {
    class CentrosLista;
}

class CentrosLista : public QDialog {
    Q_OBJECT
public:
    /*!
      \brief Constructor de la clase.
      \param parent Objeto de tipo QWidget que contiene al un puntero objeto padre de la clase. Por defecto es 0.
    */
    CentrosLista(QWidget *parent = 0);
    /*!
      \brief Destructor de la clase.
    */
    ~CentrosLista();

    /*!
      \brief Devuelve el código del centro seleccionado en la ficha.
      \return Devuelve un int con el código del centro seleccionado.
    */
    int CodigoCentroSeleccionado();
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
    Ui::CentrosLista *ui;
    /*!
      \brief Carga los datos contenidos en la base de datos en el atributo modelConsulta y los muestra por pantalla.
    */
    void CargarLineas();
};

#endif // CENTROSLISTA_H
