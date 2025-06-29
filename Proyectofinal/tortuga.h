#ifndef TORTUGA_H
#define TORTUGA_H


#include <QtCore/qglobal.h>
#include "obstaculo.h"

class Tortuga : public Obstaculo
{
    Q_OBJECT
public:
    explicit Tortuga(float velocidadInicial = 3.0f, QGraphicsItem *parent = nullptr);
    ~Tortuga();

    void mover() override;
    void destruir();

private slots:
    void onTimeout();
};

#endif // TORTUGA_H
