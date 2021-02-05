#include "muon_gen.h"


muon_gen::muon_gen(){
    rgen = new TRandom3(0);
    return;
}

muon_gen::~muon_gen(){
    return;
}


//void muon_gen::generate_muon(){
//    position[0] = 0;
//    position[1] = 0;
//    position[2] = 1300. *mm;
////    position[2] = 0. *mm;

//    direction[0] = 0;
//    direction[1] = 0;
//    direction[2] = -1.;

//    energy = 300. *MeV;

//    pdgID = 13;
////    pdgID = -13;

//    return;
//}


void muon_gen::generate_muon(){
    position[0] = rgen->Uniform(-Const::waterBoxSizeX * 2, Const::waterBoxSizeX * 2);
    position[1] = rgen->Uniform(-Const::waterBoxSizeY * 2, Const::waterBoxSizeY * 2);;
    position[2] = (Const::waterBoxSizeZ *1.5 ) ;
//    position[2] = 0. *mm;

    G4double Theta = 180.0 - genCos2dist();     //deg
    //_Theta = 177.0;     //deg
    G4double Phi = rgen->Uniform(-180.0,180.0); //deg

    direction[0] = TMath::Sin(Theta)*TMath::Cos(Phi);
    direction[1] = TMath::Sin(Theta)*TMath::Sin(Phi);
    direction[2] = TMath::Cos(Theta);

    energy = rgen->Uniform(100, 1000.) * MeV;

    pdgID = (rgen->Uniform() > 0.55 ) ? 13:  -13;


//    pdgID = -13;

    return;
}

G4double muon_gen::genCos2dist(){
      Double_t theta = -999.0;//deg
      Double_t x = -999.0;
      Double_t y = -999.0;
      while(theta==-999.0 || theta > pi/2){
        x = rgen->Uniform(0.0,70.0*TMath::Pi()/180.0); //rad
        y = rgen->Uniform(0.0,1.1); //rad
        if(TMath::Power(TMath::Cos(x),1.85)>y){
          theta = x*180.0/TMath::Pi();
        }
      }
      return theta;
}
