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
    , nivel2(nullptr)
    , gokunube(nullptr)
    , maestro(nullptr)
    , nivel3(nullptr)
    , barraEnergiaGoku(nullptr)
    , barraEnergiaMaestro(nullptr)

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

    barraEnergiaGoku = new QProgressBar(this);
    barraEnergiaGoku->setGeometry(40, 60, 220, 30);
    barraEnergiaGoku->setRange(0, 100);
    barraEnergiaGoku->setValue(100);
    barraEnergiaGoku->setFormat("Goku: %v/100");
    barraEnergiaGoku->setStyleSheet("QProgressBar { color: white; font-size:18px; background: #222; border-radius:7px; } QProgressBar::chunk { background: #16e416; }");
    barraEnergiaGoku->hide();

    barraEnergiaMaestro = new QProgressBar(this);
    barraEnergiaMaestro->setGeometry(width()-260, 60, 220, 30);
    barraEnergiaMaestro->setRange(0, 100);
    barraEnergiaMaestro->setValue(100);
    barraEnergiaMaestro->setFormat("Maestro: %v/100");
    barraEnergiaMaestro->setStyleSheet("QProgressBar { color: white; font-size:18px; background: #222; border-radius:7px; } QProgressBar::chunk { background: #e41616; }");
    barraEnergiaMaestro->hide();


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
    qDebug() << "Destructor MainWindow llamado";
    if (timerControllers) {
        timerControllers->stop();
        delete timerControllers;
        timerControllers = nullptr;
    }
    if (nivel1) {
        delete nivel1;
        nivel1 = nullptr;
    }
    if (nivel2) {
        delete nivel2;
        nivel2 = nullptr;
    }
    if (nivel3) {
        delete nivel3;
        nivel3 = nullptr;
    }
    if (goku) {
        delete goku;
        goku = nullptr;
    }
    if (gokunube) {
        delete gokunube;
        gokunube=nullptr;
    }
    if (maestro) {
        delete maestro;
        maestro = nullptr;
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

    if (barraEnergiaGoku)
        barraEnergiaGoku->setGeometry(30, 30, 220, 30);

    if (barraEnergiaMaestro)
        barraEnergiaMaestro->setGeometry(size.width() - 250, 30, 220, 30);
}


