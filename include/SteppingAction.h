/*
 * SteppingAction.h
 *
 *  Created on: Oct 2, 2018
 *      Author: vsevolod
 */

#pragma once

#include <G4UserSteppingAction.hh>
#include "PrimaryGeneratorAction.h"

class PrimaryGeneratorAction;

class SteppingAction: public G4UserSteppingAction {
public:
    SteppingAction(PrimaryGeneratorAction*);
    virtual ~SteppingAction();
	void UserSteppingAction(const G4Step*);

	void Reset();
	void ResetPerEvent();

private:
    PrimaryGeneratorAction* _genAction;

    G4double _probOfReflection;
    G4double _particleID;

    void InternalReflectionProbability(G4double energy,
                                       G4double& probability);

public:

    G4bool _muTrigg;

    G4float _muInitPosX;
    G4float _muInitPosY;
    G4float _muInitPosZ;
    G4float _muInitDirX;
    G4float _muInitDirY;
    G4float _muInitDirZ;

    G4int _muPDGid;

    G4float _muIsDecay;
    G4float _muDecayTime;
    G4float _muInitEnergy;
    G4int _pmt1nPhot;
    G4int _pmt2nPhot;

};


