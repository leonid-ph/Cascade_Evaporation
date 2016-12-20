#ifndef EVAPORATION_H
#define EVAPORATION_H

#include <QVector>
#include "math.h"
#include <cstdlib>

#define NEUTRON     0
#define PROTON      1
#define DEUTON      2
#define TRITON      3
#define HELIUM_3    4
#define HELIUM_4    5

#define ITERATIONS  10000000

//#define INTEGRAL    50000



class Evaporation
{
public:

    int A,Z;
    float E_ex;

    void Set (int nA,int nZ, float nE_ex);

    float Coulumb_barrier(char part_type);

    float Binding_energy(int A, int Z);

    double Neutron_emission_probability (void);

    float Emission_probability_ratio (char part_type);

    float Normalized_probability (char part_type);

    float Separation_energy (char part_type);

    float Kin_energy_lottery (char part_type);

    void Part_properties (char part_type, int *a, int *z, double *S_b, float*a_b, float *K_b);

    float Max_part_energy (char part_type);

    QVector <float> Spectrum_generation(char part_type, unsigned int Events, unsigned int *total_parts);

    QVector < QVector < double > >  History_gen (void);

    Evaporation();
};

#endif // EVAPORATION_H
