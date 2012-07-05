#include "MediaApp.h"
#include "Player.h"

#include <QtGui/QBoxLayout>
#include <QtGui/QFileDialog>
#include <QtGui/QToolButton>
#include <QtGui/QLabel>
#include <QtGui/QSlider>
#include <QtGui/QMouseEvent>
#include <qt4/QtCore/qvariant.h>
#include <qt4/QtGui/qlabel.h>


MediaApp* MediaApp::m_instance = NULL;

MediaApp* MediaApp::getInstance() {
    if (!m_instance) {
        m_instance = new MediaApp();
    }
    return m_instance;
}

MediaApp::MediaApp() : QWidget(), m_libraryOpened(false) {
    m_player = new Player(this);
    m_playlist = new Playlist(this);
    m_albumWidget = new AlbumWidget(this);
    m_albumWidget->LoadImage("Player", "The");
    m_albumWidget->setGeometry(46, 16, 300, 300);

    connect(m_player, SIGNAL(positionChanged()), this, SLOT(onPositionChanged()));
    connect(m_player, SIGNAL(stateChanged()), this, SLOT(onStateChanged()));

    m_baseDir = QLatin1String(".");

    m_fullScreenTimer.setSingleShot(true);
    connect(&m_fullScreenTimer, SIGNAL(timeout()), this, SLOT(hideControls()));

    QHBoxLayout* mainLayout = new QHBoxLayout();
    mainLayout->setContentsMargins(0, 0, 0, 0);
    createUI(mainLayout);
    setLayout(mainLayout);

    onStateChanged();

    setWindowTitle(tr("ThePlayer"));
    resize(700, 400);
}

MediaApp::~MediaApp() {
    SAFE_DELETE(m_player);
    SAFE_DELETE(m_playlist);
    SAFE_DELETE(m_albumWidget);
    SAFE_DELETE(m_fullScreenButton);
    SAFE_DELETE(m_nextButton);
    SAFE_DELETE(m_prevButton);
    SAFE_DELETE(m_openButton);
    SAFE_DELETE(m_openLibrary);
    SAFE_DELETE(m_pauseButton);
    SAFE_DELETE(m_playButton);
    SAFE_DELETE(m_positionSlider);
}

void MediaApp::openFile(Song* song) {
    if (song == NULL) {
        return;
    }

    m_baseDir = QFileInfo(QString::fromStdString(song->getUrl())).path();

    m_player->stop();
    m_player->setUri(QString::fromStdString(song->getUrl()));
    m_player->play();

    //    std::cout << "LENGTH: " << m_player->findOutMediaLength(song).toString("hh:mm:ss").toStdString() << std::endl;

    if (song->isAudio()) {
        QString artistName;
        QString albumName;
        if (song->isFromLibrary()) {
            artistName = QString::fromStdString(song->getArtistName());
            albumName = QString::fromStdString(song->getAlbumName());
        } else {
            artistName = "Player";
            albumName = "The";
        }

        m_player->repaint();
        m_albumWidget->LoadImage(artistName, albumName);
        m_albumWidget->setGeometry(46, 16, 300, 300);
        m_albumWidget->setVisible(true);
    } else {
        m_albumWidget->setVisible(false);
        m_player->setVisible(true);
    }
}

void MediaApp::open() {
    QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Open a Media"), m_baseDir);

    for (int i = 0; i < fileNames.count(); i++) {
        if (!fileNames.at(i).isEmpty()) {
            Song* song = new Song();
            song->setId(m_playlist->getActualPosition() + 1);
            song->setTitle(Song::getSongTitleFromPath(fileNames.at(i).toStdString()));
            song->setUrl(fileNames.at(i).toStdString());
            song->setLength(m_player->getLength().toString("hh:mm:ss").toStdString());
            song->setFromLibrary(false);
            song->setAudio(Song::findOutIfAudioMedia(song));
            m_playlist->insert(song);

            if (i == 0) {
                openFile(song);
            }
        }
    }
}

void MediaApp::openLibrary() {
    if (!m_libraryOpened) {
        //Library::getInstance()->connectToServer("127.0.0.1", 12345);
        Library::getInstance()->connectToServer("ec2-23-21-7-252.compute-1.amazonaws.com", 12345);
        Library::getInstance()->setVisible(true);
        Library::getInstance()->setSatusMessage("Connecting");
        Library::getInstance()->enableSearch(false);
        m_libraryOpened = true;
        return;
    }

    if (Library::getInstance()->isVisible()) {
        Library::getInstance()->setVisible(false);
    } else {
        Library::getInstance()->setVisible(true);
    }
}

