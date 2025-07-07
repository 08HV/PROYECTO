#ifndef GOKUNUBE_H
#define GOKUNUBE_H

#include <QtCore/qglobal.h>
#include "personaje.h"
#include <QPixmap>
#include <QTimer>

class Gokunube : public Personaje
{
private:
    QPixmap* flySprite;

public:
    Gokunube();
    ~Gokunube();

    void updateFisica() override;
    void splitSprite() override;
    void animation() override;
    void updateSprite() override;


    bool espacioPresionado = false;


    float velocidadY = 0;
    const float gravedad = 0.8f;
    const float fuerzaVuelo = 1.5f;
    const float velocidadX = 3.0f;
};

#endif // GOKUNUBE_H
