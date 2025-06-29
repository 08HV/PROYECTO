#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QMediaPlayer>
#include <QGraphicsVideoItem>
#include <QGraphicsScene>
#include <QAudioOutput>
#include <QTimer>
#include <QWidget>
#include <QMap>
#include "goku.h"



QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void resizeEvent(QResizeEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private slots:
    void startGame();
    void gameLoop();

private:
    Ui::MainWindow *ui;
    QPushButton *btnJugar;
    QPushButton *btnSalir;
    QMediaPlayer *player;
    QWidget *menu1;
    QGraphicsVideoItem *videoItem;
    QAudioOutput *audioOutput;
    QGraphicsScene *scene;

    Goku *goku;
    QTimer *timerControllers;
    QMap<char, bool> keyStates;

};

#endif // MAINWINDOW_H
