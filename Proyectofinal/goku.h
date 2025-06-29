#ifndef GOKU_H
#define GOKU_H

#include "personaje.h"
#include <QPixmap>

class Goku : public Personaje
{
    Q_OBJECT

private:
    QPixmap* runSprite;
    QPixmap* idleSprite;
    QPixmap* jumpSprite;
    QPixmap* attack1Sprite;
    // QPixmap* attack2Sprite;

public:
    Goku();
    ~Goku();

    void updateSprite() override;
    void splitSprite() override;
    void animation() override;
};

#endif // GOKU_H
