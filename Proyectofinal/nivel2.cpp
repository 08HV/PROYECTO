#include "nivel2.h"
#include <resources.h>
#include <QPixmap>

Nivel2::Nivel2(QObject *parent)
    : Nivel(parent)
{
    qDebug() << "Constructor Nivel2";
    QPixmap fondoPixmap(resources::nivel2);
    fondo = new QGraphicsPixmapItem(fondoPixmap);
    fondo->setZValue(-100);
    fondo->setPos(0, 0);
    escena->addItem(fondo);
    escena->setSceneRect(0, 0, fondoPixmap.width(), fondoPixmap.height());
    qDebug() << "Nivel2 creado correctamente";
}

Nivel2::~Nivel2()
{
    qDebug() << "Destructor Nivel2";
    if (fondo) {
        escena->removeItem(fondo);
        delete fondo;
        fondo = nullptr;
    }


}

void Nivel2::iniciarNivel() {}
