#ifndef MOVIMIENTO_H
#define MOVIMIENTO_H

/*!
  \class Movimiento_Salida
  \brief En esta clase se incluyen los métodos y atributos que tienen relación con el manejo de las salidas, su creación,
         modificación y eliminación.
*/

/*!
  \class Movimiento_DetSal
  \brief En esta clase se incluyen los métodos y atributos que tienen relación con la inclusión, modificación, y eliminación de los productos en una salida.
*/

/*!
  \class Movimiento_DetEnt
  \brief En esta clase se incluyen los métodos y atributos que tienen relación con la entrada de los productos de una salida.
*/

/*!
  \class Movimiento_Pedido
  \brief En esta clase se incluyen los métodos y atributos que tienen relación con el manejo de los pedidos internos de productos, su creación,
         modificación y eliminación.
*/

/*!
  \class Movimiento_DetPed
  \brief En esta clase se incluyen los métodos y atributos que tienen relación con la inclusión, modificación y eliminación de productos en un pedido interno.
*/

/*!
  \class Movimiento_Envio
  \brief En esta clase se incluyen los métodos y atributos que tienen relación con el manejo de los envíos de productos a otros centros, su creación,
         modificación y eliminación.
*/

/*!
  \class Movimiento_DetEnv
  \brief En esta clase se incluyen los métodos y atributos que tienen relación con la inclusión, modificación y eliminación de productos en envíos a otros centros.
*/

/*!
  \class Movimiento_Stock
  \brief En esta clase se incluyen los métodos y atributos que tienen relación con la inclusión, modificación y eliminación de productos en envíos a otros centros.
*/

#include <QString>
#include <QDate>
#include <QList>
#include <QStringList>

class clsMovimiento_DetSal;
class clsMovimiento_DetEnt;
class clsMovimiento_DetPed;
class clsMovimiento_DetEnv;

enum tOrigen{Entrada,Salida};

static QStringList qlOrigen = (QStringList() << QObject::tr("Entrada") << QObject::tr("Salida"));

class clsMovimiento_Salida
{
public:
    /*!
      \brief Constructor de la clase. Se asignan a los atributos el valor de los parámetros de entrada.
      \param _Sal Parámetro de tipo int que almacena el número de la salida de productos. Por defecto es 0.
      \param _proy Parámetro de tipo QString que almacena el proyecto de la salida de producto. Por defecto es "".
      \param _tarea Parámetro de tipo QString que indica la tarea de la salida de productos. Por defecto es "".
      \param _op Parámetro de tipo QString que indica el operario responsable de la salida de productos. Por defecto es "".
    */
    clsMovimiento_Salida(int _Sal=0,QString _proy="",QString _tarea="",QString _op="");
    /*!
      \brief Destructor de la clase.
    */
    ~clsMovimiento_Salida();

