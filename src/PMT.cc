
#include "PMT.hh"

#include <stdio.h>

//#include "G4ios.hh"
#include <cmath>
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "BM_Output.hh"

PMT* PMT::Instance_ = nullptr;
PMT::PMT(){}

PMT::~PMT(){}
PMT* PMT::Instance()
{
  if (!Instance_)
  {
    Instance_ = new PMT();
  }
  return Instance_;
}

double PMT::ProcessPMTHits(double en)
{
    double kevtoev = 1000.0;
    double calibE = 569.0;
    double sigma = 64.0 * sqrt( en * kevtoev / calibE );
    sigma=std::max(50.0,sigma);
    itt = 0;
    double rsq = 30;
    double pmtEn = 0;
    double gset = 0.0;
    double gasdev = 0.0;
    double v1 = 0.0;
    double v2 = 0.0;
    double fac = 0.0;
    srand (time(NULL));
    if (itt == 0){
        while (true){
            if (rsq >= 1 or rsq <= 0){
                v1 =  (2 *rand() % 100000) / 100000 - 1;
                v2 =  (2 *rand() % 100000) / 100000 - 1;
                rsq = v1*v1 + v2*v2;
            }
            if (rsq < 1 and rsq > 0){
                break;
            }   
        }     
        fac = sqrt(-2. * log(rsq) / rsq);
        gset = v1 * fac;
        gasdev = v2 * fac;
        itt = 1;   
    }
    else{
        gasdev = gset;
        itt = 0;
    }
    pmtEn = en * kevtoev + gasdev * sigma;
    if (pmtEn >= 0){
        pmtEn = 0;
    }
    return pmtEn;
}
