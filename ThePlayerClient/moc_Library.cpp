/****************************************************************************
** Meta object code from reading C++ file 'Library.h'
**
** Created: Wed Jul 11 11:31:56 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "Library.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Library.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Library[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
       9,    8,    8,    8, 0x08,
      20,   18,    8,    8, 0x08,
      71,   66,    8,    8, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Library[] = {
    "Library\0\0search()\0,\0"
    "selectFromTree(QItemSelection,QItemSelection)\0"
    "song\0download(Song*)\0"
};

void Library::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Library *_t = static_cast<Library *>(_o);
        switch (_id) {
        case 0: _t->search(); break;
        case 1: _t->selectFromTree((*reinterpret_cast< const QItemSelection(*)>(_a[1])),(*reinterpret_cast< const QItemSelection(*)>(_a[2]))); break;
        case 2: _t->download((*reinterpret_cast< Song*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Library::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Library::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Library,
      qt_meta_data_Library, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Library::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Library::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Library::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Library))
        return static_cast<void*>(const_cast< Library*>(this));
    return QWidget::qt_metacast(_clname);
}

int Library::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
