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

    QTimer* caidoTimer = nullptr;
    bool attack3 = false;
    QPixmap* kickSprite = nullptr;
    QPixmap* attack2Sprite = nullptr;
    QPixmap* caidoSprite = nullptr;
    bool caido = false;
    float impulsoX = 0.0f;

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
    virtual void kickAttack();

    bool isAttacking() const;
    virtual void startAttack();
    virtual void endAttack();
    virtual void morir();

    virtual void updateFisica();
    void setPisoY(float y) { pisoY = y; }

    void caer(float fuerzaImpulsoX = 0.0f, float fuerzaImpulsoY = -18.0f);
    bool estaCaido() const { return caido; }

    bool isAttack1() const { return attack1; }
    bool isAttack2() const { return attack2; }
    bool isAttack3() const { return attack3; }

protected slots:
    virtual void animation();
    virtual void stopRattack();
    virtual void stopLattack();
    virtual void stopKickAttack();
    virtual void recuperarseDeCaida();

};

#endif // PERSONAJE_H
