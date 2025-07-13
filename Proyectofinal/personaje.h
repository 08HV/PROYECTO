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
    QTimer* attackTimer;
    bool izqui, der, jump, attack1, attack2;
    char last;
    int maxCont;
    QPixmap* body;
    bool attacking;
    float velocidadY = 0.0f;
    float gravedad = 1.2f;
    bool enElAire = false;
    float pisoY = 540;

public:
    Personaje();
    virtual ~Personaje();

    // control
    void setIzqui(bool newIzqui);
    void setDer(bool newDer);
    char getLast() const;
    void setLast(char newLast);
    bool getJump() const;
    void setJump(bool newJump);
    bool timerState();


    virtual void updateSprite();
    virtual void splitSprite();
    virtual void jumpG();
    virtual void leftAttack();
    virtual void rightAttack();

    bool isAttacking() const;
    virtual void startAttack();
    virtual void endAttack();
    virtual void morir();

    virtual void updateFisica();
    void setPisoY(float y) { pisoY = y; }

protected slots:
    virtual void animation();
    virtual void stopRattack();
    virtual void stopLattack();

};

#endif // PERSONAJE_H
