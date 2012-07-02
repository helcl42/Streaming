#include "Player.h"
#include <QtCore/QDir>
#include <QtCore/QUrl>
#include <QGlib/Connect>
#include <QGlib/Error>
#include <QGst/Pipeline>
#include <QGst/ElementFactory>
#include <QGst/Bus>
#include <QGst/Message>
#include <QGst/Query>
#include <QGst/ClockTime>
#include <QGst/Event>
#include <QGst/StreamVolume>

Player::Player(QWidget *parent) : QGst::Ui::VideoWidget(parent) {
    //this timer is used to tell the ui to change its position slider & label
    //every 100 ms, but only when the pipeline is playing
    connect(&m_positionTimer, SIGNAL(timeout()), this, SIGNAL(positionChanged()));    
}

Player::~Player() {
    if (m_pipeline) {
        m_pipeline->setState(QGst::StateNull);
        stopPipelineWatch();
    }
}

void Player::setUri(const QString & uri) {
    QString realUri = uri;

    //if uri is not a real uri, assume it is a file path
    if (realUri.indexOf("://") < 0) {
        realUri = QUrl::fromLocalFile(realUri).toEncoded();
    }

    if (!m_pipeline) {
        m_pipeline = QGst::ElementFactory::make("playbin2").dynamicCast<QGst::Pipeline > ();
        std::cout << "Pipeline is NULL" << std::endl;
        if (m_pipeline) {
            //let the video widget watch the pipeline for new video sinks
            watchPipeline(m_pipeline);

            //watch the bus for messages
            QGst::BusPtr bus = m_pipeline->bus();
            bus->addSignalWatch();
            QGlib::connect(bus, "message", this, &Player::onBusMessage);
        } else {
            qCritical() << "Failed to create the pipeline";
        }
    }

    if (m_pipeline) {
        m_pipeline->setProperty("uri", realUri);
    }
}

QTime Player::getPosition() const {
    if (m_pipeline) {
        //here we query the pipeline about its position
        //and we request that the result is returned in time format
        QGst::PositionQueryPtr query = QGst::PositionQuery::create(QGst::FormatTime);
        m_pipeline->query(query);
        return QGst::ClockTime(query->position()).toTime();
    } else {
        return QTime();
    }
}

void Player::setPosition(const QTime & pos) {
    QGst::SeekEventPtr evt = QGst::SeekEvent::create(
            1.0, QGst::FormatTime, QGst::SeekFlagFlush,
            QGst::SeekTypeSet, QGst::ClockTime::fromTime(pos),
            QGst::SeekTypeNone, QGst::ClockTime::None
            );

    m_pipeline->sendEvent(evt);
}

int Player::getVolume() const {
    if (m_pipeline) {
        QGst::StreamVolumePtr svp = m_pipeline.dynamicCast<QGst::StreamVolume > ();

        if (svp) {
            return svp->volume(QGst::StreamVolumeFormatCubic) * 10;
        }
    }

    return 0;
}

void Player::setVolume(int volume) {
    if (m_pipeline) {
        QGst::StreamVolumePtr svp = m_pipeline.dynamicCast<QGst::StreamVolume > ();

        if (svp) {
            svp->setVolume((double) volume / 10, QGst::StreamVolumeFormatCubic);
        }
    }
}

QTime Player::getLength() const {
    if (m_pipeline) {
        //here we query the pipeline about the content's duration
        //and we request that the result is returned in time format
        QGst::DurationQueryPtr query = QGst::DurationQuery::create(QGst::FormatTime);
        m_pipeline->query(query);
        return QGst::ClockTime(query->duration()).toTime();
    } else {
        return QTime();
    }
}

QGst::State Player::getState() const {
    return m_pipeline ? m_pipeline->currentState() : QGst::StateNull;
}

void Player::play() {
    if (m_pipeline) {
        m_pipeline->setState(QGst::StatePlaying);
    }
}

void Player::pause() {
    if (m_pipeline) {
        m_pipeline->setState(QGst::StatePaused);
    }
}

void Player::stop() {
    if (m_pipeline) {
        m_pipeline->setState(QGst::StateNull);

        //once the pipeline stops, the bus is flushed so we will
        //not receive any StateChangedMessage about this.
        //so, to inform the ui, we have to emit this signal manually.
        Q_EMIT stateChanged();
    }
}

void Player::onBusMessage(const QGst::MessagePtr & message) {
    switch (message->type()) {
        case QGst::MessageEos: //End of stream. We reached the end of the file.                       
            MediaApp::getInstance()->openFile(MediaApp::getInstance()->getPlaylistInstance()->getNext());
            break;
        case QGst::MessageError: //Some error occurred.
            qCritical() << message.staticCast<QGst::ErrorMessage > ()->error();
            stop();
            break;
        case QGst::MessageStateChanged: //The element in message->source() has changed state
            if (message->source() == m_pipeline) {
                handlePipelineStateChange(message.staticCast<QGst::StateChangedMessage > ());
            }
            break;
        default:
            break;
    }
}

void Player::handlePipelineStateChange(const QGst::StateChangedMessagePtr & scm) {
    switch (scm->newState()) {
        case QGst::StatePlaying:
            //start the timer when the pipeline starts playing
            m_positionTimer.start(100);
            break;
        case QGst::StatePaused:
            //stop the timer when the pipeline pauses
            if (scm->oldState() == QGst::StatePlaying) {
                m_positionTimer.stop();
            }
            break;
        default:
            break;
    }

    Q_EMIT stateChanged();
}

