#ifndef NIVEL_H
#define NIVEL_H

#include <QtCore/qglobal.h>

#include <QObject>
#include <QGraphicsScene>
#include <QTimer>
#include <QList>
#include "obstaculo.h"


class Nivel : public QObject
{
    Q_OBJECT
public:
    explicit Nivel(QObject *parent = nullptr);
    virtual ~Nivel();


    virtual void iniciarNivel() = 0;
    virtual void agregarObstaculo(Obstaculo* obs);
    virtual void eliminarObstaculo(Obstaculo* obs);
    virtual void acelerarObstaculos(float incremento);

    QGraphicsScene* getEscena() const;
    QList<Obstaculo*> getObstaculos() const;


protected:
    QGraphicsScene* escena;
    QList<Obstaculo*> obstaculos;
    QTimer* timerAceleracion;

protected slots:

    virtual void slotAcelerarObstaculos();
    virtual void slotObstaculoEliminado(Obstaculo* obs);
};

#endif // NIVEL_H
