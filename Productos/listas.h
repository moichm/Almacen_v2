#ifndef LISTAS_H
#define LISTAS_H

#include <QMainWindow>

namespace Ui {
class listas;
}

class listas : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit listas(QWidget *parent = 0);
    ~listas();

signals:
    void closeMDI();

public slots:
    /*!
      \brief Carga los datos contenidos en la base de datos en el atributo modelConsulta y los muestra en la pantalla.
    */
    void CargarLineas();
    /*!
      \brief Abre una ficha vacía de categorias para crear una nueva.
    */
    void AbrirVentanaFichaNuevaCat();
    /*!
      \brief Abre la ficha de la instancia de la categoria seleccionada para modificarla o consultarla.
    */
    void AbrirVentanaFichaModCat();
    /*!
      \brief Comprueba el criterio seleccionado y en caso de ser distinto de TODOS ejecuta la función Filtrar().
      \see Filtrar();
    */
    void ActivarFiltroCat();
    /*!
      \brief Filtra el contenido de la tabla dependiendo del criterio seleccionado.
    */
    void FiltrarCat();
    /*!
      \brief Elimina la instancia de la categoria seleccionada de la base de datos.
    */
    void EliminarCat();

    void ImprimirCat();
    /*!
      \brief Abre una ficha vacía de categorias para crear una nueva.
    */
    void AbrirVentanaFichaNuevaFam();
    /*!
      \brief Abre la ficha de la instancia de la categoria seleccionada para modificarla o consultarla.
    */
    void AbrirVentanaFichaModFam();
    /*!
      \brief Comprueba el criterio seleccionado y en caso de ser distinto de TODOS ejecuta la función Filtrar().
      \see Filtrar();
    */
    void ActivarFiltroFam();
    /*!
      \brief Filtra el contenido de la tabla dependiendo del criterio seleccionado.
    */
    void FiltrarFam();
    /*!
      \brief Elimina la instancia de la categoria seleccionada de la base de datos.
    */
    void EliminarFam();

    void ImprimirFam();
    /*!
      \brief Comprueba el criterio seleccionado en el filtro y en caso de ser distinto de TODOS ejecuta la función Filtrar().
      \see Filtrar();
    */
    void ActivarFiltroHistCert();
    /*!
      \brief Filtra el contenido de la tabla de certificación dependiendo del criterio seleccionado.
    */
    void FiltrarHistCert();

    void ImprimirHistCert();
    void ImprimirProd();
    void AbrirVentanaFichaModProd();
    void AbrirVentanaFichaNuevoProd();
    void ActivarFiltroProd();
    void FiltrarProd();
    void EliminarProducto();
protected:
    /*!
      \brief Controlador de eventos que maneja los cambios de estado.
    */
    void changeEvent(QEvent *e);

private:
    /*!
      \brief Deshabilita los controles dependiendo de si el usuario actual tiene o no permisos de modificación.
    */
    void DeshabilitarControles();
    /*!
      \brief Carga los datos contenidos en la base de datos en el atributo modelConsulta y los muestra en la pantalla.
    */
    void CargarLineasFam();
    /*!
      \brief Carga los datos contenidos en la base de datos en el atributo modelConsulta y los muestra en la pantalla.
    */
    void CargarLineasCat();
    void CargarLineaHistCert();
    void CargarLineasProd();
    Ui::listas *ui;
};

#endif // LISTAS_H
