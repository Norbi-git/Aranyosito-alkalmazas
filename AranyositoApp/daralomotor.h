#ifndef DARALOMOTOR_H
#define DARALOMOTOR_H

#include <QString>
#include <QVector>
#include <QStringList>

struct TakarmanyAlapanyag {
    QString nev;
    double raktarSuly;
    double maxSzazalek;
    double veglegesSuly;
    bool lekorlatozva;
};

class DaraloMotor {
public:
    static bool ellenorizTiltolista(const QString& nev);
    static QVector<TakarmanyAlapanyag> optimalizal(const QStringList& sorok, double kertMazsa, QString& hibaUzenet);
};

#endif // DARALOMOTOR_H