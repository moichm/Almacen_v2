#include "certificaciones.h"
#include "ui_certificaciones.h"
#include "producto.h"
#include <QCalendarWidget>
#include "clsconfiguracion.h"

extern clsConfiguracion *config;

Certificaciones::Certificaciones(QString codigo,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Certificaciones)
{
    ui->setupUi(this);

    QCalendarWidget *FechaCertif = new QCalendarWidget();
    FechaCertif->setFirstDayOfWeek(Qt::Monday);
    ui->dedtFechaCertificacion->setCalendarWidget(FechaCertif);
    ui->dedtFechaCertificacion->setDate(QDate::currentDate());

    ui->edtProducto->setText(codigo);

    connect(ui->buttonBox,SIGNAL(accepted()),this,SLOT(Certificar()));
    connect(ui->buttonBox,SIGNAL(rejected()),this,SLOT(close()));
}

Certificaciones::~Certificaciones()
{
    delete ui;
}

void Certificaciones::changeEvent(QEvent *e)
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

void Certificaciones::Certificar()
{
    clsProductos Prod;
    Prod.Cargar(ui->edtProducto->text());
    Prod.Certificar(ui->dedtFechaCertificacion->date(),ui->edtEmisor->text());
}
