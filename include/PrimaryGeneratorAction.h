/*
 * PrimaryGeneratorAction.h
 *
 *  Created on: Oct 2, 2018
 *      Author: vsevolod
 */

#pragma once

#include <G4VUserPrimaryGeneratorAction.hh>
#include "globals.hh"

#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"
#include "TRandom3.h"
#include "G4Timer.hh"

#include "TROOT.h"
#include "TFile.h"
#include "TNtuple.h"
#include "TMath.h"
#include "TChain.h"

#include "Const.hh"
#include <time.h>

#include "Randomize.hh"
#include "muon_gen.h"


//class G4Hep
class G4HEPEvtInterface;
class G4VPrimaryGenerator;
class G4ParticleGun;
class G4Event;

class PrimaryGeneratorAction: public G4VUserPrimaryGeneratorAction {
public:
    PrimaryGeneratorAction();
    virtual ~PrimaryGeneratorAction();
public:
    void GeneratePrimaries(G4Event* anEvent);

    muon_gen *generator;

private:
    //    bool GetEvent(Pythia8::Event event);

private:
    G4ParticleGun *_particleGun = new G4ParticleGun();

    //    TChain *tree;

    // Data to be transported from pythia to gun


    G4int iEv;


    // Pythia generator declaration
    //    Pythia8::Pythia pythia;
    // Pythia event declaration
    //    Pythia8::Event& PythiaEvent = pythia.event;
};

