#############################################################################
# Makefile for building: dist/Release/GNU-Linux-x86/ThePlayerClient
# Generated by qmake (2.01a) (Qt 4.8.1) on: Wed Jun 27 01:05:19 2012
# Project:  nbproject/qt-Release.pro
# Template: app
# Command: /usr/bin/qmake-qt4 VPATH=. -o qttmp-Release.mk nbproject/qt-Release.pro
#############################################################################

####### Compiler, tools and options

CC            = gcc
CXX           = g++-4.6
DEFINES       = -DQT_WEBKIT -DQT_NO_DEBUG -DQT_GUI_LIB -DQT_CORE_LIB -DQT_SHARED
CFLAGS        = -m64 -pipe -O2 -Wall -W -D_REENTRANT $(DEFINES)
CXXFLAGS      = -m64 -pipe -O2 -Wall -W -D_REENTRANT $(DEFINES)
INCPATH       = -I/usr/share/qt4/mkspecs/linux-g++-64 -Inbproject -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtGui -I/usr/include/qt4 -I. -Inbproject -I.
LINK          = g++
LFLAGS        = -m64 -Wl,-O1
LIBS          = $(SUBLIBS)  -L/usr/lib/x86_64-linux-gnu -lQtGui -lQtCore -lpthread 
AR            = ar cqs
RANLIB        = 
QMAKE         = /usr/bin/qmake-qt4
TAR           = tar -cf
COMPRESS      = gzip -9f
COPY          = cp -f
SED           = sed
COPY_FILE     = $(COPY)
COPY_DIR      = $(COPY) -r
STRIP         = strip
INSTALL_FILE  = install -m 644 -p
INSTALL_DIR   = $(COPY_DIR)
INSTALL_PROGRAM = install -m 755 -p
DEL_FILE      = rm -f
SYMLINK       = ln -f -s
DEL_DIR       = rmdir
MOVE          = mv -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p

####### Output directory

OBJECTS_DIR   = build/Release/GNU-Linux-x86/

####### Files

SOURCES       = StreamMediaClient.cpp \
		Library.cpp \
		main.cpp \
		StreamClient.cpp \
		Playlist.cpp \
		TCPClientSocket.cpp \
		AlbumWidget.cpp \
		Player.cpp \
		MediaApp.cpp \
		CoverDownloader.cpp \
		HttpClientSocket.cpp \
		Logger.cpp moc_MediaApp.cpp \
		moc_Player.cpp \
		moc_Library.cpp
OBJECTS       = build/Release/GNU-Linux-x86/StreamMediaClient.o \
		build/Release/GNU-Linux-x86/Library.o \
		build/Release/GNU-Linux-x86/main.o \
		build/Release/GNU-Linux-x86/StreamClient.o \
		build/Release/GNU-Linux-x86/Playlist.o \
		build/Release/GNU-Linux-x86/TCPClientSocket.o \
		build/Release/GNU-Linux-x86/AlbumWidget.o \
		build/Release/GNU-Linux-x86/Player.o \
		build/Release/GNU-Linux-x86/MediaApp.o \
		build/Release/GNU-Linux-x86/CoverDownloader.o \
		build/Release/GNU-Linux-x86/HttpClientSocket.o \
		build/Release/GNU-Linux-x86/Logger.o \
		build/Release/GNU-Linux-x86/moc_MediaApp.o \
		build/Release/GNU-Linux-x86/moc_Player.o \
		build/Release/GNU-Linux-x86/moc_Library.o
