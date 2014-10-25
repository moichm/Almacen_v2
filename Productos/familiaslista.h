#ifndef FAMILIASLISTA_H
#define FAMILIASLISTA_H

/*!
  \class FamiliasLista
  \brief En esta clase se incluye los métodos y atributos utilizados para mostrar la lista seleccionable de las familias y devolver la información de la familia seleccionada.
*/

#include <QDialog>

namespace Ui {
    class FamiliasLista;
}

class FamiliasLista : public QDialog {
    Q_OBJECT
public:
    /*!
      \brief Constructor de la clase.
      \param parent Objeto de tipo QWidget que contiene un puntero al objeto padre de la clase. Por defecto es 0.
    */
    FamiliasLista(QWidget *parent = 0);
    /*!
      \brief Destructor de la clase.
    */
    ~FamiliasLista();

    /*!
      \brief Devuelve el código de la familia seleccionada en la ficha.
      \return Devuelve un QString con el código de la familia seleccionada.
    */
    int CodFamiliaSeleccionada();

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
    Ui::FamiliasLista *ui;
    /*!
      \brief Carga los datos contenidos en la base de datos en el atributo modelConsulta y los muestra por pantalla.
    */
    void CargarLineas();
};

#endif // FAMILIASLISTA_H
