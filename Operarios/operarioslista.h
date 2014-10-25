#ifndef OPERARIOSLISTA_H
#define OPERARIOSLISTA_H

/*!
  \class CentrosLista
  \brief En esta clase se incluye los métodos y atributos utilizados para mostrar la lista seleccionable de los centros y devolver la información del centro seleccionado.
*/

#include <QDialog>

namespace Ui {
    class OperariosLista;
}

class OperariosLista : public QDialog {
    Q_OBJECT
public:
    /*!
      \brief Constructor de la clase.
      \param parent Objeto de tipo QWidget que contiene al un puntero objeto padre de la clase. Por defecto es 0.
    */
    OperariosLista(QWidget *parent = 0);
    /*!
      \brief Destructor de la clase.
    */
    ~OperariosLista();    

    /*!
      \brief Devuelve el código del centro seleccionado en la ficha.
      \return Devuelve un int con el código del centro seleccionado.
    */
    QString NIFOpSeleccionado();

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
    /*!
      \brief Carga la fotografía del operario.
    */
    void CargarImagen();

private:
    Ui::OperariosLista *ui;
    /*!
      \brief Carga los datos contenidos en la base de datos en el atributo modelConsulta y los muestra por pantalla.
    */
    void CargarLineas();
};

#endif // OPERARIOSLISTA_H
