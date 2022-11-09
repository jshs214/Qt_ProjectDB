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
    uint offsetsAndSizes[30];
    char stringdata0[19];
    char stringdata1[18];
    char stringdata2[1];
    char stringdata3[18];
    char stringdata4[18];
    char stringdata5[23];
    char stringdata6[16];
    char stringdata7[11];
    char stringdata8[25];
    char stringdata9[28];
    char stringdata10[27];
    char stringdata11[28];
    char stringdata12[28];
    char stringdata13[12];
    char stringdata14[6];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_ProductManagerForm_t::offsetsAndSizes) + ofs), len 
static const qt_meta_stringdata_ProductManagerForm_t qt_meta_stringdata_ProductManagerForm = {
    {
        QT_MOC_LITERAL(0, 18),  // "ProductManagerForm"
        QT_MOC_LITERAL(19, 17),  // "productAddToOrder"
        QT_MOC_LITERAL(37, 0),  // ""
        QT_MOC_LITERAL(38, 17),  // "productDelToOrder"
        QT_MOC_LITERAL(56, 17),  // "productModToOrder"
        QT_MOC_LITERAL(74, 22),  // "on_clearButton_clicked"
        QT_MOC_LITERAL(97, 15),  // "showContextMenu"
        QT_MOC_LITERAL(113, 10),  // "removeItem"
        QT_MOC_LITERAL(124, 24),  // "on_addPushButton_clicked"
        QT_MOC_LITERAL(149, 27),  // "on_modifyPushButton_clicked"
        QT_MOC_LITERAL(177, 26),  // "on_statePushButton_clicked"
        QT_MOC_LITERAL(204, 27),  // "on_searchPushButton_clicked"
        QT_MOC_LITERAL(232, 27),  // "on_productTableView_clicked"
        QT_MOC_LITERAL(260, 11),  // "QModelIndex"
        QT_MOC_LITERAL(272, 5)   // "index"
    },
    "ProductManagerForm",
    "productAddToOrder",
    "",
    "productDelToOrder",
    "productModToOrder",
    "on_clearButton_clicked",
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
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    4,   80,    2, 0x06,    1 /* Public */,
       3,    1,   89,    2, 0x06,    6 /* Public */,
       4,    4,   92,    2, 0x06,    8 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       5,    0,  101,    2, 0x08,   13 /* Private */,
       6,    1,  102,    2, 0x08,   14 /* Private */,
       7,    0,  105,    2, 0x08,   16 /* Private */,
       8,    0,  106,    2, 0x08,   17 /* Private */,
       9,    0,  107,    2, 0x08,   18 /* Private */,
      10,    0,  108,    2, 0x08,   19 /* Private */,
      11,    0,  109,    2, 0x08,   20 /* Private */,
      12,    1,  110,    2, 0x08,   21 /* Private */,

 // signals: parameters
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
    QMetaType::Void, 0x80000000 | 13,   14,

       0        // eod
};

void ProductManagerForm::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ProductManagerForm *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->productAddToOrder((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[4]))); break;
        case 1: _t->productDelToOrder((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 2: _t->productModToOrder((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[4]))); break;
        case 3: _t->on_clearButton_clicked(); break;
        case 4: _t->showContextMenu((*reinterpret_cast< std::add_pointer_t<QPoint>>(_a[1]))); break;
        case 5: _t->removeItem(); break;
        case 6: _t->on_addPushButton_clicked(); break;
        case 7: _t->on_modifyPushButton_clicked(); break;
        case 8: _t->on_statePushButton_clicked(); break;
        case 9: _t->on_searchPushButton_clicked(); break;
        case 10: _t->on_productTableView_clicked((*reinterpret_cast< std::add_pointer_t<QModelIndex>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ProductManagerForm::*)(int , QString , QString , QString );
            if (_t _q_method = &ProductManagerForm::productAddToOrder; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ProductManagerForm::*)(int );
            if (_t _q_method = &ProductManagerForm::productDelToOrder; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (ProductManagerForm::*)(int , QString , QString , QString );
            if (_t _q_method = &ProductManagerForm::productModToOrder; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
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
, QtPrivate::TypeAndForceComplete<ProductManagerForm, std::true_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>
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

// SIGNAL 0
void ProductManagerForm::productAddToOrder(int _t1, QString _t2, QString _t3, QString _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t4))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ProductManagerForm::productDelToOrder(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void ProductManagerForm::productModToOrder(int _t1, QString _t2, QString _t3, QString _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t4))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
