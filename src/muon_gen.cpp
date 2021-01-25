#include "muon_gen.h"


muon_gen::muon_gen(){
    return;
}

muon_gen::~muon_gen(){
    return;
}


void muon_gen::generate_muon(){
    position[0] = 0;
    position[1] = 0;
    position[2] = 1300. *mm;

    direction[0] = 0;
    direction[1] = 0;
    direction[2] = -1.;

    energy = 300. *MeV;

    pdgID = 13;
//    pdgID = -13;

    return;
}
