#include "daralomotor.h"

bool DaraloMotor::ellenorizTiltolista(const QString& nev) {
    QString kisNev = nev.toLower();
    return (kisNev == "vas" || kisNev == "karton" || kisNev == "ember" ||
            kisNev == "zokni" || kisNev == "bacsi" || kisNev == "bácsi" ||
            kisNev == "zoli" || kisNev == "kő" || kisNev == "ko" ||
            kisNev == "gép" || kisNev == "gep" || kisNev == "fém" ||
            kisNev == "fem" || kisNev == "muanyag" || kisNev == "műanyag" || kisNev == "fa");
}

QVector<TakarmanyAlapanyag> DaraloMotor::optimalizal(const QStringList& sorok, double kertMazsa, QString& hibaUzenet) {
    QVector<TakarmanyAlapanyag> elemek;
    double celTomegKg = kertMazsa * 100.0;

    for (const QString& nyersSor : sorok) {
        QString sor = nyersSor.trimmed().replace(",", " ").replace(";", " ").replace("\t", " ").replace("%", "");
        if (sor.isEmpty()) continue;

        QStringList reszek = sor.split(" ", Qt::SkipEmptyParts);
        if (reszek.size() < 2) continue;

        double maxSzazalek = 100.0;
        bool okSzazalek = true;
        if (reszek.size() >= 3) {
            maxSzazalek = reszek.last().toDouble(&okSzazalek);
            reszek.removeLast();
        }

        bool okSuly = false;
        double suly = reszek.last().toDouble(&okSuly);
        reszek.removeLast();

        QString nev = reszek.join(" ").trimmed();

        if (ellenorizTiltolista(nev)) {
            hibaUzenet += QString("A(z) '%1' nem darálható le biztonságosan! Kihagyva.\n").arg(nev);
            continue;
        }

        if (nev.isEmpty() || !okSuly || !okSzazalek || suly <= 0 || maxSzazalek <= 0) continue;

        elemek.append({nev, suly, maxSzazalek, 0.0, false});
    }

    double megmaradtKiosztando = celTomegKg;
    bool tortentValtozas = true;

    while (megmaradtKiosztando > 0.01 && tortentValtozas) {
        tortentValtozas = false;
        double nemLimitaltRaktarOssz = 0.0;
        for (const auto& elem : elemek) {
            if (!elem.lekorlatozva) nemLimitaltRaktarOssz += elem.raktarSuly;
        }

        if (nemLimitaltRaktarOssz <= 0) break;

        for (auto& elem : elemek) {
            if (elem.lekorlatozva) continue;

            double reszSzorzo = elem.raktarSuly / nemLimitaltRaktarOssz;
            double javasoltSuly = elem.veglegesSuly + (megmaradtKiosztando * reszSzorzo);
            double maxEngedettKg = celTomegKg * (elem.maxSzazalek / 100.0);

            if (javasoltSuly > maxEngedettKg) {
                megmaradtKiosztando -= (maxEngedettKg - elem.veglegesSuly);
                elem.veglegesSuly = maxEngedettKg;
                elem.lekorlatozva = true;
                tortentValtozas = true;
            } else {
                megmaradtKiosztando -= (javasoltSuly - elem.veglegesSuly);
                elem.veglegesSuly = javasoltSuly;
            }
        }
    }
    return elemek;
}