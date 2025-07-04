#include "nivel1.h"
#include <QRandomGenerator>
#include <QGraphicsScene>
#include <resources.h>
#include <QPixmap>
#include <QDebug>

Nivel1::Nivel1(Goku* goku,QObject *parent)
    :Nivel(parent), goku(goku)
{
    timerTortugas = new QTimer(this);
    frecuenciaTortugasMs = 3000;
    connect(timerTortugas, &QTimer::timeout, this, &Nivel1::slotCrearTortuga);

    QPixmap fondoPixmap(resources::nivel1);
    fondo = new QGraphicsPixmapItem(fondoPixmap);
    fondo->setZValue(-100);
    fondo->setPos(0, 0);
    escena->addItem(fondo);

    escena->setSceneRect(0, 0, fondoPixmap.width(), fondoPixmap.height());

    timerColeccionable = new QTimer(this);
    connect(timerColeccionable, &QTimer::timeout, this, &Nivel1::slotTiempoColeccionable);

    connect(this, &Nivel::tiempoFinalizadoNivel, this, &Nivel1::slotTiempoNivelFinalizado);

}



Nivel1::~Nivel1()
{
    timerTortugas->stop();
    timerColeccionable->stop();
    qDebug() << "Destructor Nivel1";
    if (coleccionable) {
        escena->removeItem(coleccionable);
        delete coleccionable;
        coleccionable = nullptr;
    }

}

void Nivel1::iniciarNivel()
{

    for (Obstaculo* obs : obstaculos) {
        escena->removeItem(obs);
        obs->deleteLater();
    }
    obstaculos.clear();


    timerTortugas->start(frecuenciaTortugasMs);
    timerAceleracion->start(4000);


    slotCrearTortuga();
    objetosRecogidos = 0;
    nivelTerminado = false;
    iniciarColeccionables(cantidadObjetivo);
}

void Nivel1::slotCrearTortuga()
{
    int pisoY = fondo->pixmap().height();
    int anchoPantalla = fondo->pixmap().width();
    Tortuga* t = new Tortuga(9.0f);

    int posY = pisoY - t->boundingRect().height();
    t->setPos(anchoPantalla, posY);

    qDebug() << "Creando tortuga en x=" << anchoPantalla << ", y=" << posY;
    agregarObstaculo(t);
}

void Nivel1::iniciarColeccionables(int cantidad)
{
    cantidadObjetivo = cantidad;
    objetosRecogidos = 0;
    slotCrearColeccionable();
    iniciarTemporizadorNivel(120); // 2 minutos
}

void Nivel1::slotCrearColeccionable()
{
    if (coleccionable) {
        escena->removeItem(coleccionable);
        delete coleccionable;
        coleccionable = nullptr;
    }
    colocarColeccionableAleatorio();
    timerColeccionable->start(3000); // 3 segundos la esfera
}

void Nivel1::colocarColeccionableAleatorio()
{
    int margen = 80;
    int w = escena->width() - margen;
    int h = escena->height() - margen;
    int x = QRandomGenerator::global()->bounded(margen, w);
    int y = QRandomGenerator::global()->bounded(margen, h);

    coleccionable = new QGraphicsPixmapItem(QPixmap(resources::esfera).scaled(60,60 ));
    coleccionable->setZValue(10);
    coleccionable->setPos(x, y);
    escena->addItem(coleccionable);
}

void Nivel1::slotTiempoColeccionable()
{
    slotCrearColeccionable();
}

void Nivel1::recogerColeccionable()
{
    if (nivelTerminado) return;
    objetosRecogidos++;
    if (coleccionable) {
        escena->removeItem(coleccionable);
        delete coleccionable;
        coleccionable = nullptr;
        timerColeccionable->stop();
    }

    if (objetosRecogidos >= cantidadObjetivo) {
        terminarNivel(true);
    } else {
        slotCrearColeccionable();
    }
}

void Nivel1::terminarNivel(bool exito)
{
    nivelTerminado = true;
    detenerTemporizadorNivel();
    timerColeccionable->stop();
    if (coleccionable) {
        escena->removeItem(coleccionable);
        delete coleccionable;
        coleccionable = nullptr;
    }
    if (exito) emit nivelCompletado();
    else emit nivelFallido();
}

void Nivel1::slotTiempoNivelFinalizado()
{
    if (!nivelTerminado)
        terminarNivel(false);
}

