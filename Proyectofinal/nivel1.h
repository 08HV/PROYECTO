#ifndef NIVEL1_H
#define NIVEL1_H

#include "nivel.h"
#include "tortuga.h"
#include "goku.h"
#include <QGraphicsPixmapItem>
#include <QTimer>

#include <QtCore/qglobal.h>

class Nivel1 : public Nivel
{
    Q_OBJECT
public:

    explicit Nivel1(Goku* goku, QObject *parent = nullptr);
    ~Nivel1() override;

    void iniciarNivel() override;

protected:
    QTimer* timerTortugas;
    int frecuenciaTortugasMs;

protected slots:

    void slotCrearTortuga();

private:
    Goku* goku;
    QGraphicsPixmapItem* fondo;
    QSize viewSize;
};

#endif // NIVEL1_H
