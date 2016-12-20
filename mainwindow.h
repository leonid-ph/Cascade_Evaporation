#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QTableView>
#include <QApplication>
#include <QInputDialog>
#include <QLineEdit>
#include <QIntValidator>
#include <QString>
#include <QObject>
#include <QVector>
#include <QComboBox>

#include <iomanip>

#include <qtablewidget.h>

#include "qcustomplot.h"
#include "plotwindow.h"
#include "evaporation.h"
#include "historywindow.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QPushButton *Exit_button, *Calculate_button, *Generate_spectrums_button, *Generate_history_button;
    QLineEdit *A_line, *Z_line, *Eex_line, *Events_line;
    QLabel *A_label, *Z_label, *E_ex_label, *Events_label;
    QComboBox *Part_box;
    QTableWidget *Output_table;

public slots:

    void slotSpectrumGen();
    void slotUpdateTable();
    void slotHistoryGen();
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