    /*!
      \brief Devuelve el número de la salida de productos.
      \return Devuelve un objeto de tipo int con el número de la salida de productos.
    */
    int NoSalida() const;
    /*!
      \brief Devuelve el proyecto de la salida de productos.
      \return Devuelve un objeto de tipo QString con el proyecto de la salida de productos.
    */
    QString Proyecto() const;
    /*!
      \brief Devuelve la tarea de la salida de productos.
      \return Devuelve un objeto de tipo QString con la tarea de la salida de productos.
    */
    QString Tarea() const;
    /*!
      \brief Devuelve el operario responsable de la salida de productos.
      \return Devuelve un objeto de tipo int con el operario responsable de la salida de productos.
    */
    QString Operario() const;
    /*!
      \brief Devuelve el centro donde se realiza la salida.
      \return Devuelve un objeto de tipo int con el centro donde se realiza la salida.
    */
    int Centro() const;
    /*!
      \brief Devuelve la fecha en la que se creó la salida.
      \return Devuelve un objeto de tipo QDate la fecha en la que se creó la salida.
    */
    QDate Fecha_Creacion() const;
    QString Comentarios() const;
    void setComentarios(QString _Coment);
    /*!
      \brief Modifica el proyecto de la salida de productos.
      \param _proy Contiene el valor a asignar al atributo strProyecto.
    */
    void setProyecto(QString _proy);
    /*!
      \brief Modifica la tarea de la salida de productos.
      \param _tarea Contiene el valor a asignar al atributo strTarea.
    */
    void setTarea(QString _tarea);
    /*!
      \brief Modifica el operario de la salida de productos.
      \param _op Contiene el valor a asignar al atributo strOperario.
    */
    void setOperario(QString _op);
    /*!
      \brief Devuelve la lista de productos incluidos en la salida.
      \return Devuelve un objeto de tipo QList<Movimiento_DetSal> con la lista de productos incluidos en la salida.
    */
    const QList<clsMovimiento_DetSal>& DetallesSalida() const;
    /*!
      \brief Inserta los datos de la instancia en la base de datos.
      \pre El operario, el proyecto y la tarea deben ser distintos de la cadena vacía.
    */
    bool Insertar();
    /*!
      \brief Modifica los datos de la instancia en la base de datos.
      \pre El operario, el proyecto y la tarea deben ser distintos de la cadena vacía.
    */
    bool Modificar();
    /*!
      \brief Elimina los datos de la instancia en la base de datos..
    */
    void Eliminar();
    /*!
      \brief Carga los datos almacenados en la base de datos en la instancia de la clase.
      \param _Salida Contiene el codigo de la salida de productos.
    */
    void Cargar(int _Salida);
    /*!
      \brief Comprueba si la instancia de la clase ha sido almacenada en la base de datos.
      \return Devuelve True si la instancia ha sido almacenda en la base de datos, y False en caso contrario.
    */
    bool Existe() const;
    QVector<QStringList> ListaSalida(QString filtro="");
private:
    /*!
      \brief Atributo que almacena la lista de productos incluidos en la salida.
    */
    QList<clsMovimiento_DetSal> SalDetalles;
    /*!
      \brief Atributo que almacena el número de la salida de productos.
    */
    int intNoSalida;
    /*!
      \brief Atributo que almacena el centro donde se realiza la salida de productos.
    */
    int intCentro;
    /*!
      \brief Atributo que almacena el proyecto de la salida de productos.
    */
    QString strProyecto;
    /*!
      \brief Atributo que almacena la tarea de la salida de productos.
    */
    QString strTarea;
    /*!
      \brief Atributo que almacena el operario responsable de la salida de productos.
    */
    QString strOperario;
    /*!
      \brief Atributo que almacena la fecha en la que se creó la salida.
    */
    QDate datFecha_creacion;
    QString strComentarios;
};

class clsMovimiento_DetSal
{
public:
    /*!
      \brief Constructor de la clase. Se asignan a los atributos el valor de los parámetros de entrada.
      \param Sal Parámetro de tipo int que almacena el número de la salida de productos. Por defecto es 0.
      \param CodProd Parámetro de tipo QString que almacena el código del producto. Por defecto es "".
      \param Cant Parámetro de tipo int que indica la cantidad del producto. Por defecto es 0.
      \param _Fecha_Salida Parámetro de tipo QDate que indica la fecha en la que se incluye el producto a la salida. Por defecto es 01/01/2000.
    */
    clsMovimiento_DetSal(int Sal=0,QString CodProd="",int Cant=0,QDate _Fecha_Salida=QDate(2000,01,01));
    /*!
      \brief Constructor por copia de la clase. Se asignan a los atributos el valor de los parámetros de entrada.
      \param SalDetalle Parámetro de tipo Movimiento_DetSal que almacena la clase base.
    */
    clsMovimiento_DetSal(const clsMovimiento_DetSal& SalDetalle);
    /*!
      \brief Destructor de la clase.
    */
    ~clsMovimiento_DetSal();

