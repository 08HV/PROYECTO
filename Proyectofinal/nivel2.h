#ifndef NIVEL2_H
#define NIVEL2_H

#include "nivel.h"
#include "gokunube.h"
#include <QGraphicsPixmapItem>
#include "fuego.h"
#include "obstaculoestatico.h"
#include "ave.h"
#include <QTimer>
#include <QList>
#include <QPointer>

#include <QtCore/qglobal.h>

class Nivel2 : public Nivel
{
    Q_OBJECT
public:
    explicit Nivel2(Gokunube* gokuNube, QObject* parent = nullptr);
    ~Nivel2() override;

    void iniciarNivel() override;
    void terminarNivel(bool exito);
    void eliminarTodasLasAves();


signals:
    void nivelCompletado();
    void nivelFallido();

protected slots:
    void slotCrearAve();
    void slotComprobarColisiones();

private:
    QGraphicsPixmapItem* fondo = nullptr;
    Gokunube* gokunube = nullptr;
    QSize viewSize;

    Fuego* fuego = nullptr;
    QList<ObstaculoEstatico*> obstaculosEstaticos;
    QList<QPointer<Ave>> aves;

    QTimer* timerAves = nullptr;
    QTimer* timerColisiones = nullptr;
    bool nivelTerminado = false;
    bool enDestruccion = false;

    void removerAvesFueraPantalla();
    int ultimaAlturaAve = -1;
};

#endif // NIVEL2_H
