#ifndef GOKU_H
#define GOKU_H

#include "personaje.h"
#include <QPixmap>
#include <QTimer>

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

    float velocidadNormal = 5.0f;
    float velocidadAcelerada = 10.0f;
    bool acelerando = false;
    int comboCounter = 0;
    QTimer* comboTimer = nullptr;
    bool shiftPressed = false;

    void updateSprite() override;
    void splitSprite() override;
    void animation() override;

    void activarAceleracion();
    void desactivarAceleracion();

};

#endif // GOKU_H
