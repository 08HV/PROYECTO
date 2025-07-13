#include "maestro.h"
#include <resources.h>
#include <QRandomGenerator>
#include <QDebug>
#include <QGraphicsScene>


Maestro::Maestro()
    : Personaje()
{
    runSprite    = new QPixmap(resources::runMaestro);
    idleSprite   = new QPixmap(resources::stopMaestro);
    jumpSprite   = new QPixmap(resources::jumpMaestro);
    attack1Sprite= new QPixmap(resources::attackMaestro);

    iaTimer = new QTimer(this);
    iaTimer->start(900); //
    direccion = 0;
    scale = 3;

    connect(iaTimer, &QTimer::timeout, [this]() {
        this->updateIA();
    });
}

Maestro::~Maestro()
{
    delete runSprite;
    delete idleSprite;
    delete jumpSprite;
    delete attack1Sprite;
    if (iaTimer) {
        iaTimer->stop();
        delete iaTimer;
    }
}

void Maestro::setGoku(Goku* gokuPtr) {
    goku = gokuPtr;
}

void Maestro::updateSprite()
{
    if (attack1) {
        column = (column + 1) % 3; // 3 frames
    } else if (jump) {
        column = (column + 1) % 5; // 5 frames
    } else if (izqui || der) {
        column = (column + 1) % 3; // 3 frames
    } else {
        column = (column + 1) % 10; // 10 frames
    }
}

void Maestro::splitSprite()
{
    if (attack1) {
        *body = attack1Sprite->copy(column * 119, 0, 85, 101);
        qDebug() << "Attack coords:" << column * 110 << ", 0";
    }
    else if (jump) {
        *body = jumpSprite->copy(column * 96, 0, 96, 106);
    }
    else if (izqui || der) {
        *body = runSprite->copy(column * 105, 0, 90, 101);
        qDebug() << "Run coords:" << column * 96 << ", 0";
    }
    else {
        *body = idleSprite->copy(column * 95, 0, 82, 101);
    }
}


void Maestro::animation()
{
    updateSprite();
    splitSprite();

    if ((!izqui && last == 'A') && !jump) {
        setPixmap((body->scaled(scale * 98, scale * 101)).transformed(QTransform().scale(-1, 1)));
    }
    else if ((!der && last == 'D') && !jump) {
        setPixmap(body->scaled(scale * 98, scale * 101));
    }
    else if (izqui && !jump) {
        setPixmap((body->scaled(scale * 98, scale * 101)).transformed(QTransform().scale(-1, 1)));
    }
    else if (der && !jump) {
        setPixmap(body->scaled(scale * 97, scale * 101));
    }
    else if (!izqui && jump) {
        setPixmap((body->scaled(scale * 98, scale * 106)).transformed(QTransform().scale(1, 1)));
    }
    else if (!der && jump) {
        setPixmap((body->scaled(scale * 98, scale * 106)).transformed(QTransform().scale(-1, 1)));
    }
    else if (attack1) {
        setPixmap(body->scaled(scale * 115, scale * 101));
    }

}

void Maestro::updateIA()
{
    if (!goku) return;


    if (goku->x() < x()) {
        mirandoIzquierda = true;
    } else {
        mirandoIzquierda = false;
    }


    float distancia = std::abs(goku->x() - x());


    if (!jump && distancia < 90) {
        if (!attack1) {
            leftAttack();
        }
        direccion = 0;
        return;
    }


    int mover = QRandomGenerator::global()->bounded(3);
    if (!jump && mover == 0 && distancia > 100) {
        direccion = (goku->x() < x()) ? -1 : 1;
    } else {
        direccion = 0; // quieto
    }


    if (!jump && distancia > 120) {
        int saltar = QRandomGenerator::global()->bounded(6);
        if (saltar == 0) jumpG();
    }
}


void Maestro::updateAutonomo()
{
    float v = 2.0f;
    QRectF sceneRect = scene()->sceneRect();
    QRectF maestroRect = boundingRect().translated(pos());

    if (direccion == -1) {
        if (maestroRect.left() - v >= sceneRect.left()) {
            setIzqui(true);
            setDer(false);
            setLast('A');
            moveBy(-v, 0);
        }
    } else if (direccion == 1) {
        if (maestroRect.right() + v <= sceneRect.right()) {
            setDer(true);
            setIzqui(false);
            setLast('D');
            moveBy(v, 0);
        }
    } else {
        setIzqui(false);
        setDer(false);
    }
}

bool Maestro::isAttacking() const {
    return attack1;
}

void Maestro::updateFisica()
{
    Personaje::updateFisica();
}
