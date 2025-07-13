#ifndef NIVEL3_H
#define NIVEL3_H

#include <QtCore/qglobal.h>

#include "nivel.h"
#include "goku.h"
#include "maestro.h"
#include <QGraphicsPixmapItem>
#include <QTimer>

class Nivel3 : public Nivel
{
    Q_OBJECT
public:
    explicit Nivel3(Goku* goku, Maestro* maestro, QObject *parent = nullptr);
    ~Nivel3();

    void iniciarNivel() override;

    int getEnergiaGoku() const;
    int getEnergiaMaestro() const;

    void dañarGoku(int cantidad = 10);
    void dañarMaestro(int cantidad = 10);

    void terminarNivel(bool exito);

signals:
    void nivelCompletado();
    void nivelFallido();

private:
    Goku* goku;
    Maestro* maestro;
    QGraphicsPixmapItem* fondo;
    int energiaGoku;
    int energiaMaestro;
    bool nivelTerminado;
};

#endif // NIVEL3_H
