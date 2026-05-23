#include "konyhamotor.h"

bool KonyhaMotor::ellenorizTiltolista(const QString& nev) {
    QString kisNev = nev.toLower();
    return (kisNev == "vas" || kisNev == "karton" || kisNev == "ember" ||
            kisNev == "zokni" || kisNev == "bacsi" || kisNev == "bácsi" ||
            kisNev == "zoli" || kisNev == "kő" || kisNev == "ko" ||
            kisNev == "fa" || kisNev == "muanyag" || kisNev == "műanyag");
}

QVector<KonyhaiAlapanyag> KonyhaMotor::optimalizal(const QStringList& sorok, double szemelyekSzama, QString& hibaUzenet) {
    QVector<KonyhaiAlapanyag> elemek;
    double celTomegKg = szemelyekSzama * 1.50;

    for (const QString& nyersSor : sorok) {
        QString sor = nyersSor.trimmed().replace(";", " ").replace("\t", " ");
        if (sor.isEmpty()) continue;

        int utolsoSzokoz = sor.lastIndexOf(" ");
        if (utolsoSzokoz == -1) continue;

        QString nev = sor.left(utolsoSzokoz).trimmed();
        QString szamSzoveg = sor.mid(utolsoSzokoz).trimmed().replace(",", ".");

        if (ellenorizTiltolista(nev)) {
            hibaUzenet += QString("A(z) '%1' nem ehető! Kihagyva.\n").arg(nev);
            continue;
        }

        bool ok = false;
        double suly = szamSzoveg.toDouble(&ok);
        if (!nev.isEmpty() && ok && suly > 0) {
            elemek.append({nev, suly, 0.0, false});
        }
    }

    double megmaradtKiosztando = celTomegKg;
    bool tortentValtozas = true;

    while (megmaradtKiosztando > 0.01 && tortentValtozas) {
        tortentValtozas = false;
        double szabadRaktar = 0.0;
        for (const auto& e : elemek) {
            if (!e.lekorlatozva) szabadRaktar += e.raktarSuly;
        }
        if (szabadRaktar <= 0) break;

        for (auto& e : elemek) {
            if (e.lekorlatozva) continue;
            double resz = e.raktarSuly / szabadRaktar;
            double javasolt = e.veglegesSuly + (megmaradtKiosztando * resz);

            if (javasolt > e.raktarSuly) {
                megmaradtKiosztando -= (e.raktarSuly - e.veglegesSuly);
                e.veglegesSuly = e.raktarSuly;
                e.lekorlatozva = true;
                tortentValtozas = true;
            } else {
                megmaradtKiosztando -= (javasolt - e.veglegesSuly);
                e.veglegesSuly = javasolt;
            }
        }
    }
    return elemek;
}