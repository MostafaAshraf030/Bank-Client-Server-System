/****************************************************************************
** Meta object code from reading C++ file 'client.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.5.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../Client2/client.h"
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'client.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.5.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSCmyClientENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSCmyClientENDCLASS = QtMocHelpers::stringData(
    "CmyClient",
    "performOperation",
    "",
    "operation",
    "disconnect",
    "connectToHost",
    "host",
    "port",
    "connected",
    "disconnected",
    "error",
    "QAbstractSocket::SocketError",
    "socketError",
    "stateChanged",
    "QAbstractSocket::SocketState",
    "socketState",
    "readyRead"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSCmyClientENDCLASS_t {
    uint offsetsAndSizes[34];
    char stringdata0[10];
    char stringdata1[17];
    char stringdata2[1];
    char stringdata3[10];
    char stringdata4[11];
    char stringdata5[14];
    char stringdata6[5];
    char stringdata7[5];
    char stringdata8[10];
    char stringdata9[13];
    char stringdata10[6];
    char stringdata11[29];
    char stringdata12[12];
    char stringdata13[13];
    char stringdata14[29];
    char stringdata15[12];
    char stringdata16[10];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSCmyClientENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSCmyClientENDCLASS_t qt_meta_stringdata_CLASSCmyClientENDCLASS = {
    {
        QT_MOC_LITERAL(0, 9),  // "CmyClient"
        QT_MOC_LITERAL(10, 16),  // "performOperation"
        QT_MOC_LITERAL(27, 0),  // ""
        QT_MOC_LITERAL(28, 9),  // "operation"
        QT_MOC_LITERAL(38, 10),  // "disconnect"
        QT_MOC_LITERAL(49, 13),  // "connectToHost"
        QT_MOC_LITERAL(63, 4),  // "host"
        QT_MOC_LITERAL(68, 4),  // "port"
        QT_MOC_LITERAL(73, 9),  // "connected"
        QT_MOC_LITERAL(83, 12),  // "disconnected"
        QT_MOC_LITERAL(96, 5),  // "error"
        QT_MOC_LITERAL(102, 28),  // "QAbstractSocket::SocketError"
        QT_MOC_LITERAL(131, 11),  // "socketError"
        QT_MOC_LITERAL(143, 12),  // "stateChanged"
        QT_MOC_LITERAL(156, 28),  // "QAbstractSocket::SocketState"
        QT_MOC_LITERAL(185, 11),  // "socketState"
        QT_MOC_LITERAL(197, 9)   // "readyRead"
    },
    "CmyClient",
    "performOperation",
    "",
    "operation",
    "disconnect",
    "connectToHost",
    "host",
    "port",
    "connected",
    "disconnected",
    "error",
    "QAbstractSocket::SocketError",
    "socketError",
    "stateChanged",
    "QAbstractSocket::SocketState",
    "socketState",
    "readyRead"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSCmyClientENDCLASS[] = {

 // content:
      11,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   62,    2, 0x0a,    1 /* Public */,
       4,    0,   65,    2, 0x0a,    3 /* Public */,
       5,    2,   66,    2, 0x0a,    4 /* Public */,
       8,    0,   71,    2, 0x08,    7 /* Private */,
       9,    0,   72,    2, 0x08,    8 /* Private */,
      10,    1,   73,    2, 0x08,    9 /* Private */,
      13,    1,   76,    2, 0x08,   11 /* Private */,
      16,    0,   79,    2, 0x08,   13 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::UShort,    6,    7,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 11,   12,
    QMetaType::Void, 0x80000000 | 14,   15,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject CmyClient::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CLASSCmyClientENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSCmyClientENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSCmyClientENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<CmyClient, std::true_type>,
        // method 'performOperation'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'disconnect'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'connectToHost'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<quint16, std::false_type>,
        // method 'connected'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'disconnected'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'error'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QAbstractSocket::SocketError, std::false_type>,
        // method 'stateChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QAbstractSocket::SocketState, std::false_type>,
        // method 'readyRead'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void CmyClient::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<CmyClient *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->performOperation((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 1: _t->disconnect(); break;
        case 2: _t->connectToHost((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<quint16>>(_a[2]))); break;
        case 3: _t->connected(); break;
        case 4: _t->disconnected(); break;
        case 5: _t->error((*reinterpret_cast< std::add_pointer_t<QAbstractSocket::SocketError>>(_a[1]))); break;
        case 6: _t->stateChanged((*reinterpret_cast< std::add_pointer_t<QAbstractSocket::SocketState>>(_a[1]))); break;
        case 7: _t->readyRead(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 5:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QAbstractSocket::SocketError >(); break;
            }
            break;
        case 6:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QAbstractSocket::SocketState >(); break;
            }
            break;
        }
    }
}

const QMetaObject *CmyClient::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CmyClient::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSCmyClientENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int CmyClient::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    return _id;
}
QT_WARNING_POP
