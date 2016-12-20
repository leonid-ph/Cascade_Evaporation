#include "plotwindow.h"
#include "ui_plotwindow.h"

QVector<double> E(POINTS), N(POINTS);

PlotWindow::PlotWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlotWindow)
{
    ui->setupUi(this);

    Save_button = new QPushButton("Save", this);
    Save_button->setGeometry(550, 20, 100, 30);
    adjustSize();

    connect(Save_button, SIGNAL(clicked()), this, SLOT( Save_Spectrum() ) );

   // E.clear();N.clear();


}

void PlotWindow::Plot_Spectrum (Evaporation E, char part_type, unsigned int Events)
{
    Plot(ui->widget, E , part_type, Events);

    ui->widget->replot();
}

void PlotWindow::Plot(QCustomPlot *customPlot, Evaporation Ev, char part_type, unsigned int Events)
{

    QVector <float> Y(0);

    for (int i = 0;i < POINTS ;i++)
    {
        E[i] = 0;
        N[i] = 0;
    }

    unsigned int total_parts = 0;

    Y =  Ev.Spectrum_generation(part_type, Events,&total_parts);

    QVector <float>::const_iterator largest;
    largest = std::max_element( Y.begin(), Y.end() );

    Total_parts_label = new QLabel("total_parts", this);
    Total_parts_label->setGeometry(350, 60, 200, 30);

    Total_parts_label->setText( "Total parts:\n    " + QString::number(total_parts));

    //float Step = *largest/float(POINTS);
    float Step = 50/float(POINTS);

    ///////////////////////////////////////////////////////////////////
    //QVector<double> E(POINTS), N(POINTS);

    for (int i = 0;i < POINTS ;i++)
    {
        E[i] = i*Step;
    }

    for (unsigned int i=0; i<Events; i++)
    {
        for (int j =0; j < POINTS; j++ )

            if ( (Y[i] >= E[j] - Step/2 )&&( Y[i] < E[j] + Step/2 )  )
            {
                N[j]++;
            }
    }
    ///////////////////////////////////////////////////////////////////


    // create graph and assign data to it:

    customPlot->addGraph();
    customPlot->graph(0)->setData(E, N);

    // give the axes some labels:
    customPlot->xAxis->setLabel("Energy (Mev)");
    customPlot->yAxis->setLabel("N");
    // set axes ranges, so we see all data:
    customPlot->graph(0)->rescaleAxes();
}

void PlotWindow::Save_Spectrum(void)
{
    QFile file("Spectrum.txt");
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);


    for (int j =0; j < E.size() ; j++ )
    {
        out <<E[j]<<" "<<N[j]<<"\n";
    }
    // optional, as QFile destructor will already do it:
    file.close();
}


PlotWindow::~PlotWindow()
{
    delete ui;
}
