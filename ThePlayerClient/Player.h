/* 
 * File:    Player.h
 * Project: ThePlayerClient
 * Author:  lubos
 *
 * Created on June 18, 2012, 1:41 PM
 */

#ifndef PLAYER_H
#define	PLAYER_H


#include <QtCore/QTimer>
#include <QtCore/QTime>
#include <QtGStreamer/QGst/Pipeline>
#include <QtGStreamer/QGst/Ui/VideoWidget>

#include "MediaApp.h"

class Player : public QGst::Ui::VideoWidget
{
    Q_OBJECT
public:    
    Player(QWidget *parent = 0);
    ~Player();

    void        setUri(const QString & uri);

    QTime       getPosition() const;
    void        setPosition(const QTime & pos);
    int         getVolume() const;

    QTime       getLength() const;
//  QTime       findOutMediaLength(Song* media) const;
    QGst::State getState() const;

public Q_SLOTS:
    void play();
    void pause();
    void stop();    
    void setVolume(int volume);

Q_SIGNALS:
    void positionChanged();
    void stateChanged();

private:
    void onBusMessage(const QGst::MessagePtr & message);
    void handlePipelineStateChange(const QGst::StateChangedMessagePtr & scm);

    QGst::PipelinePtr m_pipeline;
    QTimer            m_positionTimer;    
};


#endif	/* PLAYER_H */

