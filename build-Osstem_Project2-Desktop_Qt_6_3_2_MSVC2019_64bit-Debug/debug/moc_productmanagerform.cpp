/****************************************************************************
** Meta object code from reading C++ file 'productmanagerform.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.3.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../Osstem_Project2/productmanagerform.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'productmanagerform.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ProductManagerForm_t {
    uint offsetsAndSizes[24];
    char stringdata0[19];
    char stringdata1[23];
    char stringdata2[1];
    char stringdata3[16];
    char stringdata4[11];
    char stringdata5[25];
    char stringdata6[28];
    char stringdata7[27];
    char stringdata8[28];
    char stringdata9[28];
    char stringdata10[12];
    char stringdata11[6];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_ProductManagerForm_t::offsetsAndSizes) + ofs), len 
static const qt_meta_stringdata_ProductManagerForm_t qt_meta_stringdata_ProductManagerForm = {
    {
        QT_MOC_LITERAL(0, 18),  // "ProductManagerForm"
        QT_MOC_LITERAL(19, 22),  // "on_clearButton_clicked"
        QT_MOC_LITERAL(42, 0),  // ""
        QT_MOC_LITERAL(43, 15),  // "showContextMenu"
        QT_MOC_LITERAL(59, 10),  // "removeItem"
        QT_MOC_LITERAL(70, 24),  // "on_addPushButton_clicked"
        QT_MOC_LITERAL(95, 27),  // "on_modifyPushButton_clicked"
        QT_MOC_LITERAL(123, 26),  // "on_statePushButton_clicked"
        QT_MOC_LITERAL(150, 27),  // "on_searchPushButton_clicked"
        QT_MOC_LITERAL(178, 27),  // "on_productTableView_clicked"
        QT_MOC_LITERAL(206, 11),  // "QModelIndex"
        QT_MOC_LITERAL(218, 5)   // "index"
    },
    "ProductManagerForm",
    "on_clearButton_clicked",
    "",
    "showContextMenu",
    "removeItem",
    "on_addPushButton_clicked",
    "on_modifyPushButton_clicked",
    "on_statePushButton_clicked",
    "on_searchPushButton_clicked",
    "on_productTableView_clicked",
    "QModelIndex",
    "index"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ProductManagerForm[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   62,    2, 0x08,    1 /* Private */,
       3,    1,   63,    2, 0x08,    2 /* Private */,
       4,    0,   66,    2, 0x08,    4 /* Private */,
       5,    0,   67,    2, 0x08,    5 /* Private */,
       6,    0,   68,    2, 0x08,    6 /* Private */,
       7,    0,   69,    2, 0x08,    7 /* Private */,
       8,    0,   70,    2, 0x08,    8 /* Private */,
       9,    1,   71,    2, 0x08,    9 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QPoint,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 10,   11,

       0        // eod
};

void ProductManagerForm::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ProductManagerForm *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->on_clearButton_clicked(); break;
        case 1: _t->showContextMenu((*reinterpret_cast< std::add_pointer_t<QPoint>>(_a[1]))); break;
        case 2: _t->removeItem(); break;
        case 3: _t->on_addPushButton_clicked(); break;
        case 4: _t->on_modifyPushButton_clicked(); break;
        case 5: _t->on_statePushButton_clicked(); break;
        case 6: _t->on_searchPushButton_clicked(); break;
        case 7: _t->on_productTableView_clicked((*reinterpret_cast< std::add_pointer_t<QModelIndex>>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject ProductManagerForm::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_ProductManagerForm.offsetsAndSizes,
    qt_meta_data_ProductManagerForm,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_ProductManagerForm_t
, QtPrivate::TypeAndForceComplete<ProductManagerForm, std::true_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<const QPoint &, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<const QModelIndex &, std::false_type>


>,
    nullptr
} };


const QMetaObject *ProductManagerForm::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ProductManagerForm::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ProductManagerForm.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int ProductManagerForm::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 8;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
