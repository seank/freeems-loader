/****************************************************************************
** Meta object code from reading C++ file 'freeems_loader.h'
**
** Created: Thu Jan 15 10:46:10 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../freeems_loader.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'freeems_loader.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_FreeEMS_Loader[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets

       0        // eod
};

static const char qt_meta_stringdata_FreeEMS_Loader[] = {
    "FreeEMS_Loader\0"
};

const QMetaObject FreeEMS_Loader::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_FreeEMS_Loader,
      qt_meta_data_FreeEMS_Loader, 0 }
};

const QMetaObject *FreeEMS_Loader::metaObject() const
{
    return &staticMetaObject;
}

void *FreeEMS_Loader::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FreeEMS_Loader))
	return static_cast<void*>(const_cast< FreeEMS_Loader*>(this));
    return QWidget::qt_metacast(_clname);
}

int FreeEMS_Loader::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