void MediaApp::openLibraryCallback(bool connected) {
    if (connected) {
        Library::getInstance()->enableSearch(true);
        Library::getInstance()->setSatusMessage("Connected");
    } else {
        Library::getInstance()->enableSearch(false);
        Library::getInstance()->setSatusMessage("Connection Failed");
    }
}

void MediaApp::next() {
    Song* nextSong = m_playlist->getNext();
    if (nextSong != NULL) {
        openFile(nextSong);
    }
}

void MediaApp::prev() {
    Song* prevSong = m_playlist->getPrevious();
    if (prevSong != NULL) {
        openFile(prevSong);
    }
}

void MediaApp::toggleFullScreen() {
    if (isFullScreen()) {
        setMouseTracking(false);
        m_player->setMouseTracking(false);
        m_fullScreenTimer.stop();
        showControls();
        showNormal();
    } else {
        setMouseTracking(true);
        m_player->setMouseTracking(true);
        hideControls();
        showFullScreen();
    }
}

void MediaApp::onStateChanged() {
    QGst::State newState = m_player->getState();
    m_playButton->setEnabled(newState != QGst::StatePlaying);
    m_pauseButton->setEnabled(newState == QGst::StatePlaying);
    m_nextButton->setEnabled(newState == QGst::StatePlaying);
    m_prevButton->setEnabled(newState == QGst::StatePlaying);
    m_stopButton->setEnabled(newState != QGst::StateNull);
    m_positionSlider->setEnabled(newState != QGst::StateNull);
    m_volumeSlider->setEnabled(newState != QGst::StateNull);
    m_volumeLabel->setEnabled(newState != QGst::StateNull);
    m_volumeSlider->setValue(m_player->getVolume());

    if (newState == QGst::StateNull) {
        onPositionChanged();
    }
}

void MediaApp::onPositionChanged() {
    QTime length(0, 0);
    QTime curpos(0, 0);

    if (m_player->getState() != QGst::StateReady && m_player->getState() != QGst::StateNull) {
        length = m_player->getLength();
        curpos = m_player->getPosition();
    }

    m_positionLabel->setText(curpos.toString("hh:mm:ss.zzz") + "/" + length.toString("hh:mm:ss.zzz"));

    if (length != QTime(0, 0)) {
        m_positionSlider->setValue(curpos.msecsTo(QTime()) * 1000 / length.msecsTo(QTime()));
    } else {
        m_positionSlider->setValue(0);
    }

    if (curpos != QTime(0, 0)) {
        m_positionLabel->setEnabled(true);
        m_positionSlider->setEnabled(true);
    }
}

/**
 * called when the user changes the slider position 
 * @param value
 */
void MediaApp::setPosition(int value) {
    uint length = -m_player->getLength().msecsTo(QTime());
    if (length != 0 && value > 0) {
        QTime pos;
        pos = pos.addMSecs(length * (value / 1000.0));
        m_player->setPosition(pos);
    }
}

void MediaApp::showControls(bool show) {
    m_openButton->setVisible(show);
    m_playButton->setVisible(show);
    m_pauseButton->setVisible(show);
    m_stopButton->setVisible(show);
    m_nextButton->setVisible(show);
    m_prevButton->setVisible(show);
    m_openLibrary->setVisible(show);
    m_fullScreenButton->setVisible(show);
    m_positionSlider->setVisible(show);
    m_volumeSlider->setVisible(show);
    m_volumeLabel->setVisible(show);
    m_positionLabel->setVisible(show);
    m_playlist->setVisible(show);
}

void MediaApp::closeEvent(QCloseEvent* /* close */) {
    if (m_libraryOpened) {
        Library* lib = Library::getInstance();
        lib->close();
    }
    std::cout << "BYE!" << std::endl;
}

void MediaApp::wheelEvent(QWheelEvent *event) {
    int numDegrees = event->delta() / 8;
    double numSteps = numDegrees / 7.5f;
    if (m_player->getVolume() > 0) {
        m_volumeSlider->moveSlider(m_player->getVolume() + numSteps);
    }
}

void MediaApp::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
        case Qt::Key_Space:
            m_positionSlider->setFocus();
            if (m_player->getState() == QGst::StatePlaying) {
                m_player->pause();
            } else {
                m_player->play();
            }
            break;
        case Qt::Key_O:
            open();
            break;
        case Qt::Key_Left:
            m_playlist->setFocus();
            openFile(m_playlist->getPrevious());
            break;
        case Qt::Key_Right:
            m_playlist->setFocus();
            openFile(m_playlist->getNext());
            break;
        case Qt::Key_Up:
            if (m_player->getVolume() < 10) {
                m_volumeSlider->moveSlider(m_player->getVolume() + 2);
            }
            break;
        case Qt::Key_Down:
            if (m_player->getVolume() > 0) {
                m_volumeSlider->moveSlider(m_player->getVolume() - 1);
            }
            break;
        case Qt::Key_Delete:
            m_playlist->removeSelected();
            break;
        default:
            QWidget::keyPressEvent(event);
    }
}

