#include "nivel3.h"
#include <resources.h>
#include <QGraphicsScene>
#include <QPixmap>
#include <QDebug>

Nivel3::Nivel3(Goku* goku, Maestro* maestro, QObject *parent)
    : Nivel(parent), goku(goku), maestro(maestro)
{
    QPixmap fondoPixmap(resources::nivel3);
    fondo = new QGraphicsPixmapItem(fondoPixmap);
    fondo->setZValue(-100);
    fondo->setPos(0, 0);
    escena->addItem(fondo);

    escena->setSceneRect(0, 0, fondoPixmap.width(), fondoPixmap.height());

    energiaGoku = 100;
    energiaMaestro = 100;
    nivelTerminado = false;
}

Nivel3::~Nivel3()
{
    if (fondo) {
        escena->removeItem(fondo);
        delete fondo;
        fondo = nullptr;
    }
}

void Nivel3::iniciarNivel()
{
    nivelTerminado = false;
    energiaGoku = 100;
    energiaMaestro = 100;
}

int Nivel3::getEnergiaGoku() const { return energiaGoku; }
int Nivel3::getEnergiaMaestro() const { return energiaMaestro; }

void Nivel3::dañarGoku(int cantidad)
{
    if (nivelTerminado) return;
    energiaGoku -= cantidad;
    if (energiaGoku <= 0) {
        energiaGoku = 0;
        terminarNivel(false);
    }
}

void Nivel3::dañarMaestro(int cantidad)
{
    if (nivelTerminado) return;
    energiaMaestro -= cantidad;
    if (energiaMaestro <= 0) {
        energiaMaestro = 0;
        terminarNivel(true);
    }
}

void Nivel3::terminarNivel(bool exito)
{
    nivelTerminado = true;
    if (exito) emit nivelCompletado();
    else emit nivelFallido();
}
