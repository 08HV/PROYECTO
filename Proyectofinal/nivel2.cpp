#include "nivel2.h"
#include <resources.h>
#include <QPixmap>
#include <QRandomGenerator>
#include <QDebug>

Nivel2::Nivel2(Gokunube* gokuNube, QObject* parent)
    : Nivel(parent), gokunube(gokuNube)

{
    qDebug() << "Constructor Nivel2";
    QPixmap fondoPixmap(resources::nivel2);
    fondo = new QGraphicsPixmapItem(fondoPixmap);
    fondo->setZValue(-100);
    fondo->setPos(0, 0);
    escena->addItem(fondo);
    escena->setSceneRect(0, 0, fondoPixmap.width(), fondoPixmap.height());


    int anchoFondo = fondo->pixmap().width();
    int anchoFuego = QPixmap(resources::Fuego).width() / 2; // Asumiendo 2 frames horizontales
    int cantidadFuegos = (anchoFondo + anchoFuego - 1) / anchoFuego;

    for (int i = 0; i < cantidadFuegos; ++i) {
        Fuego* f = new Fuego();
        f->setPos(i * anchoFuego, escena->height() - f->pixmap().height());
        f->setZValue(50);
        escena->addItem(f);
        obstaculos.append(f); // Si quieres poder borrarlos después
    }

    ObstaculoEstatico* columna1 = new ObstaculoEstatico(resources::estatico, 46, 60, 8,1.5f);
    columna1->setPos(500, 50);
    escena->addItem(columna1);
    obstaculosEstaticos.append(columna1);

    ObstaculoEstatico* columna4 = new ObstaculoEstatico(resources::estatico, 46, 60, 8,1.5f);
    columna4->setPos(500, 350);
    escena->addItem(columna4);
    obstaculosEstaticos.append(columna4);

    ObstaculoEstatico* columna2 = new ObstaculoEstatico(resources::estatico, 46, 60, 8,1.5f);
    columna2->setPos(1150, 300);
    escena->addItem(columna2);
    obstaculosEstaticos.append(columna2);

    ObstaculoEstatico* columna3 = new ObstaculoEstatico(resources::estatico, 46, 60, 8,1.5f);
    columna3->setPos(1600, 500);
    escena->addItem(columna3);
    obstaculosEstaticos.append(columna3);

    ObstaculoEstatico* columna5 = new ObstaculoEstatico(resources::estatico, 46, 60, 8,1.5f);
    columna5->setPos(1600, 100);
    escena->addItem(columna5);
    obstaculosEstaticos.append(columna5);

    ObstaculoEstatico* columna6 = new ObstaculoEstatico(resources::estatico, 46, 60, 8,1.5f);
    columna6->setPos(2000, 300);
    escena->addItem(columna6);
    obstaculosEstaticos.append(columna6);

    ObstaculoEstatico* columna7 = new ObstaculoEstatico(resources::estatico, 46, 60, 8,1.5f);
    columna7->setPos(1900, 40);
    escena->addItem(columna7);
    obstaculosEstaticos.append(columna7);



    // Timer para aves
    timerAves = new QTimer(this);
    connect(timerAves, &QTimer::timeout, this, &Nivel2::slotCrearAve);

    // Timer para colisiones
    timerColisiones = new QTimer(this);
    connect(timerColisiones, &QTimer::timeout, this, &Nivel2::slotComprobarColisiones);
    timerColisiones->start(16); // ~60 FPS

    qDebug() << "Nivel2 creado correctamente";

}

Nivel2::~Nivel2()
{
    qDebug() << "Destructor Nivel2";
    if (timerAves) timerAves->stop();
    if (timerColisiones) timerColisiones->stop();

    qDeleteAll(aves);
    aves.clear();

    if (fuego) { escena->removeItem(fuego); delete fuego; fuego = nullptr; }

    qDeleteAll(obstaculosEstaticos);
    obstaculosEstaticos.clear();

    if (fondo) {
        escena->removeItem(fondo);
        delete fondo;
        fondo = nullptr;
    }
}

void Nivel2::iniciarNivel()
{
    nivelTerminado = false;
    timerAves->start(1500);
}


void Nivel2::slotCrearAve()
{
    if (!escena) return;
    QList<int> alturas = {60, 180, 350, 430};

    int i;
    do {
        i = QRandomGenerator::global()->bounded(alturas.size());
    } while (alturas[i] == ultimaAlturaAve && alturas.size() > 1);

    int y = alturas[i];
    ultimaAlturaAve = y; // Recuerda la última altura usada

    float velocidadAve = 50.0f; // Ajusta a lo que prefieras (no pongas 70)
    Ave* ave = new Ave(velocidadAve);
    ave->setPos(escena->width(), y);
    aves.append(ave);
    escena->addItem(ave);
}

void Nivel2::slotComprobarColisiones()
{
    if (!gokunube || nivelTerminado) return;

    int alturaLimite = 81;

    // Zona de muerte por altura Y
    if (gokunube->y() + gokunube->boundingRect().height() > escena->height() - alturaLimite) {
        nivelTerminado = true;
        gokunube->morir();
        terminarNivel(false);
        return;
    }
    // Obstáculos estáticos
    for (auto est : obstaculosEstaticos) {
        if (gokunube->collidesWithItem(est)) {
            nivelTerminado = true;
            gokunube->morir();
            terminarNivel(false);
            return;
        }
    }
    // Aves
    for (auto ave : aves) {
        if (gokunube->collidesWithItem(ave)) {
            nivelTerminado = true;
            gokunube->morir();
            terminarNivel(false);
            return;
        }
    }
    // Llegó al final del fondo
    if (gokunube->pos().x() + gokunube->boundingRect().width() >= escena->width()) {
        nivelTerminado = true;;
        terminarNivel(true);
    }

    removerAvesFueraPantalla();
}

void Nivel2::terminarNivel(bool exito) {
    if (timerAves) timerAves->stop();
    if (timerColisiones) timerColisiones->stop();
    if (exito)
        emit nivelCompletado();
    else
        emit nivelFallido();
    // detener timerrr
}

void Nivel2::removerAvesFueraPantalla()
{
    for (int i = aves.size() - 1; i >= 0; --i) {
        Ave* ave = aves.at(i);
        if (ave->x() + ave->boundingRect().width() < 0) {
            escena->removeItem(ave);
            aves.removeAt(i);
            delete ave;
        }
    }
}
