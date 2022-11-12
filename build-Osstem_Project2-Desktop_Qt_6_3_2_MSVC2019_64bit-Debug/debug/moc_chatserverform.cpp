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
    uint offsetsAndSizes[30];
    char stringdata0[15];
    char stringdata1[10];
    char stringdata2[1];
    char stringdata3[10];
    char stringdata4[10];
    char stringdata5[14];
    char stringdata6[12];
    char stringdata7[13];
    char stringdata8[47];
    char stringdata9[4];
    char stringdata10[13];
    char stringdata11[8];
    char stringdata12[17];
    char stringdata13[11];
    char stringdata14[22];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_ChatServerForm_t::offsetsAndSizes) + ofs), len 
static const qt_meta_stringdata_ChatServerForm_t qt_meta_stringdata_ChatServerForm = {
    {
        QT_MOC_LITERAL(0, 14),  // "ChatServerForm"
        QT_MOC_LITERAL(15, 9),  // "addClient"
        QT_MOC_LITERAL(25, 0),  // ""
        QT_MOC_LITERAL(26, 9),  // "modClient"
        QT_MOC_LITERAL(36, 9),  // "remClient"
        QT_MOC_LITERAL(46, 13),  // "clientConnect"
        QT_MOC_LITERAL(60, 11),  // "receiveData"
        QT_MOC_LITERAL(72, 12),  // "removeClient"
        QT_MOC_LITERAL(85, 46),  // "on_clientTreeWidget_customCon..."
        QT_MOC_LITERAL(132, 3),  // "pos"
        QT_MOC_LITERAL(136, 12),  // "inviteClient"
        QT_MOC_LITERAL(149, 7),  // "kickOut"
        QT_MOC_LITERAL(157, 16),  // "acceptConnection"
        QT_MOC_LITERAL(174, 10),  // "readClient"
        QT_MOC_LITERAL(185, 21)   // "on_sendButton_clicked"
    },
    "ChatServerForm",
    "addClient",
    "",
    "modClient",
    "remClient",
    "clientConnect",
    "receiveData",
    "removeClient",
    "on_clientTreeWidget_customContextMenuRequested",
    "pos",
    "inviteClient",
    "kickOut",
    "acceptConnection",
    "readClient",
    "on_sendButton_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ChatServerForm[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    2,   86,    2, 0x08,    1 /* Private */,
       3,    2,   91,    2, 0x08,    4 /* Private */,
       4,    1,   96,    2, 0x08,    7 /* Private */,
       5,    0,   99,    2, 0x08,    9 /* Private */,
       6,    0,  100,    2, 0x08,   10 /* Private */,
       7,    0,  101,    2, 0x08,   11 /* Private */,
       8,    1,  102,    2, 0x08,   12 /* Private */,
      10,    0,  105,    2, 0x08,   14 /* Private */,
      11,    0,  106,    2, 0x08,   15 /* Private */,
      12,    0,  107,    2, 0x08,   16 /* Private */,
      13,    0,  108,    2, 0x08,   17 /* Private */,
      14,    0,  109,    2, 0x08,   18 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::QString,    2,    2,
    QMetaType::Void, QMetaType::Int, QMetaType::QString,    2,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QPoint,    9,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void ChatServerForm::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ChatServerForm *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->addClient((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 1: _t->modClient((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 2: _t->remClient((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 3: _t->clientConnect(); break;
        case 4: _t->receiveData(); break;
        case 5: _t->removeClient(); break;
        case 6: _t->on_clientTreeWidget_customContextMenuRequested((*reinterpret_cast< std::add_pointer_t<QPoint>>(_a[1]))); break;
        case 7: _t->inviteClient(); break;
        case 8: _t->kickOut(); break;
        case 9: _t->acceptConnection(); break;
        case 10: _t->readClient(); break;
        case 11: _t->on_sendButton_clicked(); break;
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
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<const QPoint &, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>


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
QT_WARNING_POP
QT_END_MOC_NAMESPACE