DIST          = /usr/share/qt4/mkspecs/common/unix.conf \
		/usr/share/qt4/mkspecs/common/linux.conf \
		/usr/share/qt4/mkspecs/common/gcc-base.conf \
		/usr/share/qt4/mkspecs/common/gcc-base-unix.conf \
		/usr/share/qt4/mkspecs/common/g++-base.conf \
		/usr/share/qt4/mkspecs/common/g++-unix.conf \
		/usr/share/qt4/mkspecs/qconfig.pri \
		/usr/share/qt4/mkspecs/modules/qt_webkit_version.pri \
		/usr/share/qt4/mkspecs/features/qt_functions.prf \
		/usr/share/qt4/mkspecs/features/qt_config.prf \
		/usr/share/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/share/qt4/mkspecs/features/default_pre.prf \
		/usr/share/qt4/mkspecs/features/release.prf \
		/usr/share/qt4/mkspecs/features/default_post.prf \
		/usr/share/qt4/mkspecs/features/unix/gdb_dwarf_index.prf \
		/usr/share/qt4/mkspecs/features/warn_on.prf \
		/usr/share/qt4/mkspecs/features/qt.prf \
		/usr/share/qt4/mkspecs/features/unix/thread.prf \
		/usr/share/qt4/mkspecs/features/moc.prf \
		/usr/share/qt4/mkspecs/features/resources.prf \
		/usr/share/qt4/mkspecs/features/uic.prf \
		/usr/share/qt4/mkspecs/features/yacc.prf \
		/usr/share/qt4/mkspecs/features/lex.prf \
		/usr/share/qt4/mkspecs/features/include_source_dir.prf \
		nbproject/qt-Release.pro
QMAKE_TARGET  = ThePlayerClient
DESTDIR       = dist/Release/GNU-Linux-x86/
TARGET        = dist/Release/GNU-Linux-x86/ThePlayerClient

first: all
####### Implicit rules

.SUFFIXES: .o .c .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o "$@" "$<"

####### Build rules

all: qttmp-Release.mk $(TARGET)

$(TARGET):  $(OBJECTS)  
	@$(CHK_DIR_EXISTS) dist/Release/GNU-Linux-x86/ || $(MKDIR) dist/Release/GNU-Linux-x86/ 
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJCOMP) $(LIBS)

qttmp-Release.mk: nbproject/qt-Release.pro  /usr/share/qt4/mkspecs/linux-g++-64/qmake.conf /usr/share/qt4/mkspecs/common/unix.conf \
		/usr/share/qt4/mkspecs/common/linux.conf \
		/usr/share/qt4/mkspecs/common/gcc-base.conf \
		/usr/share/qt4/mkspecs/common/gcc-base-unix.conf \
		/usr/share/qt4/mkspecs/common/g++-base.conf \
		/usr/share/qt4/mkspecs/common/g++-unix.conf \
		/usr/share/qt4/mkspecs/qconfig.pri \
		/usr/share/qt4/mkspecs/modules/qt_webkit_version.pri \
		/usr/share/qt4/mkspecs/features/qt_functions.prf \
		/usr/share/qt4/mkspecs/features/qt_config.prf \
		/usr/share/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/share/qt4/mkspecs/features/default_pre.prf \
		/usr/share/qt4/mkspecs/features/release.prf \
		/usr/share/qt4/mkspecs/features/default_post.prf \
		/usr/share/qt4/mkspecs/features/unix/gdb_dwarf_index.prf \
		/usr/share/qt4/mkspecs/features/warn_on.prf \
		/usr/share/qt4/mkspecs/features/qt.prf \
		/usr/share/qt4/mkspecs/features/unix/thread.prf \
		/usr/share/qt4/mkspecs/features/moc.prf \
		/usr/share/qt4/mkspecs/features/resources.prf \
		/usr/share/qt4/mkspecs/features/uic.prf \
		/usr/share/qt4/mkspecs/features/yacc.prf \
		/usr/share/qt4/mkspecs/features/lex.prf \
		/usr/share/qt4/mkspecs/features/include_source_dir.prf \
		/usr/lib/x86_64-linux-gnu/libQtGui.prl \
		/usr/lib/x86_64-linux-gnu/libQtCore.prl
	$(QMAKE) VPATH=. -o qttmp-Release.mk nbproject/qt-Release.pro
