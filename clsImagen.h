#ifndef PIXMAPWIDGET_H
#define PIXMAPWIDGET_H

/*!
  \class clsImagen
  \brief En esta clase se incluye los métodos y atributos necesarios para manejar las imágenes de los usuarios o productos.
*/

#include <QWidget>
#include <QString>

class QPixmap;

class clsImagen: public QWidget
{
    Q_OBJECT
	
public:
    /*!
      \brief Constructor de la clase. Se asignan a los atributos el valor de los parámetros de entrada.
      \param nomArch Objeto de tipo QString que contiene el nombre del archivo de imágen a cargar.
      \param _W Objeto de tipo int que contiene la anchura máxima que puede tener la imágen.
      \param _H Objeto de tipo int que contiene la altura máxima que puede tener la imágen.
      \param parent Objeto de tipo QWidget que contiene un puntero al objeto padre de la clase. Por defecto es 0.
    */
    clsImagen(const QString nomArch,int _W,int _H, QWidget *parent=0);
    /*!
      \brief Destructor de la clase.
    */
    ~clsImagen();

signals:    
    /*!
      \brief Señal que se emite cuando el tamaño de la imágen cambia.
      \param NuevoZoom Contiene el nuevo valor del porcentaje de zoom.
    */
    void CambioFactorZoom(float NuevoZoom);

public slots:
    /*!
      \brief Función modifica el tamaño de la imágen.
      \param PorZ Contiene el nuevo valor del porcentaje de zoom a asginar al atributo flPorcentajeZoom.

      En esta función se emite la señal CambioFactorZoom, y se llama a la funcion paintEvent, mediante el método repaint.
    */
    void setPorcentajeZoom(float PorZ);

protected:
    /*!
      \brief Función modifica el tamaño de la imágen.
      \param p Objeto de tipo QPaintEvent que contiene las especificaciones para "repintar" la imágen.

      En esta función se modifica la imágen según el nuevo zoom.
    */
    void paintEvent(QPaintEvent* p);
    /*!
      \brief Reimplementación de un manejador por defecto de Qt que permite modificar el tamaño de la imagen cuando se gira la ruleta central del ratón sobre ella.
    */
    void wheelEvent(QWheelEvent*);

private:
    /*!
      \brief Atributo que almacena la imágen.
    */
    QPixmap *pixImagen;
    /*!
      \brief Atributo que almacena el porcentaje de zoom de la imágen.
    */
    float flPorcentajeZoom;
    /*!
      \brief Atributo que almacena la anchura máxima de la imágen
    */
    int intW;
    /*!
      \brief Atributo que almacena la altura máxima de la imágen
    */
    int intH;
};

#endif // PIXMAPWIDGET_H