    /*!
      \brief Devuelve el número de la salida de producto.
      \return Devuelve un objeto de tipo int con el número de la salida de producto.
    */
    int Salida() const;
    /*!
      \brief Devuelve la cantidad de la línea de la salida de producto.
      \return Devuelve un objeto de tipo int con la cantidad de la linea de la salida de producto.
    */
    int Cantidad() const;
    /*!
      \brief Devuelve el codigo del producto de la línea de la salida.
      \return Devuelve un objeto de tipo QString con el codigo del producto de la línea de la salida.
    */
    QString CodProd() const;
    /*!
      \brief Devuelve la fecha en la que fue incluida la linea en la salida de producto.
      \return Devuelve un objeto de tipo QDate la fecha en la que fue incluida la linea en la salida de producto.
    */
    QDate Fecha_Salida() const;
    /*!
      \brief Inserta los datos de la instancia en la base de datos.
      \pre El atributo strCodEPI debe tener un valor distinto de la cadena vacía, la cantidad debe ser mayor que cero, y la salida donde se va a incluir la línea debe existir.
    */
    bool Insertar();
    /*!
      \brief Modifica los datos de la instancia en la base de datos.
      \pre El atributo strCodEPI debe tener un valor distinto de la cadena vacía, la cantidad debe ser mayor que cero, y la salida donde se va a incluir la línea debe existir.
    */
    bool Modificar();
    /*!
      \brief Elimina los datos de la instancia en la base de datos..
    */
    bool Eliminar();
    /*!
      \brief Carga los datos almacenados en la base de datos en la instancia de la clase.
      \param Sal Contiene el codigo de la salida de producto.
      \param prod Contiene el codigo del producto.
      \param fecha Contiene la fecha en la que fue incluido el producto en la salida.
    */
    void Cargar(int Sal,QString prod,QDate fecha);
    /*!
      \brief Comprueba si la instancia de la clase ha sido almacenada en la base de datos.
      \return Devuelve True si la instancia ha sido almacenda en la base de datos, y False en caso contrario.
    */
    bool Existe() const;
    /*!
      \brief Modifica la cantidad del producto en la línea de la salida.
      \param _Cant Contiene el valor a asignar al atributo intCantidad.
    */
    void setCantidad(int _Cant);
    /*!
      \brief Devuelve la lista de productos devueltas o perdidas de la salida.
      \return Devuelve un objeto de tipo QList<Movimiento_DetEnt> con la lista de productos incluidos devueltas o perdidas de la salida.
    */
    const QList<clsMovimiento_DetEnt>& DetallesEntrada() const;
private:
    /*!
      \brief Atributo que almacena la lista de productos incluidos devueltas o perdidas de la salida.
    */
    QList<clsMovimiento_DetEnt> EntDetalles;
    /*!
      \brief Atributo que almacena el número de la salida de producto.
    */
    int intNoSalida;
    /*!
      \brief Atributo que almacena la cantidad del producto en la línea de la salida.
    */
    int intCantidad;
    /*!
      \brief Atributo que almacena el código del producto en la línea de la salida.
    */
    QString strCodProd;
    /*!
      \brief Atributo que almacena la fecha en la que fue incluida el producto en la salida.
    */
    QDate datFecha_salida;
};

class clsMovimiento_DetEnt
{
public:
    /*!
      \brief Constructor de la clase. Se asignan a los atributos el valor de los parámetros de entrada.
      \param Sal Parámetro de tipo int que almacena el número de la salida de productos. Por defecto es 0.
      \param _Devueltas Parámetro de tipo int que almacena la cantidad introducida como devuelta del producto. Por defecto es "".
      \param _Perdidas Parámetro de tipo int que indica la cantidad introducida como perdida del producto. Por defecto es 0.
      \param _Fecha_Entrada Parámetro de tipo QDate que indica la fecha en la que se devuelve el producto de la salida. Por defecto es 01/01/2000.
      \param _Fecha_Salida Parámetro de tipo QDate que indica la fecha en la que se realiza la salida del producto. Por defecto es 01/01/2000.
    */
    clsMovimiento_DetEnt(int Sal=0,int _Devueltas=0,int _Perdidas=0,QString CodProd="",QDate _Fecha_Entrada=QDate(2000,01,01),QDate _Fecha_Salida=QDate(2000,01,01));
    /*!
      \brief Constructor por copia de la clase. Se asignan a los atributos el valor de los parámetros de entrada.
      \param EntDetalle Parámetro de tipo Movimiento_DetEnt que almacena la clase base.
    */
    clsMovimiento_DetEnt(const clsMovimiento_DetEnt& EntDetalle);
    /*!
      \brief Destructor de la clase.
    */
    ~clsMovimiento_DetEnt();

