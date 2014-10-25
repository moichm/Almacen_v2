#ifndef CERTIFICACIONES_H
#define CERTIFICACIONES_H

/*!
  \class Certificaciones
  \brief En esta clase se incluye los métodos y atributos utilizados para mostrar la ficha donde se incluyen los datos de certificación de una producto.
*/

#include <QDialog>

namespace Ui {
    class Certificaciones;
}

class Certificaciones : public QDialog {
    Q_OBJECT
public:
    /*!
      \brief Constructor de la clase. Se asignan a los atributos el valor de los parámetros de entrada.
      \param codigo Parámetro que almacena el código de la producto a mostrar en la ficha.
      \param parent Objeto de tipo QWidget que contiene un puntero al objeto padre de la clase. Por defecto es 0.
    */
    Certificaciones(QString codigo,QWidget *parent = 0);
    /*!
      \brief Destructor de la clase.
    */
    ~Certificaciones();

protected:
    /*!
      \brief Controlador de eventos que maneja los cambios de estado.
    */
    void changeEvent(QEvent *e);
public slots:
    /*!
      \brief Función que ejecuta la certificación de la producto.
    */
    void Certificar();

private:
    Ui::Certificaciones *ui;
};

#endif // CERTIFICACIONES_H
