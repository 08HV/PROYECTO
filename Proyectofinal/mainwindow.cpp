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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , btnJugar(nullptr)
    , btnSalir(nullptr)
    , player(nullptr)
    , menu1(nullptr)
    , videoItem(nullptr)
    , audioOutput(nullptr)
{
    ui->setupUi(this);

    QGraphicsScene *scene = new QGraphicsScene(this);
    ui->GraphicsView->setScene(scene);

    videoItem = new QGraphicsVideoItem();
    videoItem->setSize(ui->GraphicsView->viewport()->size());
    scene->addItem(videoItem);
    scene->setSceneRect(QRectF(QPointF(0, 0), ui->GraphicsView->viewport()->size()));

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

    connect(btnJugar, &QPushButton::clicked, this, [](){
        // La idea es iniciar el Juego desde aqui
    });
    connect(btnSalir, &QPushButton::clicked, this, &QMainWindow::close);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);


    if (videoItem && ui->GraphicsView) {
        QSizeF size = ui->GraphicsView->viewport()->size();
        videoItem->setSize(size);
        if (ui->GraphicsView->scene())
            ui->GraphicsView->scene()->setSceneRect(QRectF(QPointF(0, 0), size));
    }
    if (menu1) {
        menu1->resize(ui->GraphicsView->viewport()->size());
    }
}
