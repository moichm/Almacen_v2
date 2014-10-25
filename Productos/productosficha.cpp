#include "productosficha.h"
#include "ui_productosficha.h"
#include "certificaciones.h"
#include "categoriaslista.h"
#include "familiaslista.h"
#include "producto.h"
#include "../clsImagen.h"
#include "../clsconfiguracion.h"
#include <QMap>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QMessageBox>
#include <QDateTime>
#include <QFile>
#include <QFileDialog>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QInputDialog>
#include "clsimprimirproductos.h"

extern clsConfiguracion *config;

ProductosFicha::ProductosFicha(QString codigo,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProductosFicha)
{
    ui->setupUi(this);

    Prod = new clsProductos;
    Prod->Cargar(codigo);

    ui->chbCertificable->setEnabled(false);
    ui->chbLimitada->setEnabled(false);

    CargarDatos();

    ActivarCampos();

    connect(ui->btnCertif,SIGNAL(clicked()),this,SLOT(AbrirVentanaFichaCertif()));
    connect(ui->btnCategoria,SIGNAL(clicked()),this,SLOT(AbrirVentanaCategoria()));
    connect(ui->btnFamilia,SIGNAL(clicked()),this,SLOT(AbrirVentanaFamilia()));
    connect(ui->btnExaminar,SIGNAL(clicked()),this,SLOT(AbrirImagenNueva()));
    connect(ui->btnImprimir,SIGNAL(clicked()),this,SLOT(Imprimir()));

    QPushButton *Aplicar = ui->buttonBox->button(QDialogButtonBox::Apply);
    QPushButton *Aceptar = ui->buttonBox->button(QDialogButtonBox::Save);
    QPushButton *Cancelar = ui->buttonBox->button(QDialogButtonBox::Close);
    connect(Aplicar,SIGNAL(clicked()),this,SLOT(Aplicar()));
    connect(Aceptar,SIGNAL(clicked()),this,SLOT(Guardar()));
    connect(Cancelar,SIGNAL(clicked()),this,SLOT(close()));

    DeshabilitarControles();
}

ProductosFicha::~ProductosFicha()
{
    delete Prod;
    delete ui;
}

void ProductosFicha::changeEvent(QEvent *e)
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

void ProductosFicha::CargarDatos()
{    
    if (Prod->Codigo() == "")
        ui->edtCodigo->setReadOnly(false);
    else
    {
        ui->edtCodigo->setReadOnly(true);

        ui->chbDisponible->setEnabled(true);
        ui->edtFechaCertif->setEnabled(true);
        ui->edtFechaProxCertif->setEnabled(true);
        ui->spbFrecCertif->setEnabled(true);
        ui->edtFechaCertif->setText(QDate(Prod->FechaUltimaCertificacion()).toString(QObject::tr("dd/MM/yyyy")));
        ui->edtFechaProxCertif->setText(QDate(Prod->FechaProxCertificacion()).toString(QObject::tr("dd/MM/yyyy")));

        clsFamilias Fam;
        Fam.Cargar(Prod->Familia());

        clsCategorias Cat;
        Cat.Cargar(Prod->Categoria());

        Prod->Cargar(Prod->Codigo());


        ui->spbLimite->setValue(Fam.Limite());
        ui->chbLimitada->setChecked(Fam.Limitada());
        ui->spbFrecCertif->setValue(Fam.FrecuenciaCertificacion());
        ui->edtCodigo->setText(Prod->Codigo());
        ui->edtDescripcion->setText(Prod->Descripcion());
        ui->edtStockCentro->setText(QString::number(Prod->StockCentro()));
        ui->edtStockTotal->setText(QString::number(Prod->StockTotal()));
        ui->edtFamilia->setText(Fam.Descripcion());
        ui->edtCategoria->setText(Cat.Descripcion());
        ui->edtEstado->setText(qlEstado[Prod->Estado()]);
        ui->chbCertificable->setChecked(Fam.Certificable());
        ui->chbDisponible->setChecked(Prod->Disponible());
    }

    QString rutaimgProd = Prod->Imagen();
    if (rutaimgProd == "")
    {
        QFile file;
        if (file.exists(qApp->applicationDirPath()+"/Imagenes/NoPicture.png"))
        {
            clsImagen *clsImg = new clsImagen(qApp->applicationDirPath()+"/Imagenes/NoPicture.png",ui->scAreaImagen->width(),ui->scAreaImagen->height(),ui->scAreaImagen);
            ui->scAreaImagen->setWidget(clsImg);
        }
    }
    else
    {
        QFile file;
        if (file.exists(rutaimgProd))
        {
            clsImagen *clsImg = new clsImagen(rutaimgProd,ui->scAreaImagen->width(),ui->scAreaImagen->height(),ui->scAreaImagen);
            ui->scAreaImagen->setWidget(clsImg);
        }
    }
}

