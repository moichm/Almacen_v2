#ifndef KITS_H
#define KITS_H

/*!
  \class Kits
  \brief En esta clase se incluye los métodos y atributos utilizados para mostrar la lista de los kits.
*/

#include <QMainWindow>

namespace Ui {
    class Kits;
}

class Kits : public QMainWindow {
    Q_OBJECT
public:
    /*!
      \brief Constructor de la clase.
      \param parent Objeto de tipo QWidget que contiene un puntero al objeto padre de la clase. Por defecto es 0.
    */
    Kits(QWidget *parent = 0);
    /*!
      \brief Destructor de la clase.
    */
    ~Kits();

signals:
    void closeMDI();

public slots:
    /*!
      \brief Abre la ficha de la instancia del kit seleccionado para modificarlo o consultarlo.
    */
    void AbrirVentanaFichaMod();
    /*!
      \brief Abre una ficha vacía de kit para crear uno nuevo.
    */
    void AbrirVentanaFichaNuevo();
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
      \brief Elimina la instancia del kit seleccionado de la base de datos.
    */
    void Eliminar();

    void Imprimir();

protected:
    /*!
      \brief Controlador de eventos que maneja los cambios de estado.
    */
    void changeEvent(QEvent *e);

private:
    /*!
      \brief Deshabilita los controles dependiendo de si el usuario actual tiene o no permisos de modificación.
    */
    void DeshabilitarControles();
    /*!
      \brief Carga los datos contenidos en la base de datos en el atributo modelConsulta y los muestra en la pantalla.
    */
    void CargarLineas();
    Ui::Kits *ui;
};

#endif // KITS_H