/usr/share/qt4/mkspecs/common/unix.conf:
/usr/share/qt4/mkspecs/common/linux.conf:
/usr/share/qt4/mkspecs/common/gcc-base.conf:
/usr/share/qt4/mkspecs/common/gcc-base-unix.conf:
/usr/share/qt4/mkspecs/common/g++-base.conf:
/usr/share/qt4/mkspecs/common/g++-unix.conf:
/usr/share/qt4/mkspecs/qconfig.pri:
/usr/share/qt4/mkspecs/modules/qt_webkit_version.pri:
/usr/share/qt4/mkspecs/features/qt_functions.prf:
/usr/share/qt4/mkspecs/features/qt_config.prf:
/usr/share/qt4/mkspecs/features/exclusive_builds.prf:
/usr/share/qt4/mkspecs/features/default_pre.prf:
/usr/share/qt4/mkspecs/features/release.prf:
/usr/share/qt4/mkspecs/features/default_post.prf:
/usr/share/qt4/mkspecs/features/unix/gdb_dwarf_index.prf:
/usr/share/qt4/mkspecs/features/warn_on.prf:
/usr/share/qt4/mkspecs/features/qt.prf:
/usr/share/qt4/mkspecs/features/unix/thread.prf:
/usr/share/qt4/mkspecs/features/moc.prf:
/usr/share/qt4/mkspecs/features/resources.prf:
/usr/share/qt4/mkspecs/features/uic.prf:
/usr/share/qt4/mkspecs/features/yacc.prf:
/usr/share/qt4/mkspecs/features/lex.prf:
/usr/share/qt4/mkspecs/features/include_source_dir.prf:
/usr/lib/x86_64-linux-gnu/libQtGui.prl:
/usr/lib/x86_64-linux-gnu/libQtCore.prl:
qmake:  FORCE
	@$(QMAKE) VPATH=. -o qttmp-Release.mk nbproject/qt-Release.pro

dist: 
	@$(CHK_DIR_EXISTS) build/Release/GNU-Linux-x86/ThePlayerClient1.0.0 || $(MKDIR) build/Release/GNU-Linux-x86/ThePlayerClient1.0.0 
	$(COPY_FILE) --parents $(SOURCES) $(DIST) build/Release/GNU-Linux-x86/ThePlayerClient1.0.0/ && $(COPY_FILE) --parents Playlist.h CoverDownloader.h Logger.h MediaApp.h Message.h StreamMediaClient.h Utils.h Thread.h StreamClient.h Player.h Library.h TCPClientSocket.h AlbumWidget.h HttpClientSocket.h Types.h build/Release/GNU-Linux-x86/ThePlayerClient1.0.0/ && $(COPY_FILE) --parents StreamMediaClient.cpp Library.cpp main.cpp StreamClient.cpp Playlist.cpp TCPClientSocket.cpp AlbumWidget.cpp Player.cpp MediaApp.cpp CoverDownloader.cpp HttpClientSocket.cpp Logger.cpp build/Release/GNU-Linux-x86/ThePlayerClient1.0.0/ && (cd `dirname build/Release/GNU-Linux-x86/ThePlayerClient1.0.0` && $(TAR) ThePlayerClient1.0.0.tar ThePlayerClient1.0.0 && $(COMPRESS) ThePlayerClient1.0.0.tar) && $(MOVE) `dirname build/Release/GNU-Linux-x86/ThePlayerClient1.0.0`/ThePlayerClient1.0.0.tar.gz . && $(DEL_FILE) -r build/Release/GNU-Linux-x86/ThePlayerClient1.0.0


clean:compiler_clean 
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core


####### Sub-libraries

distclean: clean
	-$(DEL_FILE) $(TARGET) 
	-$(DEL_FILE) qttmp-Release.mk


check: first

