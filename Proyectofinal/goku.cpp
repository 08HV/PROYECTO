#include "goku.h"
#include <resources.h>

Goku::Goku()
    : Personaje()
{
    runSprite = new QPixmap(resources::runGoku);
    idleSprite = new QPixmap(resources::stopGoku);
    jumpSprite = new QPixmap(resources::jumpGoku);
    attack1Sprite = new QPixmap(resources::attack1);
    // attack2Sprite = new QPixmap(resources::attack2);
}

Goku::~Goku()
{
    delete runSprite;
    delete idleSprite;
    delete jumpSprite;
    delete attack1Sprite;
    // delete attack2Sprite;
}

void Goku::updateSprite()
{
    if (column + 35 >= 209) {
        column = 0;
    } else {
        column += 35;
    }
}

void Goku::splitSprite()
{
    if ((izqui || der) && !jump) {
        *body = runSprite->copy(column, row, 33, 40);
    }
    else if ((!izqui || !der) && !jump) {
        *body = idleSprite->copy(column, row, 34, 38);
    }
    else if (jump) {
        *body = jumpSprite->copy(column, row, 32, 47);
    }

    if (attack1) {
        *body = attack1Sprite->copy(column, row, 40, 39);
    }
    //if (attack2) { *body = attack2Sprite->copy(column, row, 40, 39); }
}

void Goku::animation()
{
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