    /*!
      \brief Devuelve el número de la salida de producto.
      \return Devuelve un objeto de tipo int con el número de la salida de producto.
    */
    int Salida() const;
    /*!
      \brief Devuelve la cantidad devuelta del producto en la salida.
      \return Devuelve un objeto de tipo int con la cantidad devuelta de la salida.
    */
    int Devueltas() const;
    /*!
      \brief Devuelve la cantidad perdida del producto en la salida.
      \return Devuelve un objeto de tipo int con la cantidad perdida en la salida.
    */
    int Perdidas() const;
    /*!
      \brief Devuelve la fecha en la que fue devuelto el producto de la salida.
      \return Devuelve un objeto de tipo QDate la fecha en la que fue devuelto el producto de la salida.
    */
    QDate Fecha_Entrada() const;
    /*!
      \brief Devuelve la fecha en la que fue incluida la linea en la salida de producto.
      \return Devuelve un objeto de tipo QDate la fecha en la que fue incluida la linea en la salida de producto.
    */
    QDate Fecha_Salida() const;
    QString CodProd() const;
    /*!
      \brief Inserta los datos de la instancia en la base de datos.
      \pre El atributo strCodEPI debe tener un valor distinto de la cadena vacía y la salida donde se va a incluir la línea debe existir.
    */
    bool Insertar();
    /*!
      \brief Modifica los datos de la instancia en la base de datos.
      \pre El atributo strCodEPI debe tener un valor distinto de la cadena vacía y la salida donde se va a incluir la línea debe existir.
    */
    bool Modificar();
    /*!
      \brief Elimina los datos de la instancia en la base de datos.
    */
    bool Eliminar();
    /*!
      \brief Carga los datos almacenados en la base de datos en la instancia de la clase.
      \param Sal Contiene el codigo de la salida de producto.
      \param prod Contiene el codigo de la producto.
      \param fecha Contiene la fecha en la que fue devueto el producto de la salida.
    */
    void Cargar(int Sal,QString prod,QDate fecha_sal,QDate fecha_ent);
    /*!
      \brief Comprueba si la instancia de la clase ha sido almacenada en la base de datos.
      \return Devuelve True si la instancia ha sido almacenda en la base de datos, y False en caso contrario.
    */
    bool Existe() const;
    /*!
      \brief Modifica la cantidad devuelta del producto en la línea de la entrada.
      \param _Dev Contiene el valor a asignar al atributo intDevueltas.
    */
    void setDevueltas(int _Dev);
    /*!
      \brief Modifica la cantidad perdida del producto en la línea de la entrada.
      \param _Perd Contiene el valor a asignar al atributo intPerdidas.
    */
    void setPerdidas(int _Perd);
private:
    /*!
      \brief Atributo que almacena el número de la salida de producto.
    */
    int intNoSalida;
    /*!
      \brief Atributo que almacena la cantidad devuelta del producto en la línea de la entrada.
    */
    int intDevueltas;
    /*!
      \brief Atributo que almacena la cantidad perdida del producto en la línea de la entrada.
    */
    int intPerdidas;
    /*!
      \brief Atributo que almacena el código del producto en la línea de la entrada.
    */
    QString strProd;
    /*!
      \brief Atributo que almacena la fecha en la que fue devuelto el producto en la salida.
    */
    QDate datFecha_entrada;
    /*!
      \brief Atributo que almacena la fecha en la que fue incluida el producto en la salida.
    */
    QDate datFecha_salida;
};

class clsMovimiento_Pedido
{
public:
    /*!
      \brief Constructor de la clase. Se asignan a los atributos el valor de los parámetros de entrada.
      \param _Ped Parámetro de tipo int que almacena el número de pedido interno de productos. Por defecto es 0.
    */
    clsMovimiento_Pedido(int _Ped=0);
    /*!
      \brief Destructor de la clase.
    */
    ~clsMovimiento_Pedido();

