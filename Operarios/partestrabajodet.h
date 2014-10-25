#ifndef PARTESTRABAJODETALLE_H
#define PARTESTRABAJODETALLE_H

/*!
  \class PartesTrabajoDet
  \brief En esta clase se incluye los métodos y atributos utilizados para añadir una nueva línea al parte de trabajo,
         o mostrar los datos de una línea de un parte de trabajo en concreto.
*/

#include <QDialog>
#include "operario.h"

namespace Ui {
    class PartesTrabajoDet;
}

class PartesTrabajoDet : public QDialog {
    Q_OBJECT
public:
    /*!
      \brief Constructor de la clase. Se asignan a los atributos el valor de los parámetros de entrada.
      \param _NoParte Parámetro que almacena el número del parte de trabajo a mostrar en la ficha. Por defecto es 0.
      \param _LineaParte Parámetro que almacena el número de la línea del parte de trabajo a mostrar en la ficha. Por defecto es 0.
      \param parent Objeto de tipo QWidget que contiene un puntero al objeto padre de la clase. Por defecto es 0.
    */
    PartesTrabajoDet(int _NoParte=0,QString _Op="",tHora _TipoHora=Normal,double _Horas=0,QString _Proyecto="",QString _Tarea="",QWidget *parent = 0);
    /*!
      \brief Destructor de la clase.
    */
    ~PartesTrabajoDet();

protected:
    /*!
      \brief Controlador de eventos que maneja los cambios de estado.
    */
    void changeEvent(QEvent *e);

private slots:
    /*!
      \brief Inserta una nueva línea en el parte de trabajo, o modifica la línea existente con los datos contenidos en los atributos.
    */
    void InsertarLinea();
    /*!
      \brief Abre la ventana de selección de operarios.
    */
    void AbrirVentanaOperarios();

private:
    Ui::PartesTrabajoDet *ui;
    /*!
      \brief Atributo que almacena el número del parte de trabajo.
    */
    int intParte;
    /*!
      \brief Atributo que almacena el número de horas de la línea del parte de trabajo.
    */
    double decHoras;
    /*!
      \brief Atributo que almacena el proyecto de la línea del parte de trabajo.
    */
    QString strProyecto;
    /*!
      \brief Atributo que almacena la tarea de la línea del parte de trabajo.
    */
    QString strTarea;
    /*!
      \brief Atributo que almacena el operario de la línea del parte de trabajo.
    */
    QString strNIF;
    /*!
      \brief Atributo que almacena el tipo de hora de la línea del parte de trabajo.
    */
    enum tHora eTipoHora;
    /*!
      \brief Clase de tipo Operarios_Partes que proporciona los métodos que permiten trabajar con los datos de la línea del parte de trabajo seleccionado.
    */
    clsPartes_Trabajo_Det *ParteDetalle;
    /*!
      \brief Clase de tipo Operario que proporciona los métodos que permiten trabajar con los datos del operario seleccionado.
    */
    clsOperarios *Operario;
    /*!
      \brief Carga los datos de la línea del parte de trabajo en la ficha
    */
    void CargarDatos();
};

#endif // PARTESTRABAJODETALLE_H