mocclean: compiler_moc_header_clean compiler_moc_source_clean

mocables: compiler_moc_header_make_all compiler_moc_source_make_all

compiler_moc_header_make_all: moc_MediaApp.cpp moc_Player.cpp moc_Library.cpp
compiler_moc_header_clean:
	-$(DEL_FILE) moc_MediaApp.cpp moc_Player.cpp moc_Library.cpp
moc_MediaApp.cpp: Library.h \
		Utils.h \
		Types.h \
		Playlist.h \
		MediaApp.h \
		AlbumWidget.h \
		CoverDownloader.h \
		TCPClientSocket.h \
		Logger.h \
		Message.h \
		Thread.h \
		StreamMediaClient.h \
		StreamClient.h \
		MediaApp.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) MediaApp.h -o moc_MediaApp.cpp

moc_Player.cpp: MediaApp.h \
		Library.h \
		Utils.h \
		Types.h \
		Playlist.h \
		StreamMediaClient.h \
		StreamClient.h \
		TCPClientSocket.h \
		Logger.h \
		Message.h \
		Thread.h \
		AlbumWidget.h \
		CoverDownloader.h \
		Player.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) Player.h -o moc_Player.cpp

moc_Library.cpp: Utils.h \
		Types.h \
		Playlist.h \
		MediaApp.h \
		Library.h \
		StreamMediaClient.h \
		StreamClient.h \
		TCPClientSocket.h \
		Logger.h \
		Message.h \
		Thread.h \
		AlbumWidget.h \
		CoverDownloader.h \
		Library.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) Library.h -o moc_Library.cpp

compiler_rcc_make_all:
compiler_rcc_clean:
compiler_image_collection_make_all: qmake_image_collection.cpp
compiler_image_collection_clean:
	-$(DEL_FILE) qmake_image_collection.cpp
compiler_moc_source_make_all:
compiler_moc_source_clean:
compiler_uic_make_all:
compiler_uic_clean:
compiler_yacc_decl_make_all:
compiler_yacc_decl_clean:
compiler_yacc_impl_make_all:
compiler_yacc_impl_clean:
compiler_lex_make_all:
compiler_lex_clean:
compiler_clean: compiler_moc_header_clean 

####### Compile

build/Release/GNU-Linux-x86/StreamMediaClient.o: StreamMediaClient.cpp StreamMediaClient.h \
		StreamClient.h \
		Utils.h \
		TCPClientSocket.h \
		Logger.h \
		Message.h \
		Types.h \
		Thread.h \
		Library.h \
		Playlist.h \
		MediaApp.h \
		AlbumWidget.h \
		CoverDownloader.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/Release/GNU-Linux-x86/StreamMediaClient.o StreamMediaClient.cpp

build/Release/GNU-Linux-x86/Library.o: Library.cpp Library.h \
		Utils.h \
		Types.h \
		Playlist.h \
		MediaApp.h \
		AlbumWidget.h \
		CoverDownloader.h \
		TCPClientSocket.h \
		Logger.h \
		Message.h \
		Thread.h \
		StreamMediaClient.h \
		StreamClient.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/Release/GNU-Linux-x86/Library.o Library.cpp

build/Release/GNU-Linux-x86/main.o: main.cpp StreamClient.h \
		Utils.h \
		TCPClientSocket.h \
		Logger.h \
		Message.h \
		Types.h \
		Thread.h \
		MediaApp.h \
		Library.h \
		Playlist.h \
		StreamMediaClient.h \
		AlbumWidget.h \
		CoverDownloader.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/Release/GNU-Linux-x86/main.o main.cpp

build/Release/GNU-Linux-x86/StreamClient.o: StreamClient.cpp StreamClient.h \
		Utils.h \
		TCPClientSocket.h \
		Logger.h \
		Message.h \
		Types.h \
		Thread.h \
		Library.h \
		Playlist.h \
		MediaApp.h \
		AlbumWidget.h \
		CoverDownloader.h \
		StreamMediaClient.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/Release/GNU-Linux-x86/StreamClient.o StreamClient.cpp

