/* 
 * File:   VolumeSlider.h
 * Author: lubos
 *
 * Created on July 3, 2012, 7:32 PM
 */

#ifndef VOLUMESLIDER_H
#define	VOLUMESLIDER_H

#include <QtGui/QSlider>
#include <iostream>

class VolumeSlider : public QSlider {    

public: 
    VolumeSlider(QWidget *parent = 0) : QSlider(parent) {}
    VolumeSlider(Qt::Orientation orientation, QWidget *parent = 0) : QSlider(orientation, parent) {}
    
    void moveSlider(int val) {        
        emit sliderMoved(val);
    }
};

#endif	/* VOLUMESLIDER_H */

