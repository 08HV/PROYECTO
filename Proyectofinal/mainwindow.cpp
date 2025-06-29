#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMediaPlayer>
#include <QGraphicsVideoItem>
#include <QGraphicsScene>
#include <QVBoxLayout>
#include <QWidget>
#include <QUrl>
#include <QDir>
#include <QDebug>
#include <QResizeEvent>
#include <QAudioOutput>
#include <QKeyEvent>
#include <QMouseEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , btnJugar(nullptr)
    , btnSalir(nullptr)
    , player(nullptr)
    , menu1(nullptr)
    , videoItem(nullptr)
    , audioOutput(nullptr)
    , scene(nullptr)
    , goku(nullptr)
    , timerControllers(nullptr)
    , nivel1(nullptr)
{
    ui->setupUi(this);

    scene = new QGraphicsScene(this);
    ui->GraphicsView->setScene(scene);

    videoItem = new QGraphicsVideoItem();
    videoItem->setSize(ui->GraphicsView->viewport()->size());
    scene->addItem(videoItem);
    scene->setSceneRect(QRectF(0, 0, ui->GraphicsView->viewport()->width(), ui->GraphicsView->viewport()->height()));

    player = new QMediaPlayer(this);
    audioOutput = new QAudioOutput(this);
    player->setAudioOutput(audioOutput);
    audioOutput->setVolume(1.0);
    player->setVideoOutput(videoItem);

    QString ruta = QDir::currentPath() + "/recursos/video.mp4";
    qDebug() << "Buscando video en:" << ruta << QFile::exists(ruta);

    if (!QFile::exists(ruta)) {
        qDebug() << "ERROR: El archivo de video no existe. No se podrá reproducir.";
    } else {
        player->setSource(QUrl::fromLocalFile(ruta));
        player->play();
    }


    connect(player, &QMediaPlayer::errorOccurred, this, [](QMediaPlayer::Error error, const QString &errorString){
        qDebug() << "ERROR QMediaPlayer:" << error << errorString;
    });


    connect(player, &QMediaPlayer::mediaStatusChanged, this, [this](QMediaPlayer::MediaStatus status){
        if (status == QMediaPlayer::EndOfMedia) {
            player->setPosition(0);
            player->play();
        }
    });

    menu1 = new QWidget(ui->GraphicsView->viewport()); // ¡Usa el miembro!
    menu1->setAttribute(Qt::WA_TransparentForMouseEvents, false);
    menu1->setAttribute(Qt::WA_TranslucentBackground, true);
    menu1->setStyleSheet(R"(
        background-color: transparent;
        border: 2px solid red;
        border-radius: 10px;
    )");
    menu1->setObjectName("menu1");

    QVBoxLayout *vertlay = new QVBoxLayout(menu1);
    vertlay->setAlignment(Qt::AlignCenter);
    vertlay->setSpacing(35);
    vertlay->setContentsMargins(30, 120, 30, 120);

    btnJugar = new QPushButton("JUGAR", menu1);
    btnSalir = new QPushButton("SALIR", menu1);

    btnJugar->setFixedWidth(120);
    btnSalir->setFixedWidth(120);

    btnJugar->setStyleSheet("font-size: 32px;");
    btnSalir->setStyleSheet("font-size: 32px;");

    vertlay->addWidget(btnJugar);
    vertlay->addWidget(btnSalir);

    menu1->resize(ui->GraphicsView->viewport()->size());
    menu1->show();

    connect(btnJugar, &QPushButton::clicked, this, &MainWindow::startGame);

    connect(btnSalir, &QPushButton::clicked, this, &QMainWindow::close);

    keyStates['W'] = false;
    keyStates['A'] = false;
    keyStates['S'] = false;
    keyStates['D'] = false;

    showMaximized();
}

MainWindow::~MainWindow()
{
    if (timerControllers) {
        timerControllers->stop();
        delete timerControllers;
        timerControllers = nullptr;
    }
    if (nivel1) {
        delete nivel1;
        nivel1 = nullptr;
    }
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);

    QSizeF size = ui->GraphicsView->viewport()->size();

    if (videoItem && ui->GraphicsView) {
        videoItem->setSize(size);
        if (ui->GraphicsView->scene())
            ui->GraphicsView->scene()->setSceneRect(QRectF(0, 0, size.width(), size.height()));
    }
    if (menu1) {
        menu1->resize(size.toSize());
    }
    if (nivel1 && nivel1->getEscena()) {
        nivel1->getEscena()->setSceneRect(QRectF(0, 0, size.width(), size.height()));
    }
}

void MainWindow::startGame()
{
    menu1->hide();
    player->stop();

    if (nivel1) {
        delete nivel1;
        nivel1 = nullptr;
    }  
    goku = new Goku;
    goku->setPos(0, 410);

    nivel1 = new Nivel1(goku, this);
    nivel1->getEscena()->addItem(goku);
    nivel1->iniciarNivel();

    QSizeF size = ui->GraphicsView->viewport()->size();
    ui->GraphicsView->setScene(nivel1->getEscena());
    ui->GraphicsView->scene()->setSceneRect(QRectF(0, 0, size.width(), size.height()));

    if (timerControllers) {
        timerControllers->stop();
        delete timerControllers;
    }
    timerControllers = new QTimer(this);
    connect(timerControllers, &QTimer::timeout, this, &MainWindow::gameLoop);
    timerControllers->start(16);

}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (!goku) return;
    int key = event->key();

    if (key == Qt::Key_W) {
        keyStates['W'] = true;
    }
    else if (key == Qt::Key_A) {
        keyStates['A'] = true;
        goku->setIzqui(true);
        goku->setLast('A');
    }
    else if (key == Qt::Key_S) {
        keyStates['S'] = true;
    }
    else if (key == Qt::Key_D) {
        keyStates['D'] = true;
        goku->setDer(true);
        goku->setLast('D');
    }
    else if ((key == Qt::Key_Space) && !goku->timerState()) {
        goku->jumpG();
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent* event)
{
    if (!goku) return;
    int key = event->key();

    if (key == Qt::Key_W) {
        keyStates['W'] = false;
    }
    else if (key == Qt::Key_A) {
        keyStates['A'] = false;
        goku->setIzqui(false);
    }
    else if (key == Qt::Key_S) {
        keyStates['S'] = false;
    }
    else if (key == Qt::Key_D) {
        keyStates['D'] = false;
        goku->setDer(false);
    }
}

void MainWindow::gameLoop()
{
    if (!goku) return;
    if (keyStates['D']) {
        goku->moveBy(5, 0);
    }
    else if (keyStates['A']) {
        goku->moveBy(-5, 0);
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (!goku) return;
    Qt::MouseButton button = event->button();
    if (button == Qt::LeftButton) {
        goku->leftAttack();
    }
    else if (button == Qt::RightButton) {
        goku->rightAttack();
    }
}

