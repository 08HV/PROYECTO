#ifndef NIVEL2_H
#define NIVEL2_H

#include "nivel.h"
#include "gokunube.h"
#include <QGraphicsPixmapItem>

#include <QtCore/qglobal.h>

class Nivel2 : public Nivel
{
    Q_OBJECT
public:
    explicit Nivel2(Gokunube* gokuNube, QObject* parent = nullptr);
    ~Nivel2() override;

    void iniciarNivel() override;

private:
    QGraphicsPixmapItem* fondo = nullptr;
};

#endif // NIVEL2_H
