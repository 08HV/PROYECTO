#ifndef NIVEL1_H
#define NIVEL1_H

#include "nivel.h"
#include "tortuga.h"
#include "goku.h"
#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QSize>

#include <QtCore/qglobal.h>

class Nivel1 : public Nivel
{
    Q_OBJECT
public:

    explicit Nivel1(Goku* goku, QObject *parent = nullptr);
    ~Nivel1() override;

    void iniciarNivel() override;

    void iniciarColeccionables(int cantidad);
    QGraphicsPixmapItem* getColeccionable() const { return coleccionable; }
    int getObjetosRecogidos() const { return objetosRecogidos; }
    int getCantidadObjetivo() const { return cantidadObjetivo; }
    void recogerColeccionable();

    void setCantidadObjetivo(int cantidad) { cantidadObjetivo = cantidad; }
    void terminarNivel(bool exito);

signals:
    void nivelCompletado();
    void nivelFallido();

protected:
    QTimer* timerTortugas;
    int frecuenciaTortugasMs;

protected slots:

    void slotCrearTortuga();

    void slotCrearColeccionable();
    void slotTiempoColeccionable();
    void slotTiempoNivelFinalizado();


private:
    Goku* goku;
    QGraphicsPixmapItem* fondo;
    QSize viewSize;

    QGraphicsPixmapItem* coleccionable = nullptr;
    QTimer* timerColeccionable = nullptr;
    int cantidadObjetivo = 12;
    int objetosRecogidos = 0;
    void colocarColeccionableAleatorio();
    bool nivelTerminado = false;


};

#endif // NIVEL1_H
