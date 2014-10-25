#include <QPushButton>
#include "partestrabajodet.h"
#include "ui_partestrabajodet.h"
#include "operarioslista.h"
#include "clsconfiguracion.h"

extern clsConfiguracion *config;

PartesTrabajoDet::PartesTrabajoDet(int _NoParte,QString _Op,tHora _TipoHora,double _Horas,QString _Proyecto,QString _Tarea,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PartesTrabajoDet)
{
    ui->setupUi(this);

    intParte = _NoParte;
    decHoras = _Horas;
    strProyecto = _Proyecto;
    strTarea = _Tarea;
    strNIF = _Op;
    eTipoHora = _TipoHora;

    ui->cbTipoHora->addItem(QObject::tr("Normal"));
    ui->cbTipoHora->addItem(QObject::tr("Extra"));
    ui->cbTipoHora->setCurrentIndex(0);
    ui->edtOperario->setReadOnly(true);
    ui->edtIDParte->setReadOnly(true);

    if (_Op != "")
        Operario = new clsOperarios(_Op);

    ParteDetalle = new clsPartes_Trabajo_Det();

    CargarDatos();

    QPushButton *btnGuardar = ui->buttonBox->button(QDialogButtonBox::Ok);
    QPushButton *btnCancelar = ui->buttonBox->button(QDialogButtonBox::Cancel);
    connect(btnGuardar,SIGNAL(clicked()),this,SLOT(InsertarLinea()));
    connect(btnCancelar,SIGNAL(clicked()),this,SLOT(accept()));
    connect(ui->btnOperario,SIGNAL(clicked()),this,SLOT(AbrirVentanaOperarios()));
}

PartesTrabajoDet::~PartesTrabajoDet()
{
    delete ParteDetalle;
    delete Operario;
    delete ui;
}

void PartesTrabajoDet::changeEvent(QEvent *e)
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

void PartesTrabajoDet::CargarDatos()
{
    ui->edtIDParte->setText(QString::number(intParte));
    if (strNIF != "")
    {
        ParteDetalle->Cargar(intParte,strNIF,eTipoHora,strProyecto,strTarea);

        Operario->Cargar(strNIF);

        ui->edtTarea->setText(strTarea);
        ui->edtProyecto->setText(strProyecto);
        ui->edtOperario->setText(Operario->Nombre() + " " + Operario->Apellidos());
        ui->dspHoras->setValue(ParteDetalle->Horas());
        ui->cbTipoHora->setCurrentIndex(ParteDetalle->TipoHora());

        ui->btnOperario->setEnabled(false);
        ui->edtProyecto->setReadOnly(true);
        ui->edtTarea->setReadOnly(true);
        ui->cbTipoHora->setEnabled(false);
    }
    else
    {
        ui->edtTarea->setText("");
        ui->edtProyecto->setText("");
        ui->edtOperario->setText("");
        ui->dspHoras->setValue(0);
        ui->cbTipoHora->setCurrentIndex(0);
    }
}

void PartesTrabajoDet::InsertarLinea()
{
    ParteDetalle->setNoParte(intParte);
    ParteDetalle->setHoras(ui->dspHoras->value());
    ParteDetalle->setOperario(Operario->NIF());
    ParteDetalle->setProyecto(ui->edtProyecto->text());
    ParteDetalle->setTarea(ui->edtTarea->text());
    tHora eHora;
    if (ui->cbTipoHora->currentIndex() == 0)
        eHora = Normal;
    else if(ui->cbTipoHora->currentIndex() == 1)
        eHora = Extra;
    else
        eHora = Normal;
    ParteDetalle->setTipoHora(eHora);

    if (ParteDetalle->Existe())
    {
        if(ParteDetalle->Modificar())
            this->accept();
        else
            this->reject();
    }
    else
    {
        if(ParteDetalle->Insertar())
            this->accept();
        else
            this->reject();
    }
}

void PartesTrabajoDet::AbrirVentanaOperarios()
{
    OperariosLista *ventanaOperario = new OperariosLista();
    ventanaOperario->setWindowModality(Qt::ApplicationModal);
    ventanaOperario->show();
    if (ventanaOperario->exec() == QDialog::Accepted)
    {
        Operario = new clsOperarios(ventanaOperario->NIFOpSeleccionado());
        Operario->Cargar(ventanaOperario->NIFOpSeleccionado());
        ui->edtOperario->setText(Operario->Nombre() + " " + Operario->Apellidos());
    }
}
