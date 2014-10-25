#ifndef MOVIMIENTOSSTOCK_H
#define MOVIMIENTOSSTOCK_H

/*!
  \class MovimientosStock
  \brief En esta clase se incluye los métodos y atributos utilizados para crea un movimiento de stock para los productos.
*/

#include <QDialog>
#include "movimientos.h"

namespace Ui {
    class MovimientosStock;
}

class MovimientosStock : public QDialog {
    Q_OBJECT
public:
    /*!
      \brief Constructor de la clase.
      \param parent Objeto de tipo QWidget que contiene un puntero al objeto padre de la clase. Por defecto es 0.
    */
    MovimientosStock(clsMovimiento_Stock _Stock=clsMovimiento_Stock(), QWidget *parent=0);
    /*!
      \brief Destructor de la clase.
    */
    ~MovimientosStock();

private slots:
    /*!
      \brief Guarda los datos del movimiento de stock del producto actual en la base de datos y cierra la ventana.
    */
    void Guardar();
    /*!
      \brief Abre el cuadro de dialogo para seleccionar un producto para el que crear el movimiento de stock.
    */
    void AbrirVentanaProd();
    void Imprimir();

protected:
    /*!
      \brief Controlador de eventos que maneja los cambios de estado.
    */
    void changeEvent(QEvent *e);

private:
    Ui::MovimientosStock *ui;
    clsMovimiento_Stock *Stock;
    void CargarDatos();
};

#endif // MOVIMIENTOSSTOCK_H
