#ifndef HISTORY_H
#define HISTORY_H

#include <QDialog>
#include <QTableView>
#include <qtablewidget.h>

#include "evaporation.h"

namespace Ui {
class History;
}

class History : public QDialog
{
    Q_OBJECT

public:
    explicit History(QWidget *parent = 0);

    void Generate (Evaporation Evap);

    QTableWidget *Output_table;
    ~History();

private:
    Ui::History *ui;
};

#endif // HISTORY_H
