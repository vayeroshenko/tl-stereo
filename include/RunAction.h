/*
 * RunAction.h
 *
 *  Created on: Oct 2, 2018
 *      Author: vsevolod
 */

#pragma once

#include <G4UserRunAction.hh>

#include "G4Run.hh"
#include "G4RunManager.hh"

//my
#include "HitDataStructure.hh"
#include "Const.hh"

//G4
#include "G4Timer.hh"
#include "globals.hh"
#include "G4UserRunAction.hh"

//root
#include "TTree.h"
#include "TFile.h"


class RunAction: public G4UserRunAction {
public:
    RunAction();
    virtual ~RunAction();
public:
    virtual void BeginOfRunAction(const G4Run* run);
    virtual void   EndOfRunAction(const G4Run* run);

public:
    void SetOutputFileName(G4String fileName) {_outputFileName = fileName;}
    G4String GetOutputFileName() { return _outputFileName;}
    TTree* tree;
    HitData HitInfo;

//    G4int _nPart;
    G4int _EventID;
    G4int _nSec;
    G4int _nPhot;

    static const G4int _nPartMax = 200000;
//    G4int _TrackID[_nPartMax];
//    G4int _ParentID[_nPartMax];
//    G4int _PdgID[_nPartMax];
//    G4int _StationID[_nPartMax];
//    G4double _Energy[_nPartMax];
//    G4double _Time[_nPartMax];
//    G4double _X[_nPartMax];
//    G4double _Y[_nPartMax];
//    G4double _Z[_nPartMax];
//    G4double _Px[_nPartMax];
//    G4double _Py[_nPartMax];
//    G4double _Pz[_nPartMax];
//    G4double _Momentum[_nPartMax];

    G4float _muInitPosX;
    G4float _muInitPosY;
    G4float _muInitPosZ;
    G4float _muInitDirX;
    G4float _muInitDirY;
    G4float _muInitDirZ;

    G4int _muPDGid;

    G4int _muIsDecay;
    G4float _muInitEnergy;
    G4float _muDecayTime;
    G4int _pmt1nPhot;
    G4int _pmt2nPhot;

    G4float _photonTime[_nPartMax];
    G4float _photonWavelength[_nPartMax];
    G4int _photonParentID[_nPartMax];
    G4int _photonDetectorID[_nPartMax];





private:
    G4Timer* timer;
    TFile* hfile;
    G4String _outputFileName = "data.root";
};
