#include "tortuga.h"
#include <QGraphicsScene>
#include <QPixmap>
#include <resources.h>

Tortuga::Tortuga(float velocidadInicial, QGraphicsItem *parent)
    : Obstaculo(parent)
{
    velocidad = velocidadInicial;
    velocidadMaxima = 10.0f;


    QPixmap pm(resources::obstacle1);
    qDebug() << "Creando tortuga, pixmap válida:" << !pm.isNull() << "Ruta:" << resources::obstacle1;
    setPixmap(pm);



    connect(timer, &QTimer::timeout, this, &Tortuga::onTimeout);
    timer->start(30);
}

Tortuga::~Tortuga()
{
    timer->stop();
}

void Tortuga::mover()
{

    setX(x() - velocidad);

    if (x() + boundingRect().width() < 0) {
        destruir();
    }
}

void Tortuga::destruir()
{
    emit obstaculoEliminado(this);
    deleteLater();
}

void Tortuga::onTimeout()
{
    mover();
}