    /*!
      \brief Devuelve el número del pedido interno de productos.
      \return Devuelve un objeto de tipo int con el número del pedido interno de productos.
    */
    int NoPedido() const;
    /*!
      \brief Devuelve el centro donde se realiza el pedido interno.
      \return Devuelve un objeto de tipo int con el centro donde se realiza el pedido interno.
    */
    int Centro() const;
    /*!
      \brief Devuelve la lista de productos incluidos en el pedido interno.
      \return Devuelve un objeto de tipo QList<Movimiento_DetPed> con la lista de productos incluidos en el pedido interno.
    */
    const QList<clsMovimiento_DetPed>& Detalles() const;
    /*!
      \brief Inserta los datos de la instancia en la base de datos.
    */
    bool Insertar();
    /*!
      \brief Modifica los datos de la instancia en la base de datos.
    */
    bool Modificar();
    /*!
      \brief Elimina los datos de la instancia en la base de datos.
      \pre El pedido interno no puede haber sido servido parcial o completamente.
    */
    bool Eliminar();
    /*!
      \brief Carga los datos almacenados en la base de datos en la instancia de la clase.
      \param _Pedido Contiene el codigo del pedido interno de productos.
    */
    void Cargar(int _Pedido);
    /*!
      \brief Comprueba si la instancia de la clase ha sido almacenada en la base de datos.
      \return Devuelve True si la instancia ha sido almacenda en la base de datos, y False en caso contrario.
    */
    bool Existe() const;
    /*!
      \brief Devuelve la fecha en la que se creó el pedido.
      \return Devuelve un objeto de tipo QDate la fecha en la que fue incluida la linea en la salida de producto.
    */
    QDate Fecha() const;
    bool SeHaServido() const;
    QString Comentarios() const;
    void setComentarios(QString _Coment);
    QVector<QStringList> ListaPedido(QString filtro="");
private:
    /*!
      \brief Atributo que almacena la lista de productos incluidos en el pedido interno.
    */
    QList<clsMovimiento_DetPed> PedDetalles;
    /*!
      \brief Atributo que almacena el número del pedido interno.
    */
    int intNoPedido;
    /*!
      \brief Atributo que almacena el centro donde se realiza el pedido interno.
    */
    int intCentro;
    /*!
      \brief Atributo que almacena la fecha en la que se creó el pedido.
    */
    QDate datFecha;
    QString strComentarios;
};

class clsMovimiento_DetPed
{
public:
    /*!
      \brief Constructor de la clase. Se asignan a los atributos el valor de los parámetros de entrada.
      \param Ped Parámetro de tipo int que almacena el número del pedido interno de productos. Por defecto es 0.
      \param CodProd Parámetro de tipo QString que almacena el código del producto. Por defecto es "".
      \param Cant Parámetro de tipo int que indica la cantidad del producto. Por defecto es 0.
    */
    clsMovimiento_DetPed(int Ped=0,QString CodProd="",int Cant=0);
    /*!
      \brief Constructor por copia de la clase. Se asignan a los atributos el valor de los parámetros de entrada.
      \param Detalle Parámetro de tipo Movimiento_DetPed que almacena la clase base.
    */
    clsMovimiento_DetPed(const clsMovimiento_DetPed& Detalle);
    /*!
      \brief Destructor de la clase.
    */
    ~clsMovimiento_DetPed();

