#ifndef MAESTRO_H
#define MAESTRO_H

#include <QtCore/qglobal.h>
#include "personaje.h"
#include <QTimer>
#include <QPixmap>
#include "goku.h"

class Maestro : public Personaje
{
    Q_OBJECT

public:
    Maestro();
    ~Maestro();

    void updateSprite() override;
    void splitSprite() override;
    void animation() override;
    void updateFisica() override;
    void updateAutonomo();
    void updateIA();

    bool isAttacking() const;
    void setGoku(Goku* gokuPtr);

private:
    QPixmap* runSprite;
    QPixmap* idleSprite;
    QPixmap* jumpSprite;
    QPixmap* attack1Sprite;


    QTimer* iaTimer;
    int direccion; // 1 = derecha, -1 = izquierda, 0 = quieto

    Goku* goku = nullptr;
    bool mirandoIzquierda = false;
};

#endif // MAESTRO_H
