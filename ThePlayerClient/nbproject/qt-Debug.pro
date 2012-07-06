# This file is generated automatically. Do not edit.
# Use project properties -> Build -> Qt -> Expert -> Custom Definitions.
TEMPLATE = app
DESTDIR = dist/Debug/GNU-Linux-x86
TARGET = ThePlayerClient
VERSION = 1.0.0
CONFIG -= debug_and_release app_bundle lib_bundle
CONFIG += debug 
QT = core gui
SOURCES += StreamMediaClient.cpp Library.cpp main.cpp StreamClient.cpp Playlist.cpp TCPClientSocket.cpp AlbumWidget.cpp Player.cpp MediaApp.cpp CoverDownloader.cpp HttpClientSocket.cpp Logger.cpp
HEADERS += Playlist.h CoverDownloader.h Logger.h MediaApp.h Message.h StreamMediaClient.h Utils.h VolumeSlider.h Thread.h Player.h StreamClient.h Library.h TCPClientSocket.h AlbumWidget.h HttpClientSocket.h Types.h
FORMS +=
RESOURCES +=
TRANSLATIONS +=
OBJECTS_DIR = build/Debug/GNU-Linux-x86
MOC_DIR = 
RCC_DIR = 
UI_DIR = 
QMAKE_CC = gcc
QMAKE_CXX = g++-4.6
DEFINES += 
INCLUDEPATH += /usr/lib/QtGStreamer /usr/include/QtGStreamer 
LIBS += -lQtGLib-2.0 -lQtGStreamer-0.10 -lQtGStreamerUi-0.10 -lQtGStreamerUtils-0.10 -lpthread  
