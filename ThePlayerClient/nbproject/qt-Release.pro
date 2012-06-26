# This file is generated automatically. Do not edit.
# Use project properties -> Build -> Qt -> Expert -> Custom Definitions.
TEMPLATE = app
DESTDIR = dist/Release/GNU-Linux-x86
TARGET = ThePlayerClient
VERSION = 1.0.0
CONFIG -= debug_and_release app_bundle lib_bundle
CONFIG += release 
QT = core gui
SOURCES += StreamMediaClient.cpp Library.cpp main.cpp StreamClient.cpp Playlist.cpp TCPClientSocket.cpp AlbumWidget.cpp Player.cpp MediaApp.cpp CoverDownloader.cpp HttpClientSocket.cpp Logger.cpp
HEADERS += Playlist.h CoverDownloader.h Logger.h MediaApp.h Message.h StreamMediaClient.h Utils.h Thread.h StreamClient.h Player.h Library.h TCPClientSocket.h AlbumWidget.h HttpClientSocket.h Types.h
FORMS +=
RESOURCES +=
TRANSLATIONS +=
OBJECTS_DIR = build/Release/GNU-Linux-x86
MOC_DIR = 
RCC_DIR = 
UI_DIR = 
QMAKE_CC = gcc
QMAKE_CXX = g++-4.6
DEFINES += 
INCLUDEPATH += 
LIBS += 