    /*!
      \brief Devuelve el número del pedido interno de productos.
      \return Devuelve un objeto de tipo int con el número del pedido interno de productos.
    */
    int Pedido() const;
    /*!
      \brief Devuelve la cantidad de la línea del pedido interno de productos.
      \return Devuelve un objeto de tipo int con la cantidad de la linea del pedido interno de productos.
    */
    int Cantidad() const;
    /*!
      \brief Devuelve el codigo del producto de la línea del pedido interno.
      \return Devuelve un objeto de tipo QString con el codigo del producto de la línea del pedido interno.
    */
    QString CodProd() const;
    /*!
      \brief Inserta los datos de la instancia en la base de datos.
      \pre El código del producto debe ser distinto de la cadena vacía y la cantidad debe ser mayor que cero.
    */
    bool Insertar();
    /*!
      \brief Modifica los datos de la instancia en la base de datos.
      \pre El código del producto debe ser distinto de la cadena vacía y la cantidad debe ser mayor que cero.
    */
    bool Modificar();
    /*!
      \brief Elimina los datos de la instancia en la base de datos.
      \pre La cantidad servida de la linea debe ser cero.
    */
    bool Eliminar();
    /*!
      \brief Carga los datos almacenados en la base de datos en la instancia de la clase.
      \param Ped Contiene el codigo del pedido interno de productos.
    */
    void Cargar(int Ped,QString Prod,QDate fechaenv);
    /*!
      \brief Comprueba si la instancia de la clase ha sido almacenada en la base de datos.
      \return Devuelve True si la instancia ha sido almacenda en la base de datos, y False en caso contrario.
    */
    bool Existe() const;
    /*!
      \brief Modifica la cantidad del producto en la línea del pedido interno.
      \param _Cant Contiene el valor a asignar al atributo intCantidad.
    */
    void setCantidad(int _Cant);
    bool Servido() const;
    /*!
      \brief Devuelve la cantidad servida por un centro de la línea del pedido interno de productos.
      \return Devuelve un objeto de tipo int con la cantidad servida por un centro de la linea del pedido interno de productos.
    */
    int CantidadServida() const;
    /*!
      \brief Devuelve la fecha en la que se creó el pedido.
      \return Devuelve un objeto de tipo QDate la fecha en la que fue incluida la linea en la salida de producto.
    */
    QDate Fecha() const;
    bool Servir(int NoEnvio,QDate datFechaEnv);
private:
    /*!
      \brief Atributo que almacena el número del pedido interno.
    */
    int intNoPedido;
    /*!
      \brief Atributo que almacena la cantidad del producto en la línea del pedido interno.
    */
    int intCantidad;
    /*!
      \brief Atributo que almacena el código del producto en la línea del pedido interno.
    */
    QString strCodProd;
    /*!
      \brief Atributo que almacena la fecha en la que se creó el pedido.
    */
    QDate datFecha;

    bool boServido;
};

class clsMovimiento_Envio
{
public:
    /*!
      \brief Constructor de la clase. Se asignan a los atributos el valor de los parámetros de entrada.
      \param _Env Parámetro de tipo int que almacena el número de envío del producto. Por defecto es 0.
      \param _CentroOrigen Parámetro de tipo int que almacena el centro de origen del envío del producto. Por defecto es 0.
      \param _CentroDestino Parámetro de tipo int que indica el centro de destino del envío del producto. Por defecto es 0.
      \param _FechaCreacion Parámetro de tipo QDate que indica la fecha en la que se realiza el envío del producto. Por defecto es 01/01/2000.
    */
    clsMovimiento_Envio(int _Env=0,int _CentroOrigen=0,int _CentroDestino=0,QDate _FechaCreacion=QDate(1,1,2000));
    /*!
      \brief Destructor de la clase.
    */
    ~clsMovimiento_Envio();

    /*!
      \brief Devuelve el número del envío del producto.
      \return Devuelve un objeto de tipo int con el número del envío del producto.
    */
    int NoEnvio() const;
    /*!
      \brief Devuelve el centro donde se realiza le envío.
      \return Devuelve un objeto de tipo int con el centro donde se realiza el envío.
    */
    int CentroOrigen() const;
    /*!
      \brief Devuelve el centro al cual va dirigido el envío.
      \return Devuelve un objeto de tipo int con el centro al cual va dirigido el envío.
    */
    int CentroDestino() const;
    /*!
      \brief Devuelve la fecha en la que fue creado el envío.
      \return Devuelve un objeto de tipo QDate la fecha en la que fue creado el envío.
    */
    QDate FechaCreacion() const;
    /*!
      \brief Devuelve la lista del producto incluidos en el envío.
      \return Devuelve un objeto de tipo QList<Movimiento_DetEnv> con la lista del producto incluidos en el envío.
    */
    const QList<clsMovimiento_DetEnv>& Detalles() const;
    /*!
      \brief Inserta los datos de la instancia en la base de datos.
      \pre El envío debe tener un centro destino, y una fecha de envío.
    */
    bool Insertar();
    /*!
      \brief Modifica los datos de la instancia en la base de datos.
      \pre El envío debe tener un centro destino, y una fecha de envío.
    */
    bool Modificar();
    /*!
      \brief Elimina los datos de la instancia en la base de datos.
    */
    bool Eliminar();
    /*!
      \brief Carga los datos almacenados en la base de datos en la instancia de la clase.
      \param _Envio Contiene el código del envío del producto.
    */
    void Cargar(int _Envio);
    /*!
      \brief Comprueba si la instancia de la clase ha sido almacenada en la base de datos.
      \return Devuelve True si la instancia ha sido almacenda en la base de datos, y False en caso contrario.
    */
    bool Existe() const;
    /*!
      \brief Modifica el centro al cual va dirigido el envío.
      \param _CentroDestino Contiene el valor a asignar al atributo intCentroDestino.
    */
    void setCentroDestino(int _CentroDestino);
    void setFechaCreacion(QDate _FechaCrea);
    QString Comentarios() const;
    void setComentarios(QString _Coment);
    QVector<QStringList> ListaEnvio(QString filtro="");
private:
    /*!
      \brief Atributo que almacena la lista del producto incluidos en el envío.
    */
    QList<clsMovimiento_DetEnv> EnvDetalles;
    /*!
    \brief Atributo que almacena el centro donde se realiza el envío.
    */
    int intCentroOrigen;
    /*!
    \brief Atributo que almacena el centro al cual va dirigido el envío.
    */
    int intCentroDestino;
    /*!
      \brief Atributo que almacena el número del envío.
    */
    int intNoEnvio;
    /*!
      \brief Atributo que almacena la fecha en la que se crea el envío.
    */
    QDate datFechaCreacion;
    QString strComentarios;
};

