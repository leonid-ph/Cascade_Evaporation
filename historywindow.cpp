#include "historywindow.h"
#include "ui_historywindow.h"

HistoryWindow::HistoryWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HistoryWindow)
{
    ui->setupUi(this);
    setGeometry(300, 200, 640, 400);

    Output_table = new QTableWidget(1,6,this);

    Output_table->setGeometry(10,10,625,350);
    //Output_table->adjustSize();

    QStringList Output_H_table_Header;
    Output_H_table_Header<<"Particle type"<<"Energy (MeV)"<<"Separation\n Energy (Mev)"<<"Relative\n probability"<<"Excitation (MeV)"<<"Probability";
    Output_table->setHorizontalHeaderLabels(Output_H_table_Header);

    Output_table->show();

}

void HistoryWindow::Generate ( Evaporation Evap )
{
    QVector < QVector < double > > History_table = Evap.History_gen();

    for(int i=0;i<History_table.size(); i++)
    {
        Output_table->insertRow( Output_table->rowCount() );
        for (int j=0;j<History_table[i].size(); j++)
        {


            Output_table->setItem(i, j, new QTableWidgetItem(  QString::number( History_table[i][j], 'f', 2  ) ) );
           // if(j==5) Output_table->setItem(i, j, new QTableWidgetItem(  QString::number( History_table[i][j], 'e', 2  ) ) );
            if(j==5) Output_table->setItem(i, j, new QTableWidgetItem( "NA" ) );
            int k = History_table[i][0];

            switch (k)
            {
            case NEUTRON:
                Output_table->setItem(i, 0, new QTableWidgetItem("n") );
                break;
            case PROTON:
                Output_table->setItem(i, 0, new QTableWidgetItem("p") );
                break;
            case DEUTON:
                Output_table->setItem(i, 0, new QTableWidgetItem("d") );
                break;
            case TRITON:
                Output_table->setItem(i, 0, new QTableWidgetItem("t") );
                break;
            case HELIUM_3:
                Output_table->setItem(i, 0, new QTableWidgetItem("He3") );
                break;
            case HELIUM_4:
                Output_table->setItem(i, 0, new QTableWidgetItem("He4") );
                break;

            default:
                break;
            }
        }
    }

    Output_table->setItem( History_table.size() , 0, new QTableWidgetItem( "Gamma") );
    Output_table->setItem( History_table.size() , 1, new QTableWidgetItem(  QString::number( History_table[ History_table.size() - 1][4], 'f', 2  ) ) );
    //Output_table->setItem( History_table.size() , 5, new QTableWidgetItem( "0") );


}

HistoryWindow::~HistoryWindow()
{
    delete ui;
}
