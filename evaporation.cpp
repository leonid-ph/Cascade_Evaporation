#include "evaporation.h"
#include <QtDebug>

#define m_n     939.565 //Mev
#define m_p     938.272
#define Pi      3.14159
#define r_0     1.3E-13 //cm
#define h       6.58e-22//Mev*s

Evaporation::Evaporation()
{
}

void Evaporation::Set (int nA, int nZ, float nE_ex)
{
    A = nA;
    Z = nZ;
    E_ex = nE_ex;
}

float Evaporation::Max_part_energy (char part_type)
{
    return E_ex - Separation_energy(part_type) - Coulumb_barrier(part_type);
}

void Evaporation::Part_properties (char part_type, int *an, int *z, double *S_b, float *a_b, float *K_b)
{
    //////////////////////////////////////////////////////
    int N = A-Z;

    float D = (N-Z)/A;
    float a = 0.085 * N;
    ////////////////////////////////////////////////////////
    float a_n = a* (1 - 1.3 *(D/A) ) * (1 - 1.3 *(D/A) ) ;
    float a_p = a*(1 + 1.3 * D/A ) * (1 + 1.3 * D/A );
    float a_d = a*(1 - 1/(2*A) ) * (1 - 1/(2*A));
    float a_t = a*(1 - 1/A - 1.3 * D/A )*(1 - 1/A - 1.3 * D/A );
    float a_He3 = a*(1 - 1/A + 1.3 * D/A )*(1 - 1/A + 1.3 * D/A );
    float a_He4 = a*(1 - 3/(2*A) )*(1 - 3/(2*A) );
    ////////////////////////////////////////////////////////

    switch (part_type)
    {
    case NEUTRON:
        *K_b = 0;
        *an = 1;
        *z = 0;
        *S_b = 1/2;
        *a_b = a_n;
        break;
    case PROTON:
        *K_b = 0.7;
        *an = 1;
        *z = 1;
        *S_b = 1/2;
        *a_b = a_p;
        break;
    case DEUTON:
        *K_b = 0.7;
        *an = 2;
        *z = 1;
        *S_b = 1;
        *a_b = a_d;
        break;
    case TRITON:
        *K_b = 0.77;
        *an = 3;
        *z = 1;
        *S_b = 1/2;
        *a_b = a_t;
        break;
    case HELIUM_3:
        *K_b = 0.8;
        *an = 3;
        *z = 2;
        *S_b = 1/2;
        *a_b = a_He3;
        break;
    case HELIUM_4:
        *K_b = 0.83;
        *an = 4;
        *z = 2;
        *S_b = 0;
        *a_b = a_He4;
        break;
    }

}

float Evaporation::Coulumb_barrier(char part_type)
{
    float K_b,a_b;
    double S_b;
    int a,z;

    Part_properties (part_type, &a, &z, &S_b, &a_b,&K_b);

    return K_b*0.96 * ( z*(Z-z) ) / ( pow ( A,0.333) + pow(a,0.333) ); //MeV;
}

float Evaporation::Binding_energy (int A, int Z)
{
    float M_vol  =  15.56*A;
    float M_sh   = -17.23* ( pow(A,0.667) );
    float M_coul = -0.714* (Z*Z)/( pow(A,0.333) );
    float M_asym = -94.8*( (A*0.5-Z)*(A*0.5-Z) ) / (float)A ;
    float M_pair;

    float Eb;

    if (A%2 == 0)
    {
        //A is even
        if (Z%2 == 0)
            //even-even
            M_pair = +34*( pow(A,(-0.75)) );
        else
            //odd-odd
            M_pair = -34*( pow(A,(-0.75)) );
    }
    else
    {
        //A is odd
        M_pair = 0;
    }

    Eb = M_vol + M_sh + M_coul + M_asym + M_pair;
    //float dM = Z*m_p + (A-Z)*m_n - Eb - A*931.4940;
    qDebug()<<"Binding energy"<<A<<Z<<Eb;

    return Eb;
}

