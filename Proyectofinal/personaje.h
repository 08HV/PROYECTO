#ifndef PERSONAJE_H
#define PERSONAJE_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QTimer>

class Personaje : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

protected:
    int row, column;
    float scale;
    QTimer* timer;
    QTimer* jumptimer;
    QTimer* attackTimer;
    bool izqui, der, jump, attack1, attack2;
    char last;
    int maxCont;
    QPixmap* body;

public:
    Personaje();
    virtual ~Personaje();

    // Estado y control
    void setIzqui(bool newIzqui);
    void setDer(bool newDer);
    char getLast() const;
    void setLast(char newLast);
    bool getJump() const;
    void setJump(bool newJump);
    bool timerState();

    // Métodos virtuales para animación que pueden ser sobreescritos
    virtual void updateSprite();
    virtual void splitSprite();
    virtual void jumpG();
    virtual void leftAttack();
    virtual void rightAttack();

protected slots:
    virtual void animation();
    virtual void stopJump();
    virtual void stopRattack();
    virtual void stopLattack();
};

#endif // PERSONAJE_H
