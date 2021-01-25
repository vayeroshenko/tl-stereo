#pragma once

#include "Const.hh"

class muon_gen
{
public:
    muon_gen();
    ~muon_gen();

public:
    void generate_muon();

public:
    G4float position[3];
    G4float direction[3];
    G4float energy;

    G4int pdgID;

};
