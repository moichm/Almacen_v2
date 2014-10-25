#ifndef PARTESTRABAJOFICHA_H
#define PARTESTRABAJOFICHA_H

/*!
  \class PartesTrabajoFicha
  \brief En esta clase se incluye los métodos y atributos utilizados para añadir un nuevo parte de trabajo, o mostrar o modificar un parte de trabajo en concreto.
*/

#include <QDialog>
#include "operario.h"
#include "Centros/centro.h"

namespace Ui {
    class PartesTrabajoFicha;
}

class PartesTrabajoFicha : public QDialog {
    Q_OBJECT
public:
    /*!
      \brief Constructor de la clase. Se asignan a los atributos el valor de los parámetros de entrada.
      \param _Parte Parámetro que almacena el codigo del parte de trabajo a mostrar en la ficha. Por defecto es 0.
      \param parent Objeto de tipo QWidget que contiene un puntero al objeto padre de la clase. Por defecto es 0.
    */
    PartesTrabajoFicha(int _Parte=0,QWidget *parent = 0);
    /*!
      \brief Destructor de la clase.
    */
    ~PartesTrabajoFicha();

protected:
    /*!
      \brief Controlador de eventos que maneja los cambios de estado.
    */
    void changeEvent(QEvent *e);

private slots:
    /*!
      \brief Abre la ventana de selección de operarios.
    */
    void AbrirVentanaOperarios();
    /*!
      \brief Guarda o modifica los datos de las líneas y del parte de trabajo actual en la base de datos.
    */
    void Guardar();
    /*!
      \brief Guarda o modifica los datos del parte de trabajo actual en la base de datos.
    */
    void GuardarDatos();
    /*!
      \brief Abre la ventana para añadir una nueva línea al parte de trabajo.
    */
    void AbrirVentanaAniadir();
    /*!
      \brief Abre la ventana para modificar una línea del parte de trabajo.
    */
    void AbrirVentanaFicha();
    /*!
      \brief Elimina la línea seleccionada del parte de trabajo.
    */
    void EliminarLinea();
    /*!
      \brief Imprime la ficha del parte.
    */
    void Imprimir();

private:
    Ui::PartesTrabajoFicha *ui;
    /*!
      \brief Deshabilita los controles dependiendo de si el usuario actual tiene o no permisos de modificación.
    */
    void DeshabilitarControles();
    /*!
      \brief Clase de tipo Operario que proporciona los métodos que permiten trabajar con los datos del operario seleccionado.
    */
    clsOperarios *Operario;
    /*!
      \brief Clase de tipo Operarios_Partes que proporciona los métodos que permiten trabajar con los datos del parte de trabajo seleccionado.
    */
    clsPartes_Trabajo *Parte;
    /*!
      \brief Carga los datos del operario en la ficha
    */
    void CargarDatos();
    /*!
      \brief Guarda los datos del parte de trabajo actual en la base de datos.
    */
    bool InsertarParte();
    /*!
      \brief Modifica los datos del parte de trabajo actual en la base de datos.
    */
    bool ModificarParte();
    /*!
      \brief Carga los datos contenidos en la base de datos en el atributo modelConsulta y los muestra por pantalla.
    */
    void CargarLineas();
};

#endif // PARTESTRABAJOFICHA_H
