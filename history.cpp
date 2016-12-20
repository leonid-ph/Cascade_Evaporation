#include "history.h"
#include "ui_history.h"
#include "evaporation.h"



History::History(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::History)
{
    ui->setupUi(this);
    //    setFixedSize(800, 500);
    setGeometry(400, 250, 400, 500);

    Output_table = new QTableWidget(HISTORY_ROWS,HISTORY_COLUMNS,this);

    Output_table->setGeometry(25,10,350,450);

    QStringList Output_H_table_Header;
    Output_H_table_Header<<"Particle type"<<"Energy (MeV)"<<"Excitation (MeV)";
    Output_table->setHorizontalHeaderLabels(Output_H_table_Header);

    Output_table->show();

    Output_table->horizontalHeader()->setResizeMode(QHeaderView::Stretch);

    // Generate(E);

}

void History::Generate ( Evaporation Evap)
{
    //Evaporation Evap = E;

    //Evap.Set(173,70,110);
    int y = 10;


    float History_table[HISTORY_ROWS][HISTORY_COLUMNS];

    for(int i = 0;i < HISTORY_ROWS; i++)
    {
        for (int j = 0; j < HISTORY_COLUMNS; j++)
        {
            History_table[i][j] = 0;
        }
    }

    Evap.Generate_history(History_table);


    for(int i = 2;i < HISTORY_ROWS; i++)
    {
        if ( History_table[i][2] == 0)
        {y = i; break;}
    }

    Output_table->setRowCount(y+1);

    for(int i = 0;i < HISTORY_ROWS; i++)
    {
        for (int j = 0; j < HISTORY_COLUMNS; j++)
        {
            if (j == 0) { Output_table->setItem(i, j, new QTableWidgetItem( QString::number( History_table[i][j] ) ));}
            else        { Output_table->setItem(i, j, new QTableWidgetItem( QString::number( History_table[i][j], 'f', 2 ) ));}

            if (History_table[i][0] == NEUTRON){Output_table->setItem(i, 0, new QTableWidgetItem("n") );}
            if (History_table[i][0] == PROTON) {Output_table->setItem(i, 0, new QTableWidgetItem("p") );}
            if (History_table[i][0] == DEUTON) {Output_table->setItem(i, 0, new QTableWidgetItem("d") );}
            if (History_table[i][0] == TRITON) {Output_table->setItem(i, 0, new QTableWidgetItem("t") );}
            if (History_table[i][0] == HELIUM_3) {Output_table->setItem(i, 0, new QTableWidgetItem("He-3") );}
            if (History_table[i][0] == HELIUM_4) {Output_table->setItem(i, 0, new QTableWidgetItem("He-4") );}
        }

    }

    Output_table->setItem(y, 0, new QTableWidgetItem( "Gamma") );
    Output_table->setItem(y, 1, new QTableWidgetItem(  QString::number( History_table[y-1][2], 'f', 2  ) ) );

}

History::~History()
{
    delete ui;
}
