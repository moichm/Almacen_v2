#ifndef KITSLISTA_H
#define KITSLISTA_H

/*!
  \class KitsLista
  \brief En esta clase se incluye los métodos y atributos utilizados para mostrar la lista seleccionable de los kits y devolver la información del kit seleccionado.
*/

#include <QDialog>
#include "kit.h"

namespace Ui {
    class KitsLista;
}

class KitsLista : public QDialog {
    Q_OBJECT
public:
    /*!
      \brief Constructor de la clase.
      \param parent Objeto de tipo QWidget que contiene un puntero al objeto padre de la clase. Por defecto es 0.
    */
    KitsLista(QWidget *parent = 0);
    /*!
      \brief Destructor de la clase.
    */
    ~KitsLista();

    /*!
      \brief Devuelve el código del kit seleccionado en la ficha.
      \return Devuelve un QString con el código del kit seleccionado.
    */
    int CodKitSeleccionado();

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
      \brief Carga los datos contenidos en la base de datos en el atributo modelConsultaDetalle y los muestra por pantalla.
    */
    void CargarLineasDetalles();

private:
    Ui::KitsLista *ui;
    /*!
      \brief Carga los datos contenidos en la base de datos en el atributo modelConsulta y los muestra por pantalla.
    */
    void CargarLineas();
};

#endif // KITSLISTA_H