double Evaporation::Neutron_emission_probability (void)
{
    //Доделать!!!

    float K_b,a_n;
    double S_n;
    int Aa_n,z;


    int N = A-Z;
    float a = 0.085 * N;

    Part_properties (NEUTRON,   &Aa_n, &z, &S_n, &a_n,&K_b);

    float Beta = ( 2.12*pow(A,(-0.667)) - 0.5)/( (float)(2.2*pow(A,(-0.333)) + 0.76) );

    float Alpha = 0.76 + 2.2*pow(A,(-0.333));

    float T_n_max = Max_part_energy(NEUTRON);

    float m_N = m_n*(A-Z) + m_p*Z;

    double W1 = ( (2*S_n + 1)*m_n/(m_N*2*pow(a_n, 2) ) )* pow((A-1),(0.667)) * Alpha* (m_n* pow(r_0,2)) /(2*Pi*pow(h,2));

    double W2 = exp(-2*sqrt(a*E_ex) + 2*sqrt(a_n*T_n_max) );

    double W3 = 4*a_n*T_n_max + (2*a_n*Beta - 3)*(2*sqrt(a_n*T_n_max) - 1);

    return W1*W2*W3;
}

float Evaporation::Emission_probability_ratio (char part_type)
{

    float W;

    float m_b;

    float K_b,a_b,a_n;
    double S_b,S_n;
    int Aa_b,z;

    Part_properties (NEUTRON,   &Aa_b, &z, &S_n, &a_n,&K_b);
    Part_properties (part_type, &Aa_b, &z, &S_b, &a_b,&K_b);

    m_b = Aa_b * m_n + z * m_p;

    float T_n_max = Max_part_energy(NEUTRON);

    float T_b_max = Max_part_energy(part_type);

    W = (  ( (2*S_b + 1) * m_b * T_b_max * a_n )/( (2*S_n + 1) * m_n * T_n_max * a_b )  )* exp( 2*sqrt(a_b*T_b_max) - 2*sqrt(a_n*T_n_max) );

    if (W != W)
    {
        return 0;
    }
    else
    {
        return W;
    }
}

float Evaporation::Normalized_probability (char part_type)
{
    float K = 1;

    for (int i = 1; i < 6;i++)
    {
        K += Emission_probability_ratio(i);
    }

    if (part_type == NEUTRON)
    {
        return 1/K;
    }
    else
    {
        return (1/K)*Emission_probability_ratio(part_type);
    }
}

float Evaporation::Separation_energy (char part_type)
{
    float K_b,a_b;
    double S_b;
    int a,z;

    Part_properties (part_type, &a, &z, &S_b, &a_b,&K_b);

    return Binding_energy(A,Z) - Binding_energy(A-a,Z-z);

}

float Evaporation::Kin_energy_lottery (char part_type)
{

    float T_b = 0;

    float K_b,a_b;
    double S_b;
    int a,z;

    Part_properties (part_type, &a, &z, &S_b, &a_b,&K_b);

    float T_b_max = Max_part_energy(part_type);
    float x_m = (sqrt(a_b*T_b_max + 0.25) - 0.5 )/a_b;

    for (int i = 0; i < ITERATIONS; i++)
    {
        float y1 = rand()/ ( (float) RAND_MAX );
        float y2 = rand()/ ( (float) RAND_MAX );

        float x = y1*T_b_max;

        if ( y2 <=  (x/x_m)* exp(  2*( (-a_b)*x_m + sqrt(a_b*(T_b_max - x)) )  ) )
        {
            T_b = T_b_max*y1 + Coulumb_barrier(part_type);
            break;
        }

    }

    return T_b;
}

