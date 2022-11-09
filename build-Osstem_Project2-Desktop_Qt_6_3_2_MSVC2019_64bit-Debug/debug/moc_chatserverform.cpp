/****************************************************************************
** Meta object code from reading C++ file 'chatserverform.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.3.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../Osstem_Project2/chatserverform.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'chatserverform.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ChatServerForm_t {
    uint offsetsAndSizes[28];
    char stringdata0[15];
    char stringdata1[17];
    char stringdata2[1];
    char stringdata3[11];
    char stringdata4[14];
    char stringdata5[12];
    char stringdata6[13];
    char stringdata7[10];
    char stringdata8[13];
    char stringdata9[8];
    char stringdata10[47];
    char stringdata11[4];
    char stringdata12[10];
    char stringdata13[22];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_ChatServerForm_t::offsetsAndSizes) + ofs), len 
static const qt_meta_stringdata_ChatServerForm_t qt_meta_stringdata_ChatServerForm = {
    {
        QT_MOC_LITERAL(0, 14),  // "ChatServerForm"
        QT_MOC_LITERAL(15, 16),  // "acceptConnection"
        QT_MOC_LITERAL(32, 0),  // ""
        QT_MOC_LITERAL(33, 10),  // "readClient"
        QT_MOC_LITERAL(44, 13),  // "clientConnect"
        QT_MOC_LITERAL(58, 11),  // "receiveData"
        QT_MOC_LITERAL(70, 12),  // "removeClient"
        QT_MOC_LITERAL(83, 9),  // "addClient"
        QT_MOC_LITERAL(93, 12),  // "inviteClient"
        QT_MOC_LITERAL(106, 7),  // "kickOut"
        QT_MOC_LITERAL(114, 46),  // "on_clientTreeWidget_customCon..."
        QT_MOC_LITERAL(161, 3),  // "pos"
        QT_MOC_LITERAL(165, 9),  // "remClient"
        QT_MOC_LITERAL(175, 21)   // "on_sendButton_clicked"
    },
    "ChatServerForm",
    "acceptConnection",
    "",
    "readClient",
    "clientConnect",
    "receiveData",
    "removeClient",
    "addClient",
    "inviteClient",
    "kickOut",
    "on_clientTreeWidget_customContextMenuRequested",
    "pos",
    "remClient",
    "on_sendButton_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ChatServerForm[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   80,    2, 0x08,    1 /* Private */,
       3,    0,   81,    2, 0x08,    2 /* Private */,
       4,    0,   82,    2, 0x08,    3 /* Private */,
       5,    0,   83,    2, 0x08,    4 /* Private */,
       6,    0,   84,    2, 0x08,    5 /* Private */,
       7,    2,   85,    2, 0x08,    6 /* Private */,
       8,    0,   90,    2, 0x08,    9 /* Private */,
       9,    0,   91,    2, 0x08,   10 /* Private */,
      10,    1,   92,    2, 0x08,   11 /* Private */,
      12,    1,   95,    2, 0x08,   13 /* Private */,
      13,    0,   98,    2, 0x08,   15 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::QString,    2,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QPoint,   11,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,

       0        // eod
};

void ChatServerForm::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ChatServerForm *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->acceptConnection(); break;
        case 1: _t->readClient(); break;
        case 2: _t->clientConnect(); break;
        case 3: _t->receiveData(); break;
        case 4: _t->removeClient(); break;
        case 5: _t->addClient((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 6: _t->inviteClient(); break;
        case 7: _t->kickOut(); break;
        case 8: _t->on_clientTreeWidget_customContextMenuRequested((*reinterpret_cast< std::add_pointer_t<QPoint>>(_a[1]))); break;
        case 9: _t->remClient((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 10: _t->on_sendButton_clicked(); break;
        default: ;
        }
    }
}

const QMetaObject ChatServerForm::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_ChatServerForm.offsetsAndSizes,
    qt_meta_data_ChatServerForm,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_ChatServerForm_t
, QtPrivate::TypeAndForceComplete<ChatServerForm, std::true_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<const QPoint &, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>


>,
    nullptr
} };


const QMetaObject *ChatServerForm::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ChatServerForm::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ChatServerForm.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int ChatServerForm::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 11;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
