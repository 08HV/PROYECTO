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
#include <QDialog>
#include <QLabel>
#include <QHBoxLayout>

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
    , labelTiempo(nullptr)
    , labelObjetos(nullptr)

{
    ui->setupUi(this);

    //ui->GraphicsView->setFixedSize(3400, 850);

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

    menu1 = new QWidget(ui->GraphicsView->viewport());
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

    labelTiempo = new QLabel("Tiempo: 0", this);
    labelTiempo->setStyleSheet("font-size: 22px; color: white; background: rgba(0,0,0,120);");
    labelTiempo->setGeometry(30, 10, 180, 40);
    labelTiempo->hide();

    labelObjetos = new QLabel("Objetos: 0/12", this);
    labelObjetos->setStyleSheet("font-size: 22px; color: white; background: rgba(0,0,0,120);");
    labelObjetos->setGeometry(width()-230, 10, 200, 40);
    labelObjetos->setAlignment(Qt::AlignRight);
    labelObjetos->hide();

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

    if (videoItem && ui->GraphicsView->scene() == scene) {
        videoItem->setSize(size);
        scene->setSceneRect(QRectF(0, 0, size.width(), size.height()));
    }
    if (menu1) {
        menu1->resize(size.toSize());
    }

    if (labelObjetos)
        labelObjetos->setGeometry(width()-230, 10, 200, 40);
}


void MainWindow::startGame()
{

    keyStates['W'] = false;
    keyStates['A'] = false;
    keyStates['S'] = false;
    keyStates['D'] = false;

    menu1->hide();
    player->stop();

    if (labelTiempo) labelTiempo->show();
    if (labelObjetos) labelObjetos->show();

    if (nivel1) {
        delete nivel1;
        nivel1 = nullptr;
    }
    goku = new Goku;
    goku->setPos(0, 540);

    nivel1 = new Nivel1(goku,this);
    nivel1->getEscena()->addItem(goku);

    int cantidad =  12;
    if (nivel1) nivel1->setCantidadObjetivo(cantidad);
    labelObjetos->setText(QString("Objetos: 0/%1").arg(cantidad));

    nivel1->iniciarNivel();

    connect(nivel1, &Nivel1::nivelCompletado, this, [this]() {
        labelTiempo->hide();
        labelObjetos->hide();

        QDialog dialog(this);
        dialog.setWindowTitle("¡Felicidades!");
        QVBoxLayout *layout = new QVBoxLayout(&dialog);
        QLabel *label = new QLabel("¡Nivel completado!", &dialog);
        layout->addWidget(label);

        QHBoxLayout *buttonLayout = new QHBoxLayout;
        QPushButton *btnSiguienteNivel = new QPushButton("Siguiente nivel", &dialog);
        QPushButton *btnReiniciar = new QPushButton("Reiniciar", &dialog);
        QPushButton *btnSalir = new QPushButton("Salir", &dialog);

        buttonLayout->addWidget(btnSiguienteNivel);
        buttonLayout->addWidget(btnReiniciar);
        buttonLayout->addWidget(btnSalir);
        layout->addLayout(buttonLayout);


        connect(btnSiguienteNivel, &QPushButton::clicked, &dialog, &QDialog::accept);
        connect(btnReiniciar, &QPushButton::clicked, &dialog, &QDialog::reject);
        connect(btnSalir, &QPushButton::clicked, &dialog, [this, &dialog]() {
            dialog.done(2);
        });

        int result = dialog.exec();

        if (result == QDialog::Accepted) {
            //siguiente nivel2
            // por ahora solo muesro el manu1
            menu1->show();
        } else if (result == QDialog::Rejected) {

            startGame();
        } else if (result == 2) {

            close();
        }
    });

    connect(nivel1, &Nivel1::nivelFallido, this, [this]() {
        labelTiempo->hide();
        labelObjetos->hide();

        QDialog dialog(this);
        dialog.setWindowTitle("Tiempo agotado o muerto");
        QVBoxLayout *layout = new QVBoxLayout(&dialog);
        QLabel *label = new QLabel("No lograste recoger todos los objetos.", &dialog);
        layout->addWidget(label);

        QHBoxLayout *buttonLayout = new QHBoxLayout;
        QPushButton *btnReiniciar = new QPushButton("Reiniciar", &dialog);
        QPushButton *btnSalir = new QPushButton("Salir", &dialog);

        buttonLayout->addWidget(btnReiniciar);
        buttonLayout->addWidget(btnSalir);
        layout->addLayout(buttonLayout);

        connect(btnReiniciar, &QPushButton::clicked, &dialog, &QDialog::accept);
        connect(btnSalir, &QPushButton::clicked, &dialog, &QDialog::reject);

        int result = dialog.exec();

        if (result == QDialog::Accepted) {
            startGame();
        } else {
            close();
        }
    });


    connect(nivel1, &Nivel1::tiempoActualizadoNivel, this, [this](int segundos) {
        if (labelTiempo)
            labelTiempo->setText(QString("Tiempo: %1").arg(segundos));
    });

    ui->GraphicsView->setScene(nivel1->getEscena());

    if (timerControllers) {
        timerControllers->stop();
        delete timerControllers;
    }
    timerControllers = new QTimer(this);
    connect(timerControllers, &QTimer::timeout, this, &MainWindow::gameLoop);
    timerControllers->start(16);

}

