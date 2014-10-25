#ifndef PRINCIPAL_H
#define PRINCIPAL_H

/*!
  \class Principal
  \brief En esta clase se incluye los métodos y atributos que tienen relación con la ventana principal de la aplicación.
*/

#include <QMainWindow>
#include "conexionBD.h"
#include "clsconfiguracion.h"
#include <QCloseEvent>

namespace Ui {
    class Principal;
}

class Principal : public QMainWindow {
    Q_OBJECT
public:
    /*!
      \brief Constructor de la clase.
      \param parent Objeto de tipo QWidget que contiene un puntero al objeto padre de la clase. Por defecto es 0.
    */
    Principal(QWidget *parent = 0);
    /*!
      \brief Destructor de la clase.
    */
    ~Principal();

    /*!
      \brief Atributo que indica si el usuario ha iniciado sesión o no.
    */
    bool boEntra;
private slots:
    void closeEvent(QCloseEvent *event);
    /*!
      \brief Abre la ventana de movimientos en una ventana hija.
    */
    void AbrirVentanaMovs();
    /*!
      \brief Abre la ventana de kits en una ventana hija.
    */
    void AbrirVentanaKits();
    /*!
      \brief Muestra la barra de productos con las opciones del módulo de Productos.
    */
    void AbrirProductos();
    /*!
      \brief Muestra la barra de operarios con las opciones del módulo de Operarios.
    */
    void AbrirOperariosPartes();
    /*!
      \brief Muestra la barra de vehículos con las opciones del módulo de Vehículos.
    */
    void AbrirVehiculos();
    void AbrirCentros();
    /*!
      \brief Cierra la sesión actual.
    */
    void CerrarSesion();
    /*!
      \brief Cierra la sesión actual y ejecuta la función CargarSesion() para abrir una nueva sesión.
      \see CargarSesion()
    */
    void AbrirSesion();
    /*!
      \brief Abre el cuadro de diálogo de inicio de sesión y controla que los datos sean correctos.
    */
    bool CargarSesion();
    /*!
      \brief Deshabilita los controles dependiendo de los permisos del usuario.
    */
    void DeshabilitarControles();
    /*!
      \brief Abre la ventana de configuración de la aplicación.
    */
    void AbrirVentanaOpciones();    
    /*!
      \brief Traduce la aplicación al idioma elegido.
    */
    void Traducir(QAction*);    
    /*!
      \brief Guarda el archivo de configuracón y cierra la aplicación.
    */
    void Salir();

    void CambiarPass();

    void VMosaico();

    void VCascada();

public slots:
    void CerrarTodas();
protected:
    /*!
      \brief Controlador de eventos que maneja los cambios de estado.
    */
    void changeEvent(QEvent *e);

private:
    Ui::Principal *ui;
    /*!
      \brief Atributo de tipo conxionBD que permite la conexión a la base de datos.
    */
    conexionBD *conexion;
    /*!
      \brief Oculta las opciones para las que el usuario no tiene permiso de lectura.
    */
    void OcultarAcciones();
    /*!
      \brief Añade una acción por cada idioma disponible.
    */
    void IdiomasDisponibles();
    void ComprobarProductos();
    /*!
      \brief Atributo que almacena la traducción actual de la aplicación.
    */
    QTranslator *transActual;
};

#endif // PRINCIPAL_H