class clsMovimiento_DetEnv
{
public:
    /*!
      \brief Constructor de la clase. Se asignan a los atributos el valor de los parámetros de entrada.
      \param Env Parámetro de tipo int que almacena el número del envío del producto. Por defecto es 0.
      \param CodProd Parámetro de tipo QString que almacena el código del producto. Por defecto es "".
      \param Cant Parámetro de tipo int que indica la cantidad del producto. Por defecto es 0.
    */
    clsMovimiento_DetEnv(int Env=0,QString CodProd="",int Cant=0);
    /*!
      \brief Constructor por copia de la clase. Se asignan a los atributos el valor de los parámetros de entrada.
      \param Detalle Parámetro de tipo EPI_DetEnv que almacena la clase base.
    */
    clsMovimiento_DetEnv(const clsMovimiento_DetEnv& Detalle);
    /*!
      \brief Destructor de la clase.
    */
    ~clsMovimiento_DetEnv();

    /*!
      \brief Devuelve el número del envío del producto.
      \return Devuelve un objeto de tipo int con el número del envío del producto.
    */
    int Envio() const;
    /*!
      \brief Devuelve la cantidad de la línea del envío del producto.
      \return Devuelve un objeto de tipo int con la cantidad de la linea del envío del producto.
    */
    int Cantidad() const;
    /*!
      \brief Devuelve el codigo del producto de la línea del envío.
      \return Devuelve un objeto de tipo QString con el codigo del producto de la línea del envío.
    */
    QString CodProd() const;
    /*!
      \brief Comprueba si el la línea del envío ha sido recibida en el centro destino.
      \return Devuelve True si el la línea del envío ha sido recibida en el centro destino, y False en caso contrario.
    */
    bool Recibida() const;
    /*!
      \brief Inserta los datos de la instancia en la base de datos.
      \pre El código del producto debe ser distinto de la cadena vacía y la cantidad debe ser mayor que cero.
    */
    bool Insertar();
    /*!
      \brief Modifica los datos de la instancia en la base de datos.
      \pre El código del producto debe ser distinto de la cadena vacía y la cantidad debe ser mayor que cero.
    */
    bool Modificar();
    /*!
      \brief Elimina los datos de la instancia en la base de datos.
      \pre La línea no puede estar recibida en el centro destino.
    */
    bool Eliminar();
    /*!
      \brief Carga los datos almacenados en la base de datos en la instancia de la clase.
      \param Env Contiene el codigo del envío del producto.
      \param prod Contiene el codigo del producto.
    */
    void Cargar(int Env,QString prod,QDate Fecha);
    /*!
      \brief Comprueba si la instancia de la clase ha sido almacenada en la base de datos.
      \return Devuelve True si la instancia ha sido almacenda en la base de datos, y False en caso contrario.
    */
    bool Existe() const;
    /*!
      \brief Modifica la cantidad del producto en la línea del envío.
      \param _Cant Contiene el valor a asignar al atributo intCantidad.
    */
    void setCantidad(int _Cant);
    /*!
      \brief Recibe el producto seleccionado.
    */
    bool Recibir();
    QDate FechaEnvio() const;
private:
    /*!
      \brief Atributo que almacena el número del envío.
    */
    int intNoEnvio;
    /*!
      \brief Atributo que almacena la cantidad del producto en la línea del envío.
    */
    int intCantidad;
    /*!
      \brief Atributo que almacena el código del producto en la línea del envío.
    */
    QString strCodProd;
    /*!
      \brief Atributo que indica si la línea del envío ha sido recibida en el centro destino.
    */
    bool boRecibida;
    /*!
      \brief Atributo que almacena la fecha en la que se crea el envío.
    */
    QDate datFecha;
};

