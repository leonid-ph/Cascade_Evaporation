#ifndef PLOTWINDOW_H
#define PLOTWINDOW_H

#include <QDialog>
#include <QFile>
#include <QVector>

#include <algorithm>

#include "qcustomplot.h"
#include "evaporation.h"

#define POINTS      250

namespace Ui {
class PlotWindow;
}

class PlotWindow : public QDialog
{
    Q_OBJECT

public:
    explicit PlotWindow(QWidget *parent = 0);
    ~PlotWindow();

    void Plot(QCustomPlot *customPlot, Evaporation Ev, char part_type, unsigned int Events);

    void Plot_Spectrum (Evaporation E, char part_type, unsigned int Events);

    QPushButton *Save_button;

    QLabel *Total_parts_label;

public slots:

    void Save_Spectrum (void);

private:
    Ui::PlotWindow *ui;
};

#endif // PLOTWINDOW_H
