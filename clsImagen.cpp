#include "clsImagen.h"
#include <QPixmap>
#include <QPainter>
#include <QWheelEvent>
#include <QMessageBox>


clsImagen::clsImagen(const QString nomArch,int _W,int _H, QWidget *parent) : QWidget(parent)
{
    QPixmap *imagenTemp = new QPixmap(nomArch);
    flPorcentajeZoom = 1.0;
    intH = _H;
    intW = _W;
    pixImagen = new QPixmap(imagenTemp->scaled(_W,_H,Qt::IgnoreAspectRatio));
}

clsImagen::~clsImagen()
{
    delete pixImagen;
}

void clsImagen::setPorcentajeZoom(float PorZ)
{
    int w, h;
	
    if(PorZ != flPorcentajeZoom)
    {
        flPorcentajeZoom = PorZ;
        emit(CambioFactorZoom(flPorcentajeZoom));

        w = pixImagen->width()*flPorcentajeZoom;
        h = pixImagen->height()*flPorcentajeZoom;
        setMinimumSize(w, h);
	
        QWidget *p = dynamic_cast<QWidget*>(parent());
        if(p)
            resize(p->width(), p->height());

        repaint();
    }
}

void clsImagen::paintEvent(QPaintEvent *event)
{
        int desx, desy;
        bool borde = false;
	
        if(width() > pixImagen->width()*flPorcentajeZoom)
	{
                desx = (width()-pixImagen->width()*flPorcentajeZoom)/2;
                borde = true;
	}
	else
	{
                desx = 0;
	}
	
        if(height() > pixImagen->height()*flPorcentajeZoom)
	{
                desy = (height()-pixImagen->height()*flPorcentajeZoom)/2;
                borde = true;
	}
	else
	{
                desy = 0;
	}

        QPainter painter(this);
        painter.save();
        painter.translate(desx, desy);
        painter.scale(flPorcentajeZoom, flPorcentajeZoom);
        painter.drawPixmap(0, 0, *pixImagen);
        painter.restore();
        if(borde)
	{
                painter.setPen(Qt::black);
                painter.drawRect(desx-1, desy-1, pixImagen->width()*flPorcentajeZoom+1, pixImagen->height()*flPorcentajeZoom+1);
	}
}

void clsImagen::wheelEvent(QWheelEvent *event)
{
        float fFactor;

        fFactor = flPorcentajeZoom + 0.001*event->delta();
        if(fFactor < 32.0/pixImagen->width())
                fFactor = 32.0/pixImagen->width();

        setPorcentajeZoom(fFactor);
}