void MediaApp::hideControls() {
    showControls(false);
}

/**
 * hides controls in next 3s
 * @param event
 */
void MediaApp::mouseMoveEvent(QMouseEvent *event) {
    Q_UNUSED(event);
    if (isFullScreen()) {
        showControls();
        m_fullScreenTimer.start(3000);
    }
}

QToolButton *MediaApp::initButton(QStyle::StandardPixmap icon, const QString & tip, QObject* dstobj, const char* slot_method, QLayout* layout) {
    QToolButton* button = new QToolButton;
    button->setIcon(style()->standardIcon(icon));
    button->setIconSize(QSize(22, 22));
    button->setToolTip(tip);
    connect(button, SIGNAL(clicked()), dstobj, slot_method);
    layout->addWidget(button);

    return button;
}

void MediaApp::createUI(QBoxLayout *mainLayout) {
    QVBoxLayout* appLayout = new QVBoxLayout;
    appLayout->setContentsMargins(0, 0, 0, 0);
    appLayout->addWidget(m_player);

    m_positionLabel = new QLabel();

    m_positionSlider = new QSlider(Qt::Horizontal);
    m_positionSlider->setTickPosition(QSlider::TicksBelow);
    m_positionSlider->setTickInterval(10);
    m_positionSlider->setMaximum(1000);

    connect(m_positionSlider, SIGNAL(sliderMoved(int)), this, SLOT(setPosition(int)));

    m_volumeSlider = new VolumeSlider(Qt::Horizontal);
    m_volumeSlider->setTickPosition(QSlider::TicksLeft);
    m_volumeSlider->setTickInterval(2);
    m_volumeSlider->setMaximum(10);
    m_volumeSlider->setMaximumSize(54, 22);

    connect(m_volumeSlider, SIGNAL(sliderMoved(int)), m_player, SLOT(setVolume(int)));

    QGridLayout* posLayout = new QGridLayout;
    posLayout->setContentsMargins(2, 0, 2, 0);
    posLayout->addWidget(m_positionLabel, 1, 0);
    posLayout->addWidget(m_positionSlider, 1, 1, 1, 2);
    appLayout->addLayout(posLayout);

    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLayout->addStretch();

    m_openButton = initButton(QStyle::SP_DialogOpenButton, tr("Open File"), this, SLOT(open()), btnLayout);
    m_prevButton = initButton(QStyle::SP_MediaSkipBackward, tr("Prev"), this, SLOT(prev()), btnLayout);
    m_playButton = initButton(QStyle::SP_MediaPlay, tr("Play"), m_player, SLOT(play()), btnLayout);
    m_pauseButton = initButton(QStyle::SP_MediaPause, tr("Pause"), m_player, SLOT(pause()), btnLayout);
    m_stopButton = initButton(QStyle::SP_MediaStop, tr("Stop"), m_player, SLOT(stop()), btnLayout);
    m_nextButton = initButton(QStyle::SP_MediaSkipForward, tr("Next"), this, SLOT(next()), btnLayout);
    m_openLibrary = initButton(QStyle::SP_DriveNetIcon, tr("Open Library"), this, SLOT(openLibrary()), btnLayout);
    m_fullScreenButton = initButton(QStyle::SP_TitleBarMaxButton, tr("Fullscreen"), this, SLOT(toggleFullScreen()), btnLayout);

    btnLayout->addStretch();

    m_volumeLabel = new QLabel();
    m_volumeLabel->setPixmap(style()->standardIcon(QStyle::SP_MediaVolume).pixmap(QSize(22, 22), QIcon::Normal, QIcon::On));

    btnLayout->addWidget(m_volumeLabel);
    btnLayout->addWidget(m_volumeSlider);

    appLayout->addLayout(btnLayout);
    mainLayout->addLayout(appLayout);
    m_playlist->setMaximumWidth(300);
    mainLayout->addWidget(m_playlist);
}

bool MediaApp::isLibraryOpenned() const {
    return m_libraryOpened;
}

void MediaApp::setLibraryOpened(bool val) {
    m_libraryOpened = val;
}

Playlist* MediaApp::getPlaylistInstance() const {
    return m_playlist;
}

Player* MediaApp::getPlayerInstance() const {
    return m_player;
}

AlbumWidget* MediaApp::getAlbumWidgetInstance() const {
    return m_albumWidget;
}