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
};

