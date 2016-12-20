#ifndef HISTORYWINDOW_H
#define HISTORYWINDOW_H

#include <QWidget>
#include <QDialog>
#include <QTableView>
#include <qtablewidget.h>

#include "evaporation.h"

namespace Ui {
class HistoryWindow;
}

class HistoryWindow : public QWidget
{
    Q_OBJECT

public:
    explicit HistoryWindow(QWidget *parent = 0);
    ~HistoryWindow();

    void Generate (Evaporation Evap);

    QTableWidget *Output_table;



private:
    Ui::HistoryWindow *ui;
};

#endif // HISTORYWINDOW_H
