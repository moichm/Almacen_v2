#ifndef CENTROS_H
#define CENTROS_H

/*!
  \class Centros
  \brief En esta clase se incluye los métodos y atributos utilizados para mostrar la lista de los centros.
*/

#include <QMainWindow>

namespace Ui {
    class Centros;
}

class Centros : public QMainWindow {
    Q_OBJECT
public:    
    /*!
      \brief Constructor de la clase.
      \param parent Objeto de tipo QWidget que contiene un puntero al objeto padre de la clase. Por defecto es 0.
    */
    Centros(QWidget *parent = 0);
    /*!
      \brief Destructor de la clase.
    */
    ~Centros();

signals:
    void closeMDI();

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
      \brief Abre la ficha de la instancia del centro seleccionado para modificarlo o consultarlo.
    */
    void AbrirVentanaFichaMod();
    /*!
      \brief Abre una ficha vacía de centro para crear uno nuevo.
    */
    void AbrirVentanaFichaNuevo();
    /*!
      \brief Elimina la instancia del centro seleccionado de la base de datos.
    */
    void EliminarCentro();

    void Imprimir();

private:
    Ui::Centros *ui;
    /*!
      \brief Deshabilita los controles dependiendo de si el usuario actual tiene o no permisos de modificación.
    */
    void DeshabilitarControles();
    /*!
      \brief Carga los datos contenidos en la base de datos en el atributo modelConsulta y los muestra en la pantalla.
    */
    void CargarLineas();
};

#endif // CENTROS_H
