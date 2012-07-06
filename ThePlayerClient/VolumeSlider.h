/* 
 * File:    VolumeSlider.h
 * Project: ThePlayerClient
 * Author:  lubos
 *
 * Created on July 3, 2012, 7:32 PM
 */

#ifndef VOLUMESLIDER_H
#define	VOLUMESLIDER_H

#include <QtGui/QSlider>
#include <iostream>

/**
 * class derives from QSlider to make work
 * volume settings by arrows key press
 */
class VolumeSlider : public QSlider {    

public: 
    VolumeSlider(QWidget *parent = 0) : QSlider(parent) {}
    VolumeSlider(Qt::Orientation orientation, QWidget *parent = 0) : QSlider(orientation, parent) {}
    
    void moveSlider(int val) {        
        Q_EMIT sliderMoved(val);
    }
};

#endif	/* VOLUMESLIDER_H */

