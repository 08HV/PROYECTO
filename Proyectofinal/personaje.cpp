#include "personaje.h"
#include <resources.h>
#include <QGraphicsScene>

Personaje::Personaje() {
    body = new QPixmap();
    timer = new QTimer(this);
    attackTimer = new QTimer(this);
    caidoTimer = new QTimer(this);
    scale = 5;
    timer->start(120);
    connect(timer, SIGNAL(timeout()), this, SLOT(animation()));
    connect(attackTimer, SIGNAL(timeout()), this, SLOT(stopLattack()));
    connect(attackTimer, SIGNAL(timeout()), this, SLOT(stopRattack()));
    connect(attackTimer, SIGNAL(timeout()), this, SLOT(stopKickAttack()));
    connect(caidoTimer, SIGNAL(timeout()), this, SLOT(recuperarseDeCaida()));

    row = 0;
    column = 0;
    izqui = false;
    der = false;
    last = 'D';
    jump = false;
    attack1 = false;
    attack2 = false;
    attack3 = false;
    maxCont = 0;
    attacking = false;
    caido = false;
    impulsoX = 0.0f;

    kickSprite = nullptr;
    attack2Sprite = nullptr;
    caidoSprite = nullptr;
}

Personaje::~Personaje()
{
    if (timer) {
        timer->stop();
        delete timer;
        timer = nullptr;
    }

    if (attackTimer) {
        attackTimer->stop();
        delete attackTimer;
        attackTimer = nullptr;
    }
    if (caidoTimer) {
        caidoTimer->stop();
        delete caidoTimer;
        caidoTimer = nullptr;
    }
    delete body;
    body = nullptr;
    if (kickSprite) delete kickSprite;
    if (attack2Sprite) delete attack2Sprite;
    if (caidoSprite) delete caidoSprite;

}

void Personaje::setIzqui(bool newIzqui) { izqui = newIzqui; }
void Personaje::setDer(bool newDer) { der = newDer; }
char Personaje::getLast() const { return last; }
void Personaje::setLast(char newLast) { last = newLast; }
bool Personaje::getJump() const { return jump; }
void Personaje::setJump(bool newJump) { jump = newJump; }

void Personaje::updateSprite() {
    if (column + 35 >= 209) {
        column = 0;
    } else {
        column += 35;
    }
}

void Personaje::splitSprite() {

    if (attack3 && kickSprite) {
        *body = kickSprite->copy(column, row, 40, 39);
    } else if (attack2 && attack2Sprite) {
        *body = attack2Sprite->copy(column, row, 40, 39);
    } else if (attack1) {
        QPixmap bodyAux(resources::attack1);
        *body = bodyAux.copy(column, row, 40, 39);
    } else if (izqui || der) {
        QPixmap bodyAux(resources::runGoku);
        *body = bodyAux.copy(column, row, 33, 40);
    } else if (jump) {
        QPixmap bodyAux(resources::jumpGoku);
        *body = bodyAux.copy(column, row, 32, 47);
    } else {
        QPixmap bodyAux(resources::stopGoku);
        *body = bodyAux.copy(column, row, 34, 38);
    }
    if (caido && caidoSprite) {
        *body = caidoSprite->copy(0, 0, caidoSprite->width(), caidoSprite->height());
    }
}

void Personaje::animation() {
    updateSprite();
    splitSprite();

    if (caido && caidoSprite) {
        setPixmap(body->scaled(scale * 40, scale * 39));
    } else if ((!izqui && last == 'A') && !jump) {
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
    else if (attack1 || attack2 || attack3) {
        setPixmap(body->scaled(scale * 40, scale * 39));
    }
}

void Personaje::jumpG() {
    if (!enElAire && !caido) {
        qDebug() << "Inicia Salto";
        jump = true;
        enElAire=true;
        velocidadY = -30;
    }
}


void Personaje::leftAttack() {
    if (!attackTimer->isActive() && !caido) {
        qDebug() << "Ataque izquierdo";
        column = 0;
        attackTimer->start(200);
        attack1 = true;
        attacking = true;
        startAttack();
    }
}

void Personaje::rightAttack() {
    if (!attackTimer->isActive() && !caido) {
        qDebug() << "Ataque derecho";
        column = 0;
        attackTimer->start(200);
        attack2 = true;
        attacking = true;
        startAttack();
    }
}

void Personaje::kickAttack() {
    if (!attackTimer->isActive() && !caido) {
        column = 0;
        attackTimer->start(250);
        attack3 = true;
        attacking = true;
        startAttack();
    }
}

void Personaje::stopRattack() {
    qDebug() << "Finaliza izquierdo";
    attack1 = false;
    attacking = false;
    attackTimer->stop();
    endAttack();
}

void Personaje::stopLattack() {
    qDebug() << "Finaliza derecho";
    attack2 = false;
    attacking = false;
    attackTimer->stop();
    endAttack();
}
void Personaje::stopKickAttack() {
    attack3 = false;
    attacking = false;
    attackTimer->stop();
    endAttack();
}

bool Personaje::isAttacking() const {
    return attacking;
}

void Personaje::startAttack() {
    attacking = true;
}

void Personaje::endAttack() {
    attacking = false;
}

void Personaje::morir() {
    this->setVisible(false);
}

void Personaje::caer(float fuerzaImpulsoX, float fuerzaImpulsoY) {
    if (caido) return;
    caido = true;
    impulsoX = fuerzaImpulsoX;
    velocidadY = fuerzaImpulsoY;
    setPixmap(caidoSprite ? caidoSprite->scaled(scale * 40, scale * 39) : pixmap());
    caidoTimer->start(2000); // caido por 2 segundos
}

void Personaje::recuperarseDeCaida() {
    caido = false;
    impulsoX = 0.0f;
    velocidadY = 0.0f;
}

void Personaje::updateFisica() {
    if (caido) {
        moveBy(impulsoX, velocidadY);
        velocidadY += gravedad;
        if (pos().y() >= pisoY) {
            setPos(pos().x(), pisoY);
            velocidadY = 0;
        }
        return;
    }
    if (enElAire) {
        velocidadY += gravedad;
        moveBy(0, velocidadY);

        if (pos().y() >= pisoY) {
            setPos(pos().x(), pisoY);
            velocidadY = 0;
            enElAire = false;
            jump = false;
        }
    }

    if (scene()) {
        QRectF sceneRect = scene()->sceneRect();
        QPointF pos = this->pos();
        qreal ancho = this->boundingRect().width();
        if (pos.x() < sceneRect.left())
            setPos(sceneRect.left(), pos.y());
        else if (pos.x() + ancho > sceneRect.right())
            setPos(sceneRect.right() - ancho, pos.y());
    }
    return;
}

bool Personaje::timerState() {return false;}

