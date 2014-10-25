#ifndef MOVIMIENTOSLISTAS_H
#define MOVIMIENTOSLISTAS_H

/*!
  \class MovimientosListas
  \brief En esta clase se incluye los métodos y atributos utilizados para mostrar las listas de los movimientos de los productos.
*/

#include <QMainWindow>

namespace Ui {
    class MovimientosListas;
}

class MovimientosListas : public QMainWindow {
    Q_OBJECT
public:
    /*!
      \brief Constructor de la clase.
      \param parent Objeto de tipo QWidget que contiene un puntero al objeto padre de la clase. Por defecto es 0.
    */
    MovimientosListas(QWidget *parent = 0);
    /*!
      \brief Destructor de la clase.
    */
    ~MovimientosListas();

signals:
    void closeMDI();

protected:
    /*!
      \brief Controlador de eventos que maneja los cambios de estado.
    */
    void changeEvent(QEvent *e);

public slots:
    /*!
      \brief Comprueba el criterio seleccionado en la pestaña de \e Salidas y en caso de ser distinto de TODOS ejecuta la función Filtrar().
      \see Filtrar();
    */
    void ActivarFiltroSalidas();
    /*!
      \brief Filtra el contenido de la tabla de \e Salidas dependiendo del criterio seleccionado.
    */
    void FiltrarSalidas();
    void ImprimirEnt();
    void ImprimirSal();
    void ImprimirEnv();
    void ImprimirRec();
    void ImprimirStock();
    void ImprimirPedidos();
    /*!
      \brief Comprueba el criterio seleccionado en la pestaña de \e Entradas y en caso de ser distinto de TODOS ejecuta la función Filtrar().
      \see Filtrar();
    */
    void ActivarFiltroEntradas();
    /*!
      \brief Filtra el contenido de la tabla de \e Entradas dependiendo del criterio seleccionado.
    */
    void FiltrarEntradas();
    /*!
      \brief Comprueba el criterio seleccionado en la pestaña de \e Envíos y en caso de ser distinto de TODOS ejecuta la función Filtrar().
      \see Filtrar();
    */
    void ActivarFiltroEnvios();
    /*!
      \brief Filtra el contenido de la tabla de \e Envíos dependiendo del criterio seleccionado.
    */
    void FiltrarEnvios();
    /*!
      \brief Comprueba el criterio seleccionado en la pestaña de \e Recepciones y en caso de ser distinto de TODOS ejecuta la función Filtrar().
      \see Filtrar();
    */
    void ActivarFiltroRecepciones();
    /*!
      \brief Filtra el contenido de la tabla de \e Recepciones dependiendo del criterio seleccionado.
    */
    void FiltrarRecepciones();
    /*!
      \brief Comprueba el criterio seleccionado en la pestaña de \e Pedidos \e internos y en caso de ser distinto de TODOS ejecuta la función Filtrar().
      \see Filtrar();
    */
    void ActivarFiltroPedidos();
    /*!
      \brief Filtra el contenido de la tabla de \e Pedidos \e internos dependiendo del criterio seleccionado.
    */
    void FiltrarPedidos();
    /*!
      \brief Comprueba el criterio seleccionado en la pestaña de \e Stock y en caso de ser distinto de TODOS ejecuta la función Filtrar().
      \see Filtrar();
    */
    void ActivarFiltroStock();
    /*!
      \brief Filtra el contenido de la tabla de \e Stock dependiendo del criterio seleccionado.
    */
    void FiltrarStock();
    /*!
      \brief Ejecuta la función que carga las lineas de la tabla visualizada.
    */
    void CargarLineas();
    /*!
      \brief Abre una ficha vacía de salidas de productos para crear una nueva.
    */
    void AbrirVentanaFichaNuevaSal();
    /*!
      \brief Abre una ficha vacía de pedido interno de producto para crear uno nuevo.
    */
    void AbrirVentanaFichaNuevaPedido();
    /*!
      \brief Abre una ficha vacía de envío de producto para crear uno nuevo.
    */
    void AbrirVentanaFichaNuevaEnvio();
    /*!
      \brief Abre una ficha vacía de stock de producto para crear un nuevo movimiento.
    */
    void AbrirVentanaFichaNuevoStock();
    void AbrirVentanaFichaModStock();
    /*!
      \brief Abre la ficha de la instancia del pedido interno de producto seleccionado para modificarlo o consultarlo.
    */
    void AbrirVentanaFichaModPedido();
    /*!
      \brief Abre la ficha de la instancia de la salida de productos seleccionada para modificarla o consultarla.
    */
    void AbrirVentanaFichaModSal();
    /*!
      \brief Abre la ficha de la instancia de la salida de productos seleccionada para dar entrada a alguno de sus productos.
    */
    void AbrirVentanaFichaEnt();
    /*!
      \brief Abre la ficha de la instancia del envío de productos seleccionado para modificarlo o consultarlo.
    */
    void AbrirVentanaFichaModEnvio();
    /*!
      \brief Abre la ficha de la instancia del envío de productos seleccionado para recibir alguno de sus productos.
    */
    void AbrirVentanaFichaRec();
    /*!
      \brief Elimina la instancia de la salida de productos seleccionada de la base de datos.
    */
    void EliminarSalida();
    /*!
      \brief Elimina la instancia del pedido interno de productos seleccionado de la base de datos.
    */
    void EliminarPedido();
    /*!
      \brief Elimina la instancia del envío de productos seleccionado de la base de datos.
    */
    void EliminarEnvio();

private:
    /*!
      \brief Carga los datos de las salidas contenidos en la base de datos en el atributo modelConsulta y los muestra por pantalla.
    */
    void CargarLineasSalidas();
    /*!
      \brief Carga los datos de las entradas contenidos en la base de datos en el atributo modelConsulta y los muestra por pantalla.
    */
    void CargarLineasEntradas();
    /*!
      \brief Carga los datos de los envíos contenidos en la base de datos en el atributo modelConsulta y los muestra por pantalla.
    */
    void CargarLineasEnvios();
    /*!
      \brief Carga los datos de las recepciones contenidos en la base de datos en el atributo modelConsulta y los muestra por pantalla.
    */
    void CargarLineasRecepciones();
    /*!
      \brief Carga los datos de los pedidos interno contenidos en la base de datos en el atributo modelConsulta y los muestra por pantalla.
    */
    void CargarLineasPedidos();
    /*!
      \brief Carga los datos de los movimientos de stock contenidos en la base de datos en el atributo modelConsulta y los muestra por pantalla.
    */
    void CargarLineasStock();
    Ui::MovimientosListas *ui;
    /*!
      \brief Deshabilita los controles dependiendo de si el usuario actual tiene o no permisos de modificación.
    */
    void DeshabilitarControles();
};

#endif // MOVIMIENTOSLISTAS_H
