#include "mainwindow.h"
#include "ui_mainwindow.h"

#define BUTTON_HEIGHT 30
#define BUTTON_LENGHT 125

Evaporation Evap;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //setFixedSize(1125, 500);
    setGeometry(50,50,800,300);
    setWindowTitle("Evaporation MCC");


    //Labels
    A_label = new QLabel("Atomic number, A", this);
    A_label->setGeometry(50, 50, 200, 30);

    Z_label = new QLabel("Charge, Z", this);
    Z_label->setGeometry(50, 100, 200, 30);

    E_ex_label = new QLabel("Exitation Energy, E (MeV)", this);
    E_ex_label->setGeometry(50, 150, 200, 30);

    Events_label = new QLabel("Events:", this);
    Events_label->setGeometry(400, 240, 100, 20);

//    Bind_En = new QLabel("Binding Energy:", this);
//    Bind_En->setGeometry(50, 200, 200, 30);

    //Input lines
    A_line = new QLineEdit("173",this);
    A_line-> setValidator(new QIntValidator(this));
    A_line-> setGeometry(50, 75, 140, 20);

    Z_line = new QLineEdit("70",this);
    Z_line-> setValidator(new QIntValidator(this));
    Z_line-> setGeometry(50, 125, 140, 20);

    Eex_line = new QLineEdit("110",this);
    Eex_line->setValidator(new QIntValidator(this));
    Eex_line->setGeometry(50, 175, 140, 20);

    Events_line = new QLineEdit("5000",this);
    Events_line->setValidator(new QIntValidator(this));
    Events_line->setGeometry(440, 240, 75, 20);

    // Buttons
    Exit_button = new QPushButton("Exit", this);
    Exit_button->setGeometry(650, 250, BUTTON_LENGHT, BUTTON_HEIGHT);

    Generate_history_button = new QPushButton("Generate history", this);
    Generate_history_button->setGeometry(50, 250, BUTTON_LENGHT, BUTTON_HEIGHT);

    Generate_spectrums_button = new QPushButton("Generate spectrum", this);
    Generate_spectrums_button->setGeometry(250, 250, BUTTON_LENGHT, BUTTON_HEIGHT);

    //Table
    Output_table = new QTableWidget(4,6,this);
    Output_table->setGeometry(200,50,560,150);

    Output_table->setColumnWidth(0,60);
    Output_table->setColumnWidth(1,60);
    Output_table->setColumnWidth(2,60);
    Output_table->setColumnWidth(3,60);
    Output_table->setColumnWidth(4,60);
    Output_table->setColumnWidth(5,60);


    QStringList Output_H_table_Header, Output_V_table_Header;
    Output_H_table_Header<<"n"<<"p"<<"d"<<"t"<<"He-3"<<"He-4";
    Output_table->setHorizontalHeaderLabels(Output_H_table_Header);

    Output_V_table_Header<<"Probability of emission"<<"Maximum possible energy (MeV)"<<"Separation energy (MeV)"<<"Coulumb barrier (MeV)"<<"Random generated energy (MeV)";
    Output_table->setVerticalHeaderLabels(Output_V_table_Header);


    Part_box  = new QComboBox(this);
    Part_box->setGeometry(415,265,100,20);

    Part_box->addItem("Neutron");
    Part_box->addItem("Proton");
    Part_box->addItem("Deuton");
    Part_box->addItem("Triton");
    Part_box->addItem("Helium-3");
    Part_box->addItem("Helium-4");

    slotUpdateTable ();

    //Do the connection
    connect(Exit_button,                SIGNAL(clicked()), QApplication::instance(),    SLOT( quit() ) );
    connect(Generate_history_button,    SIGNAL(clicked()), this,                        SLOT( slotHistoryGen() ) );
    connect(Generate_spectrums_button,  SIGNAL(clicked()), this,                        SLOT( slotSpectrumGen() ) );
    connect(A_line,                     SIGNAL(editingFinished () ) , this,             SLOT( slotUpdateTable () ));
    connect(Z_line,                     SIGNAL(editingFinished () ) , this,             SLOT( slotUpdateTable () ));
    connect(Eex_line,                   SIGNAL(editingFinished () ) , this,             SLOT( slotUpdateTable () ));
}



void MainWindow::slotUpdateTable()
{

    Evap.Set(( A_line->text() ).toInt(),( Z_line->text() ).toInt(),( Eex_line->text() ).toInt());
   //Bind_En->setText( "Binding Energy: " + QString::number( Evap.Binding_energy(( A_line->text() ).toInt(),( Z_line->text() ).toInt()) ) + " MeV" );

    for (int i = 0; i<6; i++)
    {
        if (i < 3 ) {Output_table->setItem(0, i, new QTableWidgetItem( QString::number( Evap.Normalized_probability(i),  'f', 3 ) )); }
        else        {Output_table->setItem(0, i, new QTableWidgetItem( QString::number( Evap.Normalized_probability(i),  'e', 2 ) )); }
                     Output_table->setItem(1, i, new QTableWidgetItem( QString::number( Evap.Max_part_energy(i),         'f', 2 ) ));
                     Output_table->setItem(2, i, new QTableWidgetItem( QString::number( Evap.Separation_energy(i),       'f', 2 ) ));
                     Output_table->setItem(3, i, new QTableWidgetItem( QString::number( Evap.Coulumb_barrier(i),         'f', 2 ) ));
                     Output_table->setItem(4, i, new QTableWidgetItem( QString::number( Evap.Kin_energy_lottery(i),      'f', 2 ) ));
    }

    Output_table->show();
}


void MainWindow::slotSpectrumGen()
{
    slotUpdateTable ();

    PlotWindow *pw = new PlotWindow;
    pw->setWindowTitle("Spectrum");
    pw->Plot_Spectrum(Evap,Part_box->currentIndex(),  ( Events_line->text() ).toInt() );
    pw->show();
}


void MainWindow::slotHistoryGen()
{
    slotUpdateTable ();

    HistoryWindow *h = new HistoryWindow;
    h->setWindowTitle("History");
    h->Generate(Evap);
    h->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}