class clsMovimiento_Stock
{
public:
    /*!
      \brief Constructor de la clase. Se asignan a los atributos el valor de los parámetros de entrada.
      \param _Stock Parámetro de tipo int que almacena el número del movimiento de stock. Por defecto es 0.
    */
    clsMovimiento_Stock(QString _Prod="",QDate _datStock=QDate(2000,01,01),int _Centro=0,tOrigen _Origen=Entrada);
    /*!
      \brief Destructor de la clase.
    */
    ~clsMovimiento_Stock();

    /*!
      \brief Devuelve el número del movimiento de stock.
      \return Devuelve un objeto de tipo int con el número del movimiento de stock.
    */
    QString CodProd() const;
    /*!
      \brief Devuelve el centro donde se realiza el pedido interno.
      \return Devuelve un objeto de tipo int con el centro donde se realiza el pedido interno.
    */
    int Centro() const;
    /*!
      \brief Devuelve la fecha en la que se creó el pedido.
      \return Devuelve un objeto de tipo QDate la fecha en la que fue incluida la linea en la salida de producto.
    */
    QDate Fecha() const;
    /*!
      \brief Devuelve la fecha en la que se creó el pedido.
      \return Devuelve un objeto de tipo QDate la fecha en la que fue incluida la linea en la salida de producto.
    */
    QString Comentario() const;
    /*!
      \brief Devuelve la fecha en la que se creó el pedido.
      \return Devuelve un objeto de tipo QDate la fecha en la que fue incluida la linea en la salida de producto.
    */
    int Cantidad() const;
    /*!
      \brief Devuelve la fecha en la que se creó el pedido.
      \return Devuelve un objeto de tipo QDate la fecha en la que fue incluida la linea en la salida de producto.
    */
    tOrigen Origen() const;
    /*!
      \brief Inserta los datos de la instancia en la base de datos.
    */
    bool Insertar();
    /*!
      \brief Modifica los datos de la instancia en la base de datos.
    */
    bool Modificar();
    /*!
      \brief Elimina los datos de la instancia en la base de datos.
      \pre El pedido interno no puede haber sido servido parcial o completamente.
    */
    bool Eliminar();
    /*!
      \brief Carga los datos almacenados en la base de datos en la instancia de la clase.
      \param _Pedido Contiene el codigo del pedido interno de productos.
    */
    void Cargar(QString _Prod,QDate _datStock,int _Centro,tOrigen _Origen);
    /*!
      \brief Comprueba si la instancia de la clase ha sido almacenada en la base de datos.
      \return Devuelve True si la instancia ha sido almacenda en la base de datos, y False en caso contrario.
    */
    bool Existe() const;
    void setComentario(QString _Comentario);
    void setCantidad(int _Cant);
    QVector<QStringList> ListaStock(QString filtro="");
private:
    /*!
      \brief Atributo que almacena el código del producto del movimiento de stock.
    */
    QString strProd;
    /*!
      \brief Atributo que almacena el centro donde se realiza el movimiento de stock.
    */
    int intCentro;
    /*!
      \brief Atributo que almacena la fecha en la que se creó el movimiento de stock.
    */
    QDate datFecha;
    QString strComentario;
    int intCantidad;
    enum tOrigen eOrigen;
};

#endif // MOVIMIENTOS_H
