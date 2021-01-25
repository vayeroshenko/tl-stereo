/*
 * EventAction.h
 *
 *  Created on: Oct 2, 2018
 *      Author: vsevolod
 */

#pragma once

#include <G4UserEventAction.hh>
#include "globals.hh"
#include "RunAction.h"
#include "Hit.h"
#include "SteppingAction.h"

class G4Event;

class RunAction;
class SteppingAction;
class PrimaryGeneratorAction;

class EventAction: public G4UserEventAction {
public:
    EventAction(RunAction*, SteppingAction*);
    virtual ~EventAction();
public:
    virtual void  BeginOfEventAction(const G4Event* );
    virtual void    EndOfEventAction(const G4Event* );

    void SetPrimGenerator(PrimaryGeneratorAction *gen){_primGenerator = gen;}
    inline void InsertPhoton(G4int secID){ _nPhot[secID] ++; }

private:
    RunAction* runAction;
    SteppingAction* _steppingAction;
    G4int printModulo;
    G4int theCollectionID;

    G4int _nPhot[2];

    PrimaryGeneratorAction* _primGenerator;
};
