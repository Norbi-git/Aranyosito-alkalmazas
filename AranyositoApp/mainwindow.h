#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Gombok regisztrálása
    void on_btnKonyha_clicked();
    void on_btnDaralo_clicked();
    void on_btnVisszaFooroldalra_clicked();
    void on_btnMehet_clicked();
    void on_btnDaraloMehet_clicked();
    void on_actionClose_triggered();
    void on_actionNew_triggered();
    void on_actionOpen_triggered();
    void on_actionSave_triggered();
private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H