QVector < QVector < double >  > Evaporation::History_gen (void)
{

    QVector < QVector < double >  >  History_table(0);

    QVector <double> Row(0);

    Evaporation Evap_Buf;
    Evap_Buf.Set(A,Z,E_ex);

    float R = rand()/ ( (float) RAND_MAX );
    float E_b;
    int i = 0;

    char part_type;

    while ( ( ( Evap_Buf.Max_part_energy(NEUTRON) > 0 ) ||( Evap_Buf.Max_part_energy(PROTON) > 0 ) )&&(i<ITERATIONS)  )
    {

        float P_n   =   Evap_Buf.Normalized_probability(NEUTRON);
        float P_p   =   Evap_Buf.Normalized_probability(PROTON);
        float P_d   =   Evap_Buf.Normalized_probability(DEUTON);
        float P_t   =   Evap_Buf.Normalized_probability(TRITON);
        float P_He3 =   Evap_Buf.Normalized_probability(HELIUM_3);
        float P_He4 =   Evap_Buf.Normalized_probability(HELIUM_4);

        R = rand()/ ( (float) RAND_MAX );

        //Choose the particle

        //Neutron
        if (  (R <= P_n) &&( Evap_Buf.Max_part_energy(NEUTRON) > 0 )  )
        {
            part_type = NEUTRON;

            E_b = Evap_Buf.Kin_energy_lottery(part_type);

            Row.clear();
            Row.push_back(part_type);
            Row.push_back(E_b);
            Row.push_back( Evap_Buf.Separation_energy(part_type) );
            Row.push_back( Evap_Buf.Normalized_probability(part_type) );

            Evap_Buf.E_ex = Evap_Buf.E_ex - ( Evap_Buf.Separation_energy(part_type) + Evap_Buf.Coulumb_barrier(part_type) + E_b );
            Row.push_back(Evap_Buf.E_ex) ;

            Row.push_back(Evap_Buf.Neutron_emission_probability() * Evap_Buf.Emission_probability_ratio(part_type) );

            History_table.push_back(Row);

            Evap_Buf.A = Evap_Buf.A - 1;

        }

        //Proton
        if (  (R > P_n ) && (R <= P_n + P_p )&&( Evap_Buf.Max_part_energy(PROTON) > 0 )  )
        {

            part_type = PROTON;
            E_b = Evap_Buf.Kin_energy_lottery(part_type);

            Row.clear();
            Row.push_back(part_type);
            Row.push_back(E_b);
            Row.push_back( Evap_Buf.Separation_energy(part_type) );
            Row.push_back( Evap_Buf.Normalized_probability(part_type) );



            Evap_Buf.E_ex = Evap_Buf.E_ex - ( Evap_Buf.Separation_energy(part_type) + Evap_Buf.Coulumb_barrier(part_type) + E_b );
            Row.push_back(Evap_Buf.E_ex) ;

            Row.push_back(Evap_Buf.Neutron_emission_probability() * Evap_Buf.Emission_probability_ratio(part_type) );

            History_table.push_back(Row);
            Evap_Buf.A = Evap_Buf.A - 1;
            Evap_Buf.Z = Evap_Buf.Z - 1;
        }

        //Deuton
        if (  (R > P_n + P_p ) && ( R <= (P_n + P_p + P_d) )  )
        {

            part_type = DEUTON;
            E_b = Evap_Buf.Kin_energy_lottery(part_type);

            Row.clear();
            Row.push_back(part_type);
            Row.push_back(E_b);
            Row.push_back( Evap_Buf.Separation_energy(part_type) );
            Row.push_back( Evap_Buf.Normalized_probability(part_type) );



            Evap_Buf.E_ex = Evap_Buf.E_ex - ( Evap_Buf.Separation_energy(part_type) + Evap_Buf.Coulumb_barrier(part_type) + E_b );
            Row.push_back(Evap_Buf.E_ex) ;
            Row.push_back(Evap_Buf.Neutron_emission_probability() * Evap_Buf.Emission_probability_ratio(part_type) );
            History_table.push_back(Row);

            Evap_Buf.A = Evap_Buf.A - 2;
            Evap_Buf.Z = Evap_Buf.Z - 1;
        }

        //Triton
        if (  (R > P_n + P_p + P_d ) && ( R <= (P_n + P_p + P_d + P_t) )  )
        {

            part_type = TRITON;

            E_b = Evap_Buf.Kin_energy_lottery(part_type);

            Row.clear();
            Row.push_back(part_type);
            Row.push_back(E_b);
            Row.push_back( Evap_Buf.Separation_energy(part_type) );
            Row.push_back( Evap_Buf.Normalized_probability(part_type) );



            Evap_Buf.E_ex = Evap_Buf.E_ex - ( Evap_Buf.Separation_energy(part_type) + Evap_Buf.Coulumb_barrier(part_type) + E_b );
            Row.push_back(Evap_Buf.E_ex) ;

            History_table.push_back(Row);
            Row.push_back(Evap_Buf.Neutron_emission_probability() * Evap_Buf.Emission_probability_ratio(part_type) );
            Evap_Buf.A = Evap_Buf.A - 3;
            Evap_Buf.Z = Evap_Buf.Z - 1;
        }

        //Helium - 3
        if (  (R > P_n + P_p + P_d + P_t ) && ( R <= (P_n + P_p + P_d + P_t + P_He3) )  )
        {

            part_type = HELIUM_3;

            E_b = Evap_Buf.Kin_energy_lottery(part_type);

            Row.clear();
            Row.push_back(part_type);
            Row.push_back(E_b);
            Row.push_back( Evap_Buf.Separation_energy(part_type) );
            Row.push_back( Evap_Buf.Normalized_probability(part_type) );



            Evap_Buf.E_ex = Evap_Buf.E_ex - ( Evap_Buf.Separation_energy(part_type) + Evap_Buf.Coulumb_barrier(part_type) + E_b );
            Row.push_back(Evap_Buf.E_ex) ;

            Row.push_back(Evap_Buf.Neutron_emission_probability() * Evap_Buf.Emission_probability_ratio(part_type) );

            History_table.push_back(Row);

            Evap_Buf.A = Evap_Buf.A - 3;
            Evap_Buf.Z = Evap_Buf.Z - 2;
        }


        //Helium - 4
        if (  (R > P_n + P_p + P_d + P_t + P_He3) && ( R <= (P_n + P_p + P_d + P_t + P_He3 + P_He4) )  )
        {

            part_type = HELIUM_4;

            E_b = Evap_Buf.Kin_energy_lottery(part_type);

            Row.clear();
            Row.push_back(part_type);
            Row.push_back(E_b);
            Row.push_back( Evap_Buf.Separation_energy(part_type) );
            Row.push_back( Evap_Buf.Normalized_probability(part_type) );



            Evap_Buf.E_ex = Evap_Buf.E_ex - ( Evap_Buf.Separation_energy(part_type) + Evap_Buf.Coulumb_barrier(part_type) + E_b );
            Row.push_back(Evap_Buf.E_ex) ;
            Row.push_back(Evap_Buf.Neutron_emission_probability() * Evap_Buf.Emission_probability_ratio(part_type) );
            History_table.push_back(Row);
            Evap_Buf.A = Evap_Buf.A - 4;
            Evap_Buf.Z = Evap_Buf.Z - 2;
        }

        //        else
        //        {
        //            History_table[i][0] = -1;
        //            History_table[i][1] = R ;
        //            History_table[i][2] = -1;

        //        }

        i++;
    }

    return History_table;
}

QVector <float> Evaporation::Spectrum_generation(char part_type, unsigned int Events, unsigned int *total_parts)
{

    QVector <float> Energy(0);

    int m =0;

    unsigned int t = 1;


    while (Energy.size() != Events )
    {

        QVector < QVector < double > > History_table = History_gen();

        for(int i=0;i<History_table.size(); i++)
        {
            if(Energy.size() == Events){break;}
            t++;

            if ( (History_table[i][0] == part_type) && (History_table[i][1] != 0) )
            {
                Energy.push_back( History_table[i][1] );
            }
        }

        m++;
    }

    *total_parts = t;

//    qDebug()<<"Events"<<Energy.size();
//    qDebug()<<"total parts***********"<<t;
    return Energy;
}
