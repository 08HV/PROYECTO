#include "personaje.h"
#include <resources.h>

Personaje::Personaje() {
    body = new QPixmap();
    timer = new QTimer(this);
    jumptimer = new QTimer(this);
    attackTimer = new QTimer(this);
    scale = 5;
    timer->start(120);
    connect(timer, SIGNAL(timeout()), this, SLOT(animation()));
    connect(jumptimer, SIGNAL(timeout()), this, SLOT(stopJump()));
    connect(attackTimer, SIGNAL(timeout()), this, SLOT(stopLattack()));
    connect(attackTimer, SIGNAL(timeout()), this, SLOT(stopRattack()));

    row = 0;
    column = 0;
    izqui = false;
    der = false;
    last = 'D';
    jump = false;
    attack1 = false;
    attack2 = false;
    maxCont = 0;
}

Personaje::~Personaje()
{
    delete timer;
    delete body;
    delete jumptimer;
    delete attackTimer;
}

void Personaje::setIzqui(bool newIzqui) { izqui = newIzqui; }
void Personaje::setDer(bool newDer) { der = newDer; }
char Personaje::getLast() const { return last; }
void Personaje::setLast(char newLast) { last = newLast; }
bool Personaje::getJump() const { return jump; }
void Personaje::setJump(bool newJump) { jump = newJump; }
bool Personaje::timerState() { return jumptimer->isActive(); }

void Personaje::updateSprite() {
    if (column + 35 >= 209) {
        column = 0;
    } else {
        column += 35;
    }
}

void Personaje::splitSprite() {
    if ((izqui || der) && !jump) {
        QPixmap bodyAux(resources::runGoku);
        *body = bodyAux.copy(column, row, 33, 40);
    }
    else if ((!izqui || !der) && !jump) {
        QPixmap bodyAux(resources::stopGoku);
        *body = bodyAux.copy(column, row, 34, 38);
    }
    else if (jump) {
        QPixmap bodyAux(resources::jumpGoku);
        *body = bodyAux.copy(column, row, 32, 47);
    }

    if (attack1) {
        QPixmap bodyAux(resources::attack1);
        *body = bodyAux.copy(column, row, 40, 39);
    }
}

void Personaje::animation() {
    updateSprite();
    splitSprite();

    if ((!izqui && last == 'A') && !jump) {
        setPixmap((body->scaled(scale * 35, scale * 40)).transformed(QTransform().scale(-1, 1)));
    }
    else if ((!der && last == 'D') && !jump) {
        setPixmap(body->scaled(scale * 35, scale * 40));
    }
    else if (izqui && !jump) {
        setPixmap((body->scaled(scale * 35, scale * 38)).transformed(QTransform().scale(-1, 1)));
    }
    else if (der && !jump) {
        setPixmap(body->scaled(scale * 35, scale * 38));
    }
    else if (!izqui && jump) {
        setPixmap((body->scaled(scale * 35, scale * 47)).transformed(QTransform().scale(1, 1)));
    }
    else if (!der && jump) {
        setPixmap((body->scaled(scale * 35, scale * 47)).transformed(QTransform().scale(-1, 1)));
    }
    else if (attack1) {
        setPixmap(body->scaled(scale * 40, scale * 39));
    }
}

void Personaje::jumpG() {
    if (!jumptimer->isActive()) {
        qDebug() << "Inicia Salto";
        jump = true;
        moveBy(0, -120);
        jumptimer->start(500);
    }
}

void Personaje::stopJump() {
    qDebug() << "Finaliza Salto";
    jump = false;
    jumptimer->stop();
    moveBy(0, 120);
}

void Personaje::leftAttack() {
    if (!attackTimer->isActive()) {
        qDebug() << "Ataque izquierdo";
        column = 0;
        attackTimer->start(200);
        attack1 = true;
    }
}

void Personaje::rightAttack() {
    if (!attackTimer->isActive()) {
        qDebug() << "Ataque derecho";
        column = 0;
        attackTimer->start(200);
        attack2 = true;
    }
}

void Personaje::stopRattack() {
    qDebug() << "Finaliza izquierdo";
    attackTimer->stop();
    attack1 = false;
}

void Personaje::stopLattack() {
    qDebug() << "Finaliza derecho";
    attackTimer->stop();
    attack2 = false;
}
