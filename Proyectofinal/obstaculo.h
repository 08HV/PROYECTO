#ifndef OBSTACULO_H
#define OBSTACULO_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QTimer>


class Obstaculo : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit Obstaculo(QGraphicsItem *parent = nullptr);
    virtual ~Obstaculo();


    virtual void mover() = 0;
    virtual void aumentarVelocidad(float incremento);

    float getVelocidad() const;


    void setVelocidad(float v);

signals:
    void obstaculoEliminado(Obstaculo*);

protected:
    float velocidad;
    float velocidadMaxima;
    QTimer *timer;
};

#endif // OBSTACULO_H