void MainWindow::manejarAceleracion(int key, bool presionado)
{
    if (!goku) return;
    Goku* gokuReal = dynamic_cast<Goku*>(goku);
    if (!gokuReal) return;

    if (key == Qt::Key_Shift) {
        gokuReal->shiftPressed = presionado;
        if (!presionado) {
            gokuReal->desactivarAceleracion();
        }
    }

    if ((key == Qt::Key_A || key == Qt::Key_D) && presionado) {
        gokuReal->comboCounter++;
        if (gokuReal->comboTimer->isActive())
            gokuReal->comboTimer->stop();
        gokuReal->comboTimer->start(1000);

        if (gokuReal->comboCounter >= 3 || gokuReal->shiftPressed) {
            gokuReal->activarAceleracion();
        }
    }
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
     manejarAceleracion(key, true);
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

     manejarAceleracion(key, true);
}

void MainWindow::gameLoop()
{
    if (!goku || !nivel1 ) return;
    float v = 5.0f;
    Goku* gokuReal = dynamic_cast<Goku*>(goku);
    if (gokuReal) {
        v = gokuReal->acelerando ? gokuReal->velocidadAcelerada : gokuReal->velocidadNormal;
    }

    QRectF sceneRect = nivel1->getEscena()->sceneRect();
    QRectF gokuRect = goku->boundingRect().translated(goku->pos());

    if (keyStates['D']) {
        if (gokuRect.right() + v <= sceneRect.right())
        goku->moveBy(v, 0);
    }
    else if (keyStates['A']) {
        if (gokuRect.left() - v >= sceneRect.left())
        goku->moveBy(-v, 0);
    }

    goku->updateFisica();
    ui->GraphicsView->centerOn(goku);


    for (Obstaculo* obs : nivel1->getObstaculos()) {
        Tortuga* tortuga = dynamic_cast<Tortuga*>(obs);
        if (!tortuga) continue;

        if (tortuga->x() < 0) {
            nivel1->eliminarObstaculo(tortuga);
            continue;
        }

        if (goku->collidesWithItem(tortuga)) {
            if (goku->isAttacking()) {
                nivel1->eliminarObstaculo(tortuga);
                break;
            } else {
                goku->morir();
                nivel1->terminarNivel(false);
                return;
            }
        }
    }

    if (nivel1->getColeccionable() && goku->collidesWithItem(nivel1->getColeccionable())) {
        nivel1->recogerColeccionable();
        if (labelObjetos)
            labelObjetos->setText(
                QString("Objetos: %1/%2")
                    .arg(nivel1->getObjetosRecogidos())
                    .arg(nivel1->getCantidadObjetivo())
            );
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


