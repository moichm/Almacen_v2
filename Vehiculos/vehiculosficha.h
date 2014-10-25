#ifndef VEHICULOSFICHA_H
#define VEHICULOSFICHA_H

/*!
  \class VehiculosFicha
  \brief En esta clase se incluye los métodos y atributos utilizados para añadir un nuevo vehículo, o mostrar o modificar un vehículo en concreto.
*/

#include <QDialog>
#include "vehiculo.h"
#include "Centros/centro.h"

namespace Ui {
    class VehiculosFicha;
}

class VehiculosFicha : public QDialog {
    Q_OBJECT
public:
    /*!
      \brief Constructor de la clase. Se asignan a los atributos el valor de los parámetros de entrada.
      \param _Matricula Parámetro que almacena matrícula del vehículo a mostrar en la ficha. Por defecto es "".
      \param parent Objeto de tipo QWidget que contiene un puntero al objeto padre de la clase. Por defecto es 0.
    */
    VehiculosFicha(QString _Matricula="",QWidget *parent = 0);
    /*!
      \brief Destructor de la clase.
    */
    ~VehiculosFicha();

protected:
    /*!
      \brief Controlador de eventos que maneja los cambios de estado.
    */
    void changeEvent(QEvent *e);

private slots:
    /*!
      \brief Abre la ventana de selección de centros.
    */
    void AbrirVentanaCentros();
    /*!
      \brief Guarda o modifica los datos del vehículo actual en la base de datos.
    */
    void Guardar();

    /*!
      \brief Imprime la ficha del vehículo.
    */
    void Imprimir();
    void AbrirImagenNueva();

private:
    QDate CalcularFechaITV();

    Ui::VehiculosFicha *ui;
    /*!
      \brief Deshabilita los controles dependiendo de si el usuario actual tiene o no permisos de modificación.
    */
    void DeshabilitarControles();
    /*!
      \brief Clase de tipo Vehículo que proporciona los métodos que permiten trabajar con los datos del vehículo seleccionado.
    */
    clsVehiculos *Vehiculo;
    /*!
      \brief Carga los datos del vehículo en la ficha
    */
    void CargarDatos();
    /*!
      \brief Clase de tipo Centro que proporciona los métodos que permiten trabajar con los datos del centro donde se encuentra el vehículo.
    */
    clsCentros *Centro;
    /*!
      \brief Carga los datos contenidos en la base de datos en el atributo modelConsulta y los muestra por pantalla.
    */
    void CargarLineas();
    /*!
      \brief Atributo que controla si se ha cambiado el centro donde se encuentra el vehículo.
    */
    bool ModCentro;
};

#endif // VEHICULOSFICHA_H
