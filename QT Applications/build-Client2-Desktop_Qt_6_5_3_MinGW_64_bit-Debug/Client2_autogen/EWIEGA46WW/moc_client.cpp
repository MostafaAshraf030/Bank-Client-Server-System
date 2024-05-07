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
    "connected",
    "",
    "disconnected",
    "error",
    "QAbstractSocket::SocketError",
    "socketError",
    "stateChanged",
    "QAbstractSocket::SocketState",
    "socketState",
    "readyRead",
    "run",
    "printWelcome",
    "enterUsernameAndPassword",
    "showAdminMenu",
    "showUserMenu"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSCmyClientENDCLASS_t {
    uint offsetsAndSizes[32];
    char stringdata0[10];
    char stringdata1[10];
    char stringdata2[1];
    char stringdata3[13];
    char stringdata4[6];
    char stringdata5[29];
    char stringdata6[12];
    char stringdata7[13];
    char stringdata8[29];
    char stringdata9[12];
    char stringdata10[10];
    char stringdata11[4];
    char stringdata12[13];
    char stringdata13[25];
    char stringdata14[14];
    char stringdata15[13];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSCmyClientENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSCmyClientENDCLASS_t qt_meta_stringdata_CLASSCmyClientENDCLASS = {
    {
        QT_MOC_LITERAL(0, 9),  // "CmyClient"
        QT_MOC_LITERAL(10, 9),  // "connected"
        QT_MOC_LITERAL(20, 0),  // ""
        QT_MOC_LITERAL(21, 12),  // "disconnected"
        QT_MOC_LITERAL(34, 5),  // "error"
        QT_MOC_LITERAL(40, 28),  // "QAbstractSocket::SocketError"
        QT_MOC_LITERAL(69, 11),  // "socketError"
        QT_MOC_LITERAL(81, 12),  // "stateChanged"
        QT_MOC_LITERAL(94, 28),  // "QAbstractSocket::SocketState"
        QT_MOC_LITERAL(123, 11),  // "socketState"
        QT_MOC_LITERAL(135, 9),  // "readyRead"
        QT_MOC_LITERAL(145, 3),  // "run"
        QT_MOC_LITERAL(149, 12),  // "printWelcome"
        QT_MOC_LITERAL(162, 24),  // "enterUsernameAndPassword"
        QT_MOC_LITERAL(187, 13),  // "showAdminMenu"
        QT_MOC_LITERAL(201, 12)   // "showUserMenu"
    },
    "CmyClient",
    "connected",
    "",
    "disconnected",
    "error",
    "QAbstractSocket::SocketError",
    "socketError",
    "stateChanged",
    "QAbstractSocket::SocketState",
    "socketState",
    "readyRead",
    "run",
    "printWelcome",
    "enterUsernameAndPassword",
    "showAdminMenu",
    "showUserMenu"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSCmyClientENDCLASS[] = {

 // content:
      11,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   74,    2, 0x0a,    1 /* Public */,
       3,    0,   75,    2, 0x0a,    2 /* Public */,
       4,    1,   76,    2, 0x0a,    3 /* Public */,
       7,    1,   79,    2, 0x0a,    5 /* Public */,
      10,    0,   82,    2, 0x0a,    7 /* Public */,
      11,    0,   83,    2, 0x0a,    8 /* Public */,
      12,    0,   84,    2, 0x0a,    9 /* Public */,
      13,    0,   85,    2, 0x0a,   10 /* Public */,
      14,    0,   86,    2, 0x0a,   11 /* Public */,
      15,    0,   87,    2, 0x0a,   12 /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
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
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'run'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'printWelcome'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'enterUsernameAndPassword'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'showAdminMenu'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'showUserMenu'
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
        case 0: _t->connected(); break;
        case 1: _t->disconnected(); break;
        case 2: _t->error((*reinterpret_cast< std::add_pointer_t<QAbstractSocket::SocketError>>(_a[1]))); break;
        case 3: _t->stateChanged((*reinterpret_cast< std::add_pointer_t<QAbstractSocket::SocketState>>(_a[1]))); break;
        case 4: _t->readyRead(); break;
        case 5: _t->run(); break;
        case 6: _t->printWelcome(); break;
        case 7: _t->enterUsernameAndPassword(); break;
        case 8: _t->showAdminMenu(); break;
        case 9: _t->showUserMenu(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QAbstractSocket::SocketError >(); break;
            }
            break;
        case 3:
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
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    }
    return _id;
}
QT_WARNING_POP
