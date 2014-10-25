#include "operariosficha.h"
#include "ui_operariosficha.h"
#include "./Operarios/operario.h"
#include "../clsImagen.h"
#include "../Centros/centroslista.h"
#include <QMessageBox>
#include <QDateTime>
#include <QFile>
#include <QFileDialog>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QCalendarWidget>
#include "clsconfiguracion.h"
#include "clsimprimiroperarios.h"

extern clsConfiguracion *config;

OperariosFicha::OperariosFicha(QString _NIF,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OperariosFicha)
{
    ui->setupUi(this);

    QCalendarWidget *FechaNacim = new QCalendarWidget();
    FechaNacim->setFirstDayOfWeek(Qt::Monday);
    ui->datFechaNacimiento->setCalendarWidget(FechaNacim);
    ui->datFechaNacimiento->setDate(QDate::currentDate());

    Op = new clsOperarios(_NIF);

    Centro = new clsCentros();

    CargarDatos();

    QPushButton *btnGuardar = ui->buttonBox->button(QDialogButtonBox::Ok);
    QPushButton *btnCancelar = ui->buttonBox->button(QDialogButtonBox::Cancel);
    connect(ui->btnCentro,SIGNAL(clicked()),this,SLOT(AbrirVentanaCentros()));
    connect(ui->btnExaminar,SIGNAL(clicked()),this,SLOT(AbrirImagenNueva()));
    connect(btnGuardar,SIGNAL(clicked()),this,SLOT(Guardar()));
    connect(btnCancelar,SIGNAL(clicked()),this,SLOT(accept()));
    connect(ui->btnImprimir,SIGNAL(clicked()),this,SLOT(Imprimir()));

    DeshabilitarControles();
}

OperariosFicha::~OperariosFicha()
{
    delete Op;
    delete Centro;
    delete ui;
}

void OperariosFicha::changeEvent(QEvent *e)
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

void OperariosFicha::Imprimir()
{
    clsImprimirOperarios Inf;
    Inf.ImprimirFichaOp(Op);
}

void OperariosFicha::CargarDatos()
{
    if (Op->NIF() == "")
        ui->edtNIF->setReadOnly(false);
    else
    {
        ui->edtNIF->setReadOnly(true);

        Op->Cargar(Op->NIF());

        Centro->Cargar(Op->Centro());

        ui->edtNIF->setText(Op->NIF());
        ui->edtNombre->setText(Op->Nombre());
        ui->edtApellidos->setText(Op->Apellidos());
        ui->datFechaNacimiento->setDate(Op->FechaNacimiento());
        ui->edtDireccion->setText(Op->Direccion());
        ui->edtEmail->setText(Op->Email());
        ui->edtNCCC->setText(Op->CCC());
        ui->edtNomCentro->setText(Centro->Nombre());
    }

    QString rutaimgOp = Op->Imagen();
    if (rutaimgOp == "")
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
        if (file.exists(rutaimgOp))
        {
            clsImagen *clsImg = new clsImagen(rutaimgOp,ui->scAreaImagen->width(),ui->scAreaImagen->height(),ui->scAreaImagen);
            ui->scAreaImagen->setWidget(clsImg);
        }
    }
}

void OperariosFicha::AbrirVentanaCentros()
{
    CentrosLista *ventanaCentro = new CentrosLista();
    ventanaCentro->setWindowModality(Qt::ApplicationModal);
    ventanaCentro->show();
    if (ventanaCentro->exec() == QDialog::Accepted)
    {
        int CentroSelecc = ventanaCentro->CodigoCentroSeleccionado();
        if (CentroSelecc != Op->Centro())
        {
            Op->setCentro(CentroSelecc);
            Centro->Cargar(Op->Centro());
            ui->edtNomCentro->setText(Centro->Nombre());
        }
    }
}

void OperariosFicha::Guardar()
{
    if (ValidaNIF())
    {
        Op->setNIF(ui->edtNIF->text());
        Op->setApellidos(ui->edtApellidos->text());
        Op->setDireccion(ui->edtDireccion->text());
        Op->setEmail(ui->edtEmail->text());
        Op->setFechaNacimiento(ui->datFechaNacimiento->date());
        QString strCCC = ui->edtNCCC->text();
        Op->setCCC(strCCC.remove("-",Qt::CaseInsensitive));
        Op->setNombre(ui->edtNombre->text());

        if (!Op->Existe())
        {
            if(Op->Insertar())
                this->accept();
            else
                this->reject();
        }
        else
        {
            if(Op->Modificar())
                this->accept();
            else
                this->reject();
        }
    }
    else
    {
        QMessageBox::critical(this,QObject::tr("Error de introducción de datos"),QObject::tr("El NIF introducido es incorrecto."),0,0);
        this->reject();
    }
}

void OperariosFicha::AbrirImagenNueva()
{
    QString rutaAplicacion = qApp->applicationDirPath();
    QString nomArchivo = QFileDialog::getOpenFileName(this,QObject::tr("Abrir imagen"), rutaAplicacion+"/Imagenes", QObject::tr("Archivos de imagen (*.png *.jpg *.bmp)"));
    Op->setImagen(nomArchivo);
    clsImagen *clsImg = new clsImagen(nomArchivo,ui->scAreaImagen->width(),ui->scAreaImagen->height(),ui->scAreaImagen);
    ui->scAreaImagen->setWidget(clsImg);
}

void OperariosFicha::DeshabilitarControles()
{
    bool permiso = config->Permisos().value(OpMod);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(permiso);
    ui->btnExaminar->setEnabled(permiso);
    ui->btnCentro->setEnabled(permiso);
}

bool OperariosFicha::ValidaNIF()
{
    QString strLetra = "TRWAGMYFPDXBNJZSQVHLCKE";
    return ui->edtNIF->text().endsWith(strLetra[ui->edtNIF->text().left(8).toInt() % 23],Qt::CaseInsensitive);
}