void ProductosFicha::ActivarCampos()
{
    if (ui->chbCertificable->isChecked())
        ui->btnCertif->setEnabled(true);
    else
        ui->btnCertif->setEnabled(true);
}

void ProductosFicha::AbrirVentanaFichaCertif()
{
    if(Prod->Codigo() != "")
    {
        Certificaciones *ventanaFicha = new Certificaciones(ui->edtCodigo->text());
        ventanaFicha->setWindowModality(Qt::ApplicationModal);
        ventanaFicha->show();
    }
}

void ProductosFicha::AbrirVentanaCategoria()
{
    CategoriasLista *ventanaCategorias = new CategoriasLista();
    ventanaCategorias->setWindowModality(Qt::ApplicationModal);
    ventanaCategorias->show();
    if (ventanaCategorias->exec() == QDialog::Accepted)
    {
        clsCategorias Cat;
        Cat.Cargar(ventanaCategorias->CodCatSeleccionado());

        Prod->setCategoria(Cat.Codigo());
        ui->edtCategoria->setText(Cat.Descripcion());
    }
}

void ProductosFicha::AbrirVentanaFamilia()
{
    FamiliasLista *ventanaFamilias = new FamiliasLista();
    ventanaFamilias->setWindowModality(Qt::ApplicationModal);
    ventanaFamilias->show();
    if (ventanaFamilias->exec() == QDialog::Accepted)
    {
        clsFamilias Fam;
        Fam.Cargar(ventanaFamilias->CodFamiliaSeleccionada());

        Prod->setFamilia(Fam.Codigo());
        ui->edtFamilia->setText(Fam.Descripcion());
        ui->spbFrecCertif->setValue(Fam.FrecuenciaCertificacion());
        ui->chbCertificable->setChecked(Fam.Certificable());
        ui->chbLimitada->setChecked(Fam.Limitada());
        ui->spbLimite->setValue(Fam.Limite());
    }
}

void ProductosFicha::Almacenar()
{
    Prod->setCodigo(ui->edtCodigo->text());
    Prod->setDescripcion(ui->edtDescripcion->text());
    if(qlEstado[Prod->Estado()] == QObject::tr("Pendiente de Certificación"))
        QMessageBox::information(0,QObject::tr("Almacén"),QObject::tr("No se puede modificar la disponibilidad de un producto pendiente de certificación."));
    else
        Prod->setDisponible(ui->chbDisponible->isChecked());

    if (!Prod->Existe())
        Prod->Insertar();
    else
        Prod->Modificar();
}

void ProductosFicha::Guardar()
{
    Almacenar();

    this->accept();
}

void ProductosFicha::Aplicar()
{
    Almacenar();

    CargarDatos();
}

void ProductosFicha::AbrirImagenNueva()
{
    QString rutaAplicacion = qApp->applicationDirPath();
    QString nomArchivo = QFileDialog::getOpenFileName(this,QObject::tr("Abrir imagen"), rutaAplicacion+"/Imagenes", QObject::tr("Archivos de imagen (*.png *.jpg *.bmp)"));
    Prod->setImagen(nomArchivo);
    clsImagen *clsImg = new clsImagen(nomArchivo,ui->scAreaImagen->width(),ui->scAreaImagen->height(),ui->scAreaImagen);
    ui->scAreaImagen->setWidget(clsImg);
}

void ProductosFicha::DeshabilitarControles()
{
    bool permiso = config->Permisos().value(ProdMod);
    ui->buttonBox->button(QDialogButtonBox::Apply)->setEnabled(permiso);
    ui->buttonBox->button(QDialogButtonBox::Save)->setEnabled(permiso);
    ui->btnExaminar->setEnabled(permiso);
    ui->btnCategoria->setEnabled(permiso);
    ui->btnFamilia->setEnabled(permiso);

    if (ui->btnCertif->isEnabled())
        ui->btnCertif->setEnabled(permiso);
}

void ProductosFicha::Imprimir()
{
    clsImprimirProductos Inf;
    Inf.ImprimirFichaProd(Prod);
}
