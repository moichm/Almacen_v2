#ifndef MOVIMIENTOSENVIO_H
#define MOVIMIENTOSENVIO_H
/*!
  \class MovimientosEnvio
  \brief En esta clase se incluye los métodos y atributos utilizados para añadir o eliminar productos y kits a los envíos, y mostrar o modificar los datos de los envíos.
*/

#include <QDialog>
#include "../Centros/centro.h"
#include "Productos/producto.h"
#include "movimientos.h"

namespace Ui {
    class MovimientosEnvio;
}

class MovimientosEnvio : public QDialog {
    Q_OBJECT
public:
    /*!
      \brief Constructor de la clase. Se asignan a los atributos el valor de los parámetros de entrada.
      \param _Env Parámetro que almacena el número de envío de productos a mostrar en la ficha.
      \param parent Objeto de tipo QWidget que contiene un puntero al objeto padre de la clase. Por defecto es 0.
    */
    MovimientosEnvio(int _Env,QWidget *parent = 0);
    /*!
      \brief Destructor de la clase.
    */
    ~MovimientosEnvio();

    /*!
      \brief Carga los datos del envío de productos en la ficha
    */
    void CargarDatos();
    /*!
      \brief Crea un envío con las lineas incluidas en el pedido interno de origen, con el centro de destino igual al centro de origen del pedido interno.
    */
    void ServirPedido(int Pedido, QString _Prod, QDate _Fecha);
protected:
    /*!
      \brief Controlador de eventos que maneja los cambios de estado.
    */
    void changeEvent(QEvent *e);

public slots:
    /*!
      \brief Abre el cuadro de dialogo para seleccionar un producto a incluir en el envío.
    */
    void AbrirVentanaProd();
    /*!
      \brief Abre el cuadro de dialogo para seleccionar un kit de productos a incluir en el envío.
    */
    void AbrirVentanaKits();
    /*!
      \brief Abre el cuadro de dialogo para seleccionar un centro de destino para el envío.
    */
    void AbrirVentanaCentros();
    /*!
      \brief Elimina el producto seleccionado del envío.
    */
    void EliminarProducto();
    /*!
      \brief Guarda o modifica los datos del envío de productos y los productos añadidos en el envío actual en la base de datos y cierra la ventana.
    */
    void Guardar();
    /*!
      \brief Guarda o modifica los datos del envío de productos actual en la base de datos y cierra la ventana.
    */
    void GuardarDatos();

    void Imprimir();

private:
    Ui::MovimientosEnvio *ui;
    /*!
      \brief Clase de tipo Envio que proporciona los métodos que permiten trabajar con los datos del envío de productos seleccionado.
    */
    clsMovimiento_Envio *Env;
    /*!
      \brief Clase de tipo Centro que proporciona los métodos que permiten trabajar con los datos de los centro.
    */
    clsCentros *CentroDestino;
    /*!
      \brief Clase de tipo Centro que proporciona los métodos que permiten trabajar con los datos de los centro.
    */
    clsCentros *CentroOrigen;
    /*!
      \brief Deshabilita los controles dependiendo de si el usuario actual tiene o no permisos de modificación.
    */
    void DeshabilitarControles();
    /*!
      \brief Carga los datos contenidos en la base de datos en el atributo modelConsulta y los muestra por pantalla.
    */
    void CargarDetalles();
    /*!
      \brief Inserta un nuevo kit de productos en el pedido interno.
      \param codKit Parámetro que almacena el codigo del kit de productos a insertar en el pedido interno.
    */
    void InsertarKit(int codKit);
    /*!
      \brief Inserta una nuevo producto en el pedido interno.
      \param CodProd Parámetro que almacena el codigo del producto a insertar en el pedido interno.
      \param Cantidad Parámetro que almacena la cantidad del producto a insertar en el pedido interno.
    */
    void InsertarProducto(QString CodProd,int Cantidad);
    /*!
      \brief Importa las líneas contenidas en el pedido interno del que proviene el envío.
    */
    void ImportarLineasPedido();
};

#endif // MOVIMIENTOSENVIO_H