void MainWindow::startGame()
{
    if (timerControllers) {
        timerControllers->stop();
        delete timerControllers;
        timerControllers = nullptr;
    }

    keyStates['W'] = false;
    keyStates['A'] = false;
    keyStates['S'] = false;
    keyStates['D'] = false;

    menu1->hide();
    player->stop();

    if (labelTiempo) labelTiempo->show();
    if (labelObjetos) labelObjetos->show();

    if (nivel1) {
        qDebug() << "Eliminando nivel1 en starGame";
        delete nivel1;
        nivel1 = nullptr;
        goku = nullptr;
    }
    goku = new Goku;
    goku->setPos(0, 540);

    nivel1 = new Nivel1(goku,this);
    nivel1->getEscena()->addItem(goku);

    int cantidad =  1;
    if (nivel1) nivel1->setCantidadObjetivo(cantidad);
    labelObjetos->setText(QString("Objetos: 0/%1").arg(cantidad));

    nivel1->iniciarNivel();

    connect(nivel1, &Nivel1::nivelCompletado, this, [this]() {

        labelTiempo->hide();
        labelObjetos->hide();

        if (timerControllers) {
            timerControllers->stop();
            delete timerControllers;
            timerControllers = nullptr;
        }


        QMediaPlayer* videoPlayer = new QMediaPlayer(this);
        QAudioOutput* audioOutputTrans = new QAudioOutput(this);
        QGraphicsVideoItem* videoItemTrans = new QGraphicsVideoItem();
        QGraphicsScene* videoScene = new QGraphicsScene(this);

        videoPlayer->setAudioOutput(audioOutputTrans);
        audioOutputTrans->setVolume(1.0);
        videoPlayer->setVideoOutput(videoItemTrans);

        QString ruta = QDir::currentPath() + "/recursos/video2.mp4";
        videoPlayer->setSource(QUrl::fromLocalFile(ruta));

        videoItemTrans->setSize(ui->GraphicsView->viewport()->size());
        videoScene->addItem(videoItemTrans);
        videoScene->setSceneRect(QRectF(0, 0, ui->GraphicsView->viewport()->width(), ui->GraphicsView->viewport()->height()));

        ui->GraphicsView->setScene(videoScene);
        videoPlayer->play();

        // Cuando termine el video, inicia el nivel 2
        connect(videoPlayer, &QMediaPlayer::mediaStatusChanged, this, [this, videoPlayer](QMediaPlayer::MediaStatus status){
            if (status == QMediaPlayer::EndOfMedia) {
                videoPlayer->stop();
                startGame2();
            }
        });
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

void MainWindow::startGame2()
{
    menu1->hide();

    qDebug() << "=== INICIO DE startGame2() ===";

    if (timerControllers) {
        qDebug() << "Eliminando timerControllers en startGame2";
        timerControllers->stop();
        disconnect(timerControllers,nullptr,this,nullptr);
        delete timerControllers;
        timerControllers = nullptr;
    }
    if (nivel1) {
        delete nivel1;
        nivel1 = nullptr;
        goku = nullptr;
    }

    if (nivel2) {
        auto escena = nivel2->getEscena();
        if (escena) {
            QList<QGraphicsItem*> items = escena->items();
            for (QGraphicsItem* item : items) {
                escena->removeItem(item);
            }
        }
        disconnect(nivel2, nullptr, this, nullptr);
        delete nivel2;
        nivel2 = nullptr;
    }


    if (gokunube) {
        qDebug() << "Eliminando gokunube";
        delete gokunube;
        qDebug() << "mas bie aqui el error, si elimino ????";
        gokunube = nullptr;
    }



    qDebug() << "creando a gokunube";

    gokunube = new Gokunube;
    gokunube->setPos(0, 0);
    qDebug() << "Gokunube creado en pos:" << gokunube->pos();
    nivel2 = new Nivel2(gokunube,this);
    nivel2->getEscena()->addItem(gokunube);

    qDebug() << "Items en la escena de nivel2 tras añadir gokunube:";
    for (auto item : nivel2->getEscena()->items())
        qDebug() << "Item:" << item;

    nivel2->iniciarNivel();

    ui->GraphicsView->setScene(nivel2->getEscena());


    timerControllers = new QTimer(this);
    connect(timerControllers, &QTimer::timeout, this, &MainWindow::gameLoop2);
    timerControllers->start(16);

    connect(nivel2, &Nivel2::nivelCompletado, this, [this]() {

        if (timerControllers) {
            timerControllers->stop();
            delete timerControllers;
            timerControllers = nullptr;
        }

        QMediaPlayer* videoPlayer = new QMediaPlayer(this);
        QAudioOutput* audioOutputTrans = new QAudioOutput(this);
        QGraphicsVideoItem* videoItemTrans = new QGraphicsVideoItem();
        QGraphicsScene* videoScene = new QGraphicsScene(this);

        videoPlayer->setAudioOutput(audioOutputTrans);
        audioOutputTrans->setVolume(1.0);
        videoPlayer->setVideoOutput(videoItemTrans);

        QString ruta = QDir::currentPath() + "/recursos/video3.mp4";
        videoPlayer->setSource(QUrl::fromLocalFile(ruta));

        videoItemTrans->setSize(ui->GraphicsView->viewport()->size());
        videoScene->addItem(videoItemTrans);
        videoScene->setSceneRect(QRectF(0, 0, ui->GraphicsView->viewport()->width(), ui->GraphicsView->viewport()->height()));

        ui->GraphicsView->setScene(videoScene);
        videoPlayer->play();


        connect(videoPlayer, &QMediaPlayer::mediaStatusChanged, this, [this, videoPlayer](QMediaPlayer::MediaStatus status){
            if (status == QMediaPlayer::EndOfMedia) {
                videoPlayer->stop();

                qDebug() << "=== FIN DE video2 ===";

                if (gokunube) { delete gokunube; gokunube = nullptr; }

                qDebug() << "=== FIN DE video ===";
                startGame3();
            }
        });
    });

    connect(nivel2, &Nivel2::nivelFallido, this, [this]() {

        if (timerControllers) {
            timerControllers->stop();
            delete timerControllers;
            timerControllers = nullptr;
        }

        QDialog dialog(this);
        dialog.setWindowTitle("Has perdido");
        QVBoxLayout *layout = new QVBoxLayout(&dialog);
        QLabel *label = new QLabel("¡PERDISTES!", &dialog);
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
            startGame2(); // para reiniciar nivel
        } else {
            close();
        }
    });

    qDebug() << "=== FIN DE startGame2() ===";
}

void MainWindow::startGame3()
{
    qDebug() << "=== INICIO DE startGame3() ===";
    keyStates['W'] = false;
    keyStates['A'] = false;
    keyStates['S'] = false;
    keyStates['D'] = false;
    if (player) player->stop();

    qDebug() << "=== error aqui o aqui ===";

    if (timerControllers) {
        timerControllers->stop();
        delete timerControllers;
        timerControllers = nullptr;
    }

    qDebug() << "=== error aqui o aqui o mas aqui ===";

    if (nivel3) {
        delete nivel3;
        nivel3 = nullptr;
    }
    if (goku) {
        delete goku;
        goku = nullptr;
    }
    if (maestro) {
        delete maestro;
        maestro = nullptr;
    }

    qDebug() << "=== error aqui ?? ===";

    if (barraEnergiaGoku) barraEnergiaGoku->show();
    if (barraEnergiaMaestro) barraEnergiaMaestro->show();

    goku = new Goku;
    goku->setPos(100, 540);

    maestro = new Maestro;
    maestro->setPos(700, 440);
    maestro->setPisoY(440);
    maestro->setGoku(goku);

    nivel3 = new Nivel3(goku, maestro, this);
    nivel3->getEscena()->addItem(goku);
    nivel3->getEscena()->addItem(maestro);


    barraEnergiaGoku->setValue(100);
    barraEnergiaMaestro->setValue(100);

    qDebug() << "=== o por aqui ?? ===";

    nivel3->iniciarNivel();


    connect(nivel3, &Nivel3::nivelCompletado, this, [this]() {
        barraEnergiaGoku->hide();
        barraEnergiaMaestro->hide();
        qDebug() << "=== o por aqui en complete ?? ===";

        QMediaPlayer* videoPlayer = new QMediaPlayer(this);
        QAudioOutput* audioOutputTrans = new QAudioOutput(this);
        QGraphicsVideoItem* videoItemTrans = new QGraphicsVideoItem();
        QGraphicsScene* videoScene = new QGraphicsScene(this);

        qDebug() << "=== o por aqui en complete2 ?? ===";

        videoPlayer->setAudioOutput(audioOutputTrans);
        audioOutputTrans->setVolume(1.0);
        videoPlayer->setVideoOutput(videoItemTrans);
        qDebug() << "=== o por aqui en complete3 ?? ===";
        QString ruta = QDir::currentPath() + "/recursos/video_final.mp4";
        videoPlayer->setSource(QUrl::fromLocalFile(ruta));
        qDebug() << "=== o por aqui ruta de video ?? ===";
        videoItemTrans->setSize(ui->GraphicsView->viewport()->size());
        videoScene->addItem(videoItemTrans);
        videoScene->setSceneRect(QRectF(0, 0, ui->GraphicsView->viewport()->width(), ui->GraphicsView->viewport()->height()));

        ui->GraphicsView->setScene(videoScene);
        videoPlayer->play();
        qDebug() << "=== sera aqui ?? ===";

        connect(videoPlayer, &QMediaPlayer::mediaStatusChanged, this, [this, videoPlayer](QMediaPlayer::MediaStatus status){
            if (status == QMediaPlayer::EndOfMedia) {
                videoPlayer->stop();
                qDebug() << "=== o por aqui  ===";

                if (goku) { delete goku; goku = nullptr; }
                qDebug() << "=== para ver  ===";
                if (maestro) { delete maestro; maestro = nullptr; }
                qDebug() << "=== mas bien aqui ?? ?? ===";
                if (timerControllers) {
                    timerControllers->stop();
                    delete timerControllers;
                    timerControllers = nullptr;
                }
                qDebug() << "=== aqui ?? ===";
                ui->GraphicsView->setScene(scene);
                if (videoItem && player) {
                    player->setPosition(0);
                    player->play();
                }
                close();
            }
        });
    });


    connect(nivel3, &Nivel3::nivelFallido, this, [this]() {
        barraEnergiaGoku->hide();
        barraEnergiaMaestro->hide();

        QDialog dialog(this);
        dialog.setWindowTitle("¡Perdiste!");
        QVBoxLayout *layout = new QVBoxLayout(&dialog);
        QLabel *label = new QLabel("El Maestro mato a Goku.", &dialog);
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
            if (goku) { delete goku; goku = nullptr; }
            qDebug() << "=== para ver  ===";
            if (maestro) { delete maestro; maestro = nullptr; }
            qDebug() << "=== mas bien aqui ?? ?? ===";
            if (timerControllers) {
                timerControllers->stop();
                delete timerControllers;
                timerControllers = nullptr;
            }
            startGame3();
        } else {
            close();
        }
    });

    ui->GraphicsView->setScene(nivel3->getEscena());

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

    int key = event->key();
    if (goku && goku -> isVisible()){
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

    if (gokunube && gokunube->isVisible()) {
        if (key == Qt::Key_Space) {
            gokunube->espacioPresionado = true;
        }
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent* event)
{
    int key = event->key();

    if (goku && goku ->isVisible()) {
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
    if (gokunube && gokunube->isVisible()) {
        if (key == Qt::Key_Space) {
            gokunube->espacioPresionado = false;
        }
    }
}

void MainWindow::gameLoop()
{
    if (nivel1 && goku && !nivel3) {
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
        return;
    }

    if (nivel3 && goku && maestro) {
        float v = 5.0f;
        Goku* gokuReal = dynamic_cast<Goku*>(goku);
        if (gokuReal) {
            v = gokuReal->acelerando ? gokuReal->velocidadAcelerada : gokuReal->velocidadNormal;
        }

        QRectF sceneRect = nivel3->getEscena()->sceneRect();
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


        maestro->updateAutonomo();
        maestro->updateFisica();


        ui->GraphicsView->centerOn(goku);

        // --- Colisiones y pelea ---
        if (goku->collidesWithItem(maestro)) {
            if (goku->isAttacking()) {
                nivel3->dañarMaestro(2);
            }
            else if (maestro->isAttacking()) {
                nivel3->dañarGoku(2);
            }
        }

        // Actualiza barras de energía
        if (barraEnergiaGoku)
            barraEnergiaGoku->setValue(nivel3->getEnergiaGoku());
        if (barraEnergiaMaestro)
            barraEnergiaMaestro->setValue(nivel3->getEnergiaMaestro());

        return;
    }

}

void MainWindow::gameLoop2()
{
    qDebug() << "gameLoop2 ejecutándose";

    if (!nivel2) {
        qDebug() << "ERROR: nivel2 es nullptr";
        return;
    }
    if (!gokunube) {
        qDebug() << "ERROR: gokunube es nullptr";
        return;
    }

    if (!gokunube || !nivel2 ) return;

    gokunube->updateFisica();
    ui->GraphicsView->centerOn(gokunube);
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


