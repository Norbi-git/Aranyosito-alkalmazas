#ifndef KONYHAMOTOR_H
#define KONYHAMOTOR_H

#include <QString>
#include <QVector>
#include <QStringList>

struct KonyhaiAlapanyag {
    QString nev;
    double raktarSuly;
    double veglegesSuly;
    bool lekorlatozva;
};

class KonyhaMotor {
public:
    static bool ellenorizTiltolista(const QString& nev);
    static QVector<KonyhaiAlapanyag> optimalizal(const QStringList& sorok, double szemelyekSzama, QString& hibaUzenet);
};

#endif // KONYHAMOTOR_H