build/Release/GNU-Linux-x86/Playlist.o: Playlist.cpp Playlist.h \
		Types.h \
		Utils.h \
		MediaApp.h \
		Library.h \
		StreamMediaClient.h \
		StreamClient.h \
		TCPClientSocket.h \
		Logger.h \
		Message.h \
		Thread.h \
		AlbumWidget.h \
		CoverDownloader.h \
		Player.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/Release/GNU-Linux-x86/Playlist.o Playlist.cpp

build/Release/GNU-Linux-x86/TCPClientSocket.o: TCPClientSocket.cpp TCPClientSocket.h \
		Utils.h \
		Logger.h \
		Message.h \
		Types.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/Release/GNU-Linux-x86/TCPClientSocket.o TCPClientSocket.cpp

build/Release/GNU-Linux-x86/AlbumWidget.o: AlbumWidget.cpp AlbumWidget.h \
		CoverDownloader.h \
		TCPClientSocket.h \
		Utils.h \
		Logger.h \
		Message.h \
		Types.h \
		Thread.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/Release/GNU-Linux-x86/AlbumWidget.o AlbumWidget.cpp

build/Release/GNU-Linux-x86/Player.o: Player.cpp Player.h \
		MediaApp.h \
		Library.h \
		Utils.h \
		Types.h \
		Playlist.h \
		StreamMediaClient.h \
		StreamClient.h \
		TCPClientSocket.h \
		Logger.h \
		Message.h \
		Thread.h \
		AlbumWidget.h \
		CoverDownloader.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/Release/GNU-Linux-x86/Player.o Player.cpp

build/Release/GNU-Linux-x86/MediaApp.o: MediaApp.cpp MediaApp.h \
		Library.h \
		Utils.h \
		Types.h \
		Playlist.h \
		StreamMediaClient.h \
		StreamClient.h \
		TCPClientSocket.h \
		Logger.h \
		Message.h \
		Thread.h \
		AlbumWidget.h \
		CoverDownloader.h \
		Player.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/Release/GNU-Linux-x86/MediaApp.o MediaApp.cpp

build/Release/GNU-Linux-x86/CoverDownloader.o: CoverDownloader.cpp CoverDownloader.h \
		TCPClientSocket.h \
		Utils.h \
		Logger.h \
		Message.h \
		Types.h \
		Thread.h \
		StreamClient.h \
		HttpClientSocket.h \
		MediaApp.h \
		Library.h \
		Playlist.h \
		StreamMediaClient.h \
		AlbumWidget.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/Release/GNU-Linux-x86/CoverDownloader.o CoverDownloader.cpp

build/Release/GNU-Linux-x86/HttpClientSocket.o: HttpClientSocket.cpp HttpClientSocket.h \
		TCPClientSocket.h \
		Utils.h \
		Logger.h \
		Message.h \
		Types.h \
		StreamClient.h \
		Thread.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/Release/GNU-Linux-x86/HttpClientSocket.o HttpClientSocket.cpp

build/Release/GNU-Linux-x86/Logger.o: Logger.cpp Logger.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/Release/GNU-Linux-x86/Logger.o Logger.cpp

build/Release/GNU-Linux-x86/moc_MediaApp.o: moc_MediaApp.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/Release/GNU-Linux-x86/moc_MediaApp.o moc_MediaApp.cpp

build/Release/GNU-Linux-x86/moc_Player.o: moc_Player.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/Release/GNU-Linux-x86/moc_Player.o moc_Player.cpp

build/Release/GNU-Linux-x86/moc_Library.o: moc_Library.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/Release/GNU-Linux-x86/moc_Library.o moc_Library.cpp

####### Install

install:   FORCE

uninstall:   FORCE

FORCE:

