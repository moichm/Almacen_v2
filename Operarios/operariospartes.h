#ifndef OPERARIOSPARTES_H
#define OPERARIOSPARTES_H

#include <QMainWindow>

namespace Ui {
class OperariosPartes;
}

class OperariosPartes : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit OperariosPartes(QWidget *parent = 0);
    ~OperariosPartes();
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
    void ActivarFiltroOp();
    /*!
      \brief Filtra el contenido de la tabla dependiendo del criterio seleccionado.
    */
    void FiltrarOp();
    /*!
      \brief Abre la ficha de la instancia del operario seleccionado para modificarlo o consultarlo.
    */
    void AbrirVentanaFichaModOp();
    /*!
      \brief Abre una ficha de operario vacía para crear uno nuevo.
    */
    void AbrirVentanaFichaNuevoOp();
    /*!
      \brief Elimina la instancia del operario seleccionado de la base de datos.
    */
    void EliminarOperario();

    void ImprimirOp();
    /*!
      \brief Abre la ficha de la instancia del parte de trabajo seleccionado para modificarlo o consultarlo.
    */
    void AbrirVentanaFichaModPT();
    /*!
      \brief Abre una ficha de parte de trabajo vacía para crear uno nuevo.
    */
    void AbrirVentanaFichaNuevoPT();
    /*!
      \brief Comprueba el criterio seleccionado y en caso de ser distinto de TODOS ejecuta la función Filtrar().
      \see Filtrar();
    */
    void ActivarFiltroPT();
    /*!
      \brief Filtra el contenido de la tabla dependiendo del criterio seleccionado.
    */
    void FiltrarPT();
    /*!
      \brief Elimina la instancia del parte de trabajo seleccionado de la base de datos.
    */
    void EliminarPT();
    /*!
      \brief Imprime la lista de partes.
    */
    void ImprimirPT();
    /*!
      \brief Carga los datos contenidos en la base de datos en el atributo modelConsulta y los muestra en la pantalla.
    */
    void CargarLineas();
private:
    Ui::OperariosPartes *ui;
    /*!
      \brief Deshabilita los controles dependiendo de si el usuario actual tiene o no permisos de modificación.
    */
    void DeshabilitarControles();
    /*!
      \brief Carga los datos contenidos en la base de datos en el atributo modelConsulta y los muestra en la pantalla.
    */
    void CargarLineasOp();
    /*!
      \brief Carga los datos contenidos en la base de datos en el atributo modelConsulta y los muestra en la pantalla.
    */
    void CargarLineasPT();
};

#endif // OPERARIOSPARTES_H
