#include "goku.h"
#include <resources.h>

Goku::Goku()
    : Personaje()
{
    runSprite = new QPixmap(resources::runGoku);
    idleSprite = new QPixmap(resources::stopGoku);
    jumpSprite = new QPixmap(resources::jumpGoku);
    attack1Sprite = new QPixmap(resources::attack1);
    attack2Sprite = new QPixmap(resources::attack2);
    kickSprite = new QPixmap(resources::attack3);
    caidoSprite = new QPixmap(resources::fallGoku);

    comboTimer = new QTimer(this);
    comboTimer->setSingleShot(true);
    connect(comboTimer, &QTimer::timeout, [this](){
        comboCounter = 0;
        acelerando = false;
    });
}

Goku::~Goku()
{
    delete runSprite;
    delete idleSprite;
    delete jumpSprite;
    delete attack1Sprite;
    delete attack2Sprite;
    delete kickSprite;
    delete caidoSprite;
    qDebug() << "Destructor Goku";
    if (comboTimer)    { comboTimer->stop(); delete comboTimer; comboTimer = nullptr; }
    qDebug() << "Destructor Goku";
}

void Goku::updateSprite()
{
    if (attack3 && kickSprite) {
        column += 40;
        if (column >= kickSprite->width()) column = 0;
    } else if (attack2 && attack2Sprite) {
        column += 40;
        if (column >= attack2Sprite->width()) column = 0;
    } else if (attack1 && attack1Sprite) {
        column += 40;
        if (column >= attack1Sprite->width()) column = 0;
    } if (column + 35 >= 209) {
        column = 0;
    } else {
        column += 35;
    }
}

void Goku::splitSprite()
{
    if (caido && caidoSprite) {
        *body = caidoSprite->copy(column, row, 43, 44);
    }
    else if (attack3 && kickSprite) {
        *body = kickSprite->copy(column, row, 39, 41);
    }
    else if (attack2 && attack2Sprite) {
        *body = attack2Sprite->copy(column, row, 55, 50);
    }
    else if (attack1 && attack1Sprite) {
        *body = attack1Sprite->copy(column, row, 40, 39);
    }
    else if ((izqui || der) && !jump && runSprite) {
        *body = runSprite->copy(column, row, 33, 40);
    }
    else if ((!izqui || !der) && !jump && idleSprite) {
        *body = idleSprite->copy(column, row, 34, 38);
    }
    else if (jump && jumpSprite) {
        *body = jumpSprite->copy(column, row, 32, 47);
    }
}

void Goku::animation()
{
    updateSprite();
    splitSprite();

    if (caido && caidoSprite) {
        setPixmap(body->scaled(scale * 43, scale * 43));
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
        setPixmap(body->scaled(scale * 35, scale * 44));
    }

}

void Goku::activarAceleracion() { acelerando = true; }

void Goku::desactivarAceleracion() { acelerando = false; }
