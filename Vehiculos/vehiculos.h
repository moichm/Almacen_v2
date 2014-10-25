#ifndef VEHICULOS_H
#define VEHICULOS_H

/*!
  \class Vehiculos
  \brief En esta clase se incluye los métodos y atributos utilizados para mostrar la lista de vehículos almacenados en la base de datos.
*/

#include <QMainWindow>

namespace Ui {
    class Vehiculos;
}

class Vehiculos : public QMainWindow {
    Q_OBJECT
public:
    /*!
      \brief Constructor de la clase.
      \param parent Objeto de tipo QWidget que contiene un puntero al objeto padre de la clase. Por defecto es 0.
    */
    Vehiculos(QWidget *parent = 0);
    /*!
      \brief Destructor de la clase.
    */
    ~Vehiculos();

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
      \brief Abre la ficha de la instancia del vehículo seleccionado para modificarlo o consultarlo.
    */
    void AbrirVentanaFichaMod();
    /*!
      \brief Abre una ficha vacía de vehículos para crear uno nuevo.
    */
    void AbrirVentanaFichaNuevo();
    /*!
      \brief Elimina la instancia del vehículo seleccionado de la base de datos.
    */
    void EliminarVehiculo();

    /*!
      \brief Imprime la lista de vehículos.
    */
    void Imprimir();

private:
    Ui::Vehiculos *ui;
    /*!
      \brief Deshabilita los controles dependiendo de si el usuario actual tiene o no permisos de modificación.
    */
    void DeshabilitarControles();
    /*!
      \brief Carga los datos contenidos en la base de datos en el atributo modelConsulta y los muestra en la pantalla.
    */
    void CargarLineas();
};

#endif // VEHICULOS_H
