#ifndef DATATYPE_H
#define DATATYPE_H
#include <QVector>

struct CommandData{
    quint8 type;
    quint16 length;
    QVector<quint8> value;
};


#endif // DATATYPE_H
