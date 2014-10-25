#include "movimientosstock.h"
#include "Productos/productoslista.h"
#include "ui_movimientosstock.h"
#include "Productos/producto.h"
#include "../clsconfiguracion.h"
#include "clsimprimirmovimientos.h"
#include "movimientos.h"
#include "Centros/centro.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QCalendarWidget>

extern clsConfiguracion *config;

MovimientosStock::MovimientosStock(clsMovimiento_Stock _Stock,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MovimientosStock)
{
    ui->setupUi(this);

    QCalendarWidget *FechaStock = new QCalendarWidget();
    FechaStock->setFirstDayOfWeek(Qt::Monday);
    ui->dedtFechaStock->setCalendarWidget(FechaStock);
    ui->dedtFechaStock->setDate(QDate::currentDate());

    Stock = new clsMovimiento_Stock(_Stock);
    if(Stock->CodProd() != "")
    {
        Stock->Cargar(_Stock.CodProd(),_Stock.Fecha(),_Stock.Centro(),_Stock.Origen());

        ui->dedtFechaStock->setReadOnly(true);
        ui->edtCantidad->setReadOnly(true);
        ui->edtComentario->setReadOnly(true);
        ui->edtDescripcion->setReadOnly(true);
        ui->edtProducto->setReadOnly(true);
        ui->btnProducto->setEnabled(false);
        ui->rbtnEntrada->setEnabled(false);
        ui->rbtnSalida->setEnabled(false);

        CargarDatos();
    }

    connect(ui->buttonBox,SIGNAL(accepted()),this,SLOT(Guardar()));
    connect(ui->buttonBox,SIGNAL(rejected()),this,SLOT(accept()));
    connect(ui->btnProducto,SIGNAL(clicked()),this,SLOT(AbrirVentanaProd()));
    connect(ui->btnImprimir,SIGNAL(clicked()),this,SLOT(Imprimir()));
}

MovimientosStock::~MovimientosStock()
{
    delete ui;
    delete Stock;
}

void MovimientosStock::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MovimientosStock::Imprimir()
{
    clsImprimirMovimientos Inf;
    Inf.ImprimirFichaStock(Stock);
}

void MovimientosStock::CargarDatos()
{
    clsCentros Centro;
    Centro.Cargar(Stock->Centro());

    clsProductos Prod;
    Prod.Cargar(Stock->CodProd());

    ui->dedtFechaStock->setDate(Stock->Fecha());
    ui->edtCantidad->setText(QString::number(Stock->Cantidad()));
    ui->edtComentario->setText(Stock->Comentario());
    ui->edtDescripcion->setText(Prod.Descripcion());
    ui->edtProducto->setText(Stock->CodProd());
    if(qlOrigen[Stock->Origen()] == QObject::tr("Entrada"))        
        ui->rbtnEntrada->setChecked(true);
    else
        ui->rbtnSalida->setChecked(true);
}

void MovimientosStock::Guardar()
{
    tOrigen eOrigen;
    if (ui->rbtnEntrada->isChecked())
        eOrigen = Entrada;
    else
        eOrigen = Salida;

    clsMovimiento_Stock MovStock(ui->edtProducto->text(),ui->dedtFechaStock->date(),config->IdCentroActual(),eOrigen);

    MovStock.setCantidad(ui->edtCantidad->text().toInt());
    MovStock.setComentario(ui->edtComentario->text());

    if(MovStock.Insertar())
        this->accept();
    else
        this->reject();
}

void MovimientosStock::AbrirVentanaProd()
{
    ProductosLista *ventanaProd = new ProductosLista();
    ventanaProd->setWindowModality(Qt::ApplicationModal);
    ventanaProd->show();
    if (ventanaProd->exec() == QDialog::Accepted)
    {
        clsProductos Prod;
        Prod.Cargar(ventanaProd->CodProdSeleccionado());
        ui->edtProducto->setText(Prod.Codigo());
        ui->edtDescripcion->setText(Prod.Descripcion());
    }
}
