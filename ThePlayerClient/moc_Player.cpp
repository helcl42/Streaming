/****************************************************************************
** Meta object code from reading C++ file 'Player.h'
**
** Created: Wed Jul 4 00:57:05 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "Player.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Player.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Player[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
       8,    7,    7,    7, 0x05,
      26,    7,    7,    7, 0x05,

 // slots: signature, parameters, type, tag, flags
      41,    7,    7,    7, 0x0a,
      48,    7,    7,    7, 0x0a,
      56,    7,    7,    7, 0x0a,
      70,   63,    7,    7, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Player[] = {
    "Player\0\0positionChanged()\0stateChanged()\0"
    "play()\0pause()\0stop()\0volume\0"
    "setVolume(int)\0"
};

void Player::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Player *_t = static_cast<Player *>(_o);
        switch (_id) {
        case 0: _t->positionChanged(); break;
        case 1: _t->stateChanged(); break;
        case 2: _t->play(); break;
        case 3: _t->pause(); break;
        case 4: _t->stop(); break;
        case 5: _t->setVolume((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Player::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Player::staticMetaObject = {
    { &QGst::Ui::VideoWidget::staticMetaObject, qt_meta_stringdata_Player,
      qt_meta_data_Player, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Player::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Player::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Player::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Player))
        return static_cast<void*>(const_cast< Player*>(this));
    typedef QGst::Ui::VideoWidget QMocSuperClass;
    return QMocSuperClass::qt_metacast(_clname);
}

int Player::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    typedef QGst::Ui::VideoWidget QMocSuperClass;
    _id = QMocSuperClass::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void Player::positionChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void Player::stateChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
QT_END_MOC_NAMESPACE
