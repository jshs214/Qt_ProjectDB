/****************************************************************************
** Meta object code from reading C++ file 'clientmanagerform.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.3.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../Osstem_Project2/clientmanagerform.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'clientmanagerform.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ClientManagerForm_t {
    uint offsetsAndSizes[32];
    char stringdata0[18];
    char stringdata1[18];
    char stringdata2[1];
    char stringdata3[17];
    char stringdata4[17];
    char stringdata5[17];
    char stringdata6[23];
    char stringdata7[16];
    char stringdata8[25];
    char stringdata9[28];
    char stringdata10[11];
    char stringdata11[28];
    char stringdata12[27];
    char stringdata13[27];
    char stringdata14[12];
    char stringdata15[6];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_ClientManagerForm_t::offsetsAndSizes) + ofs), len 
static const qt_meta_stringdata_ClientManagerForm_t qt_meta_stringdata_ClientManagerForm = {
    {
        QT_MOC_LITERAL(0, 17),  // "ClientManagerForm"
        QT_MOC_LITERAL(18, 17),  // "clientAddToServer"
        QT_MOC_LITERAL(36, 0),  // ""
        QT_MOC_LITERAL(37, 16),  // "clientAddToOrder"
        QT_MOC_LITERAL(54, 16),  // "clientDelToOrder"
        QT_MOC_LITERAL(71, 16),  // "clientModToOrder"
        QT_MOC_LITERAL(88, 22),  // "on_clearButton_clicked"
        QT_MOC_LITERAL(111, 15),  // "showContextMenu"
        QT_MOC_LITERAL(127, 24),  // "on_addPushButton_clicked"
        QT_MOC_LITERAL(152, 27),  // "on_modifyPushButton_clicked"
        QT_MOC_LITERAL(180, 10),  // "removeItem"
        QT_MOC_LITERAL(191, 27),  // "on_searchPushButton_clicked"
        QT_MOC_LITERAL(219, 26),  // "on_statePushButton_clicked"
        QT_MOC_LITERAL(246, 26),  // "on_clientTableView_clicked"
        QT_MOC_LITERAL(273, 11),  // "QModelIndex"
        QT_MOC_LITERAL(285, 5)   // "index"
    },
    "ClientManagerForm",
    "clientAddToServer",
    "",
    "clientAddToOrder",
    "clientDelToOrder",
    "clientModToOrder",
    "on_clearButton_clicked",
    "showContextMenu",
    "on_addPushButton_clicked",
    "on_modifyPushButton_clicked",
    "removeItem",
    "on_searchPushButton_clicked",
    "on_statePushButton_clicked",
    "on_clientTableView_clicked",
    "QModelIndex",
    "index"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ClientManagerForm[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    2,   86,    2, 0x06,    1 /* Public */,
       3,    4,   91,    2, 0x06,    4 /* Public */,
       4,    1,  100,    2, 0x06,    9 /* Public */,
       5,    4,  103,    2, 0x06,   11 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       6,    0,  112,    2, 0x08,   16 /* Private */,
       7,    1,  113,    2, 0x08,   17 /* Private */,
       8,    0,  116,    2, 0x08,   19 /* Private */,
       9,    0,  117,    2, 0x08,   20 /* Private */,
      10,    0,  118,    2, 0x08,   21 /* Private */,
      11,    0,  119,    2, 0x08,   22 /* Private */,
      12,    0,  120,    2, 0x08,   23 /* Private */,
      13,    1,  121,    2, 0x08,   24 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::QString,    2,    2,
    QMetaType::Void, QMetaType::Int, QMetaType::QString, QMetaType::QString, QMetaType::QString,    2,    2,    2,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int, QMetaType::QString, QMetaType::QString, QMetaType::QString,    2,    2,    2,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QPoint,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 14,   15,

       0        // eod
};

void ClientManagerForm::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ClientManagerForm *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->clientAddToServer((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 1: _t->clientAddToOrder((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[4]))); break;
        case 2: _t->clientDelToOrder((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 3: _t->clientModToOrder((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[4]))); break;
        case 4: _t->on_clearButton_clicked(); break;
        case 5: _t->showContextMenu((*reinterpret_cast< std::add_pointer_t<QPoint>>(_a[1]))); break;
        case 6: _t->on_addPushButton_clicked(); break;
        case 7: _t->on_modifyPushButton_clicked(); break;
        case 8: _t->removeItem(); break;
        case 9: _t->on_searchPushButton_clicked(); break;
        case 10: _t->on_statePushButton_clicked(); break;
        case 11: _t->on_clientTableView_clicked((*reinterpret_cast< std::add_pointer_t<QModelIndex>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ClientManagerForm::*)(int , QString );
            if (_t _q_method = &ClientManagerForm::clientAddToServer; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ClientManagerForm::*)(int , QString , QString , QString );
            if (_t _q_method = &ClientManagerForm::clientAddToOrder; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (ClientManagerForm::*)(int );
            if (_t _q_method = &ClientManagerForm::clientDelToOrder; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (ClientManagerForm::*)(int , QString , QString , QString );
            if (_t _q_method = &ClientManagerForm::clientModToOrder; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
    }
}

const QMetaObject ClientManagerForm::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_ClientManagerForm.offsetsAndSizes,
    qt_meta_data_ClientManagerForm,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_ClientManagerForm_t
, QtPrivate::TypeAndForceComplete<ClientManagerForm, std::true_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<const QPoint &, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<const QModelIndex &, std::false_type>


>,
    nullptr
} };


const QMetaObject *ClientManagerForm::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ClientManagerForm::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ClientManagerForm.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int ClientManagerForm::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void ClientManagerForm::clientAddToServer(int _t1, QString _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ClientManagerForm::clientAddToOrder(int _t1, QString _t2, QString _t3, QString _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t4))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void ClientManagerForm::clientDelToOrder(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void ClientManagerForm::clientModToOrder(int _t1, QString _t2, QString _t3, QString _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t4))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
