#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "konyhamotor.h"
#include "daralomotor.h"
#include <QFileDialog>
#include <QHeaderView>
#include <QMessageBox>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Kezdésként a Főoldalra ugrunk
    ui->stackedWidget->setCurrentIndex(0);

    // Eredménytáblázat oszlopainak kihúzása
    ui->tableEredmeny->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableEredmeny->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Oldalváltások
void MainWindow::on_btnKonyha_clicked() { ui->stackedWidget->setCurrentIndex(1); }
void MainWindow::on_btnDaralo_clicked() { ui->stackedWidget->setCurrentIndex(2); }

void MainWindow::on_btnVisszaFooroldalra_clicked()
{
    ui->txtKonyhaSzoveg->clear();
    ui->txtDaraloSzoveg->clear();
    ui->tableEredmeny->setRowCount(0);

    ui->comboSzemelyek->setCurrentIndex(0);
    ui->comboMennyiseg->setCurrentIndex(0);

    ui->stackedWidget->setCurrentIndex(0);
}

// Menüpontok
void MainWindow::on_actionClose_triggered() { this->close(); }

void MainWindow::on_actionNew_triggered()
{
    ui->txtKonyhaSzoveg->clear();
    ui->txtDaraloSzoveg->clear();
    ui->tableEredmeny->setRowCount(0);
    ui->stackedWidget->setCurrentIndex(0);
    QMessageBox::information(this, "Új projekt", "Minden mező leürítve!");
}

void MainWindow::on_actionOpen_triggered()
{
    QString fajlNev = QFileDialog::getOpenFileName(this, "Recept betöltése", "", "Szöveges fájlok (*.txt *.csv)");
    if (fajlNev.isEmpty()) return;

    QFile fajl(fajlNev);
    if (!fajl.open(QIODevice::ReadOnly | QIODevice::Text)) return;

    QTextStream be(&fajl);
    int aktOldal = ui->stackedWidget->currentIndex();

    if (aktOldal == 1) ui->txtKonyhaSzoveg->setPlainText(be.readAll());
    else if (aktOldal == 2) ui->txtDaraloSzoveg->setPlainText(be.readAll());

    fajl.close();
}

void MainWindow::on_actionSave_triggered()
{
    QString fajlNev = QFileDialog::getSaveFileName(this, "Recept mentése", "", "Szöveges fájlok (*.txt *.csv)");
    if (fajlNev.isEmpty()) return;

    QFile fajl(fajlNev);
    if (!fajl.open(QIODevice::WriteOnly | QIODevice::Text)) return;

    QTextStream ki(&fajl);
    int aktOldal = ui->stackedWidget->currentIndex();

    if (aktOldal == 1) ki << ui->txtKonyhaSzoveg->toPlainText();
    else if (aktOldal == 2) ki << ui->txtDaraloSzoveg->toPlainText();

    fajl.close();
    QMessageBox::information(this, "Mentés", "Sikeresen elmentve!");
}

// ==========================================
// KONYHA:
// ==========================================
void MainWindow::on_btnMehet_clicked()
{
    double szemelyekSzama = ui->comboSzemelyek->currentText().split(" ")[0].toDouble();
    if (szemelyekSzama <= 0) szemelyekSzama = 1.0;

    ui->tableEredmeny->setRowCount(0);
    QString hibaUzenet = "";

    QStringList sorok = ui->txtKonyhaSzoveg->toPlainText().split("\n");
    auto receptElemek = KonyhaMotor::optimalizal(sorok, szemelyekSzama, hibaUzenet);

    if (!hibaUzenet.isEmpty()) {
        QMessageBox::warning(this, "Érvénytelen alapanyag", hibaUzenet);
    }

    if (receptElemek.isEmpty()) {
        ui->stackedWidget->setCurrentIndex(3);
        return;
    }

    QString figyelmeztetesSzoveg = "";
    for (const auto& elem : receptElemek) {
        if (elem.lekorlatozva) {
            figyelmeztetesSzoveg += QString("%1: Elfogyott a teljes otthoni készlet.\n").arg(elem.nev.toUpper());
        }

        int ujSor = ui->tableEredmeny->rowCount();
        ui->tableEredmeny->insertRow(ujSor);
        ui->tableEredmeny->setItem(ujSor, 0, new QTableWidgetItem(elem.nev.toUpper()));

        QString kisNev = elem.nev.toLower();
        QString veglegesMertekegyseg = " kg";
        double veglegesErtek = elem.veglegesSuly;

        if (kisNev.contains("tej") || kisNev.contains("viz") || kisNev.contains("víz") ||
            kisNev.contains("olaj") || kisNev.contains("tejszin") || kisNev.contains("tejszín") ||
            kisNev.contains("le") || kisNev.contains("szrup") || kisNev.contains("szirup") ||
            kisNev.contains("joghurt") || kisNev.contains("kefir") || kisNev.contains("kefír"))
        {
            if (elem.veglegesSuly < 0.2) {
                veglegesErtek = elem.veglegesSuly * 1000.0;
                veglegesMertekegyseg = " ml";
            } else {
                veglegesErtek = elem.veglegesSuly;
                veglegesMertekegyseg = " l";
            }
        }

        ui->tableEredmeny->setItem(ujSor, 1, new QTableWidgetItem(QString::number(veglegesErtek, 'f', 2) + veglegesMertekegyseg));
    }

    if (!figyelmeztetesSzoveg.isEmpty()) {
        QMessageBox::information(this, "Konyhai készlet-optimalizálás", "Néhány alapanyagból a teljes otthoni készletet felhasználtam:\n\n" + figyelmeztetesSzoveg);
    }

    ui->stackedWidget->setCurrentIndex(3);
}

// ==========================================
// DARÁLÓ OLDAL:
// ==========================================
void MainWindow::on_btnDaraloMehet_clicked()
{
    double kertMazsa = ui->comboMennyiseg->currentText().toDouble();
    if (kertMazsa <= 0) kertMazsa = 1.0;

    ui->tableEredmeny->setRowCount(0);
    QString hibaUzenet = "";

    QStringList sorok = ui->txtDaraloSzoveg->toPlainText().split("\n");
    auto keverekElemek = DaraloMotor::optimalizal(sorok, kertMazsa, hibaUzenet);

    if (!hibaUzenet.isEmpty()) {
        QMessageBox::warning(this, "Veszélyes alapanyag", hibaUzenet);
    }

    if (keverekElemek.isEmpty()) return;

    QString limitSzoveg = "";
    for (const auto& elem : keverekElemek) {
        if (elem.lekorlatozva && elem.maxSzazalek < 100.0) {
            limitSzoveg += QString("%1: Elérte a biztonságos %2%-os limitet (%3 kg).\n")
                               .arg(elem.nev.toUpper())
                               .arg(elem.maxSzazalek)
                               .arg(QString::number(elem.veglegesSuly, 'f', 2));
        }

        int ujSor = ui->tableEredmeny->rowCount();
        ui->tableEredmeny->insertRow(ujSor);
        ui->tableEredmeny->setItem(ujSor, 0, new QTableWidgetItem(elem.nev.toUpper()));
        ui->tableEredmeny->setItem(ujSor, 1, new QTableWidgetItem(QString::number(elem.veglegesSuly, 'f', 2) + " kg"));
    }

    if (!limitSzoveg.isEmpty()) {
        QMessageBox::information(this, "Takarmány receptúra optimalizálva", "Az állatok egészsége érdekében a keverék arányait finomítottam:\n\n" + limitSzoveg);
    }

    ui->stackedWidget->setCurrentIndex(3);
}