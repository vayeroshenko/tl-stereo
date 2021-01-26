/*
 * EventAction.cpp
 *
 *  Created on: Oct 2, 2018
 *      Author: vsevolod
 */

#include "EventAction.h"


#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4ios.hh"

//G4
#include "G4Event.hh"
#include "G4SDManager.hh"
#include "globals.hh"

EventAction::EventAction(RunAction* runact,
                         SteppingAction* steppingAction) :
    runAction(runact), _steppingAction(steppingAction), printModulo(100)
{
    thePhotonCollectionID = -1;
}

EventAction::~EventAction() {
}

void EventAction::BeginOfEventAction(const G4Event* event)
{

    //    G4cout << "BeginOfEventAction" << G4endl;
    G4int eventNum = event->GetEventID();

    // Printing an event number
    if (eventNum%printModulo == 0) {
        G4cout << "\n---> Begin of Event: " << eventNum << G4endl;
    }

    if (thePhotonCollectionID < 0) {
        G4String colName;
        thePhotonCollectionID =
                G4SDManager::GetSDMpointer()->
                GetCollectionID(colName="OpticalPhotonCollection");
    }



    // Reset stepping
    _steppingAction->Reset();
    _steppingAction->ResetPerEvent();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event* event)
{

    //    G4cout << "End of event" << G4endl;
    // Print info about end of the event
    G4int eventNum = event->GetEventID();

    if (thePhotonCollectionID < 0) return;

    G4HCofThisEvent *HCE = event->GetHCofThisEvent();
    HitsCollection *THC = 0;

    if (HCE)
        THC = (HitsCollection*)(HCE->GetHC(thePhotonCollectionID));

    if (0 == THC) return;

//    G4int nTotPhot = _stackingAction->GetTotPhotNum();
    G4int nHit = -1;
    nHit = THC->entries();

    if (! _steppingAction->_muTrigg)
        return;

    runAction->_pmt1nPhot = 0;
    runAction->_pmt2nPhot = 0;
    runAction->_nPhot = 0;

    for (G4int i = 0; i < nHit; i++) {
        runAction->_photonTime[i] = (*THC)[i]->myData.photonTime / ns;
        runAction->_photonDetectorID[i] = (*THC)[i]->myData.photonDetID;
        runAction->_photonParentID[i] = (*THC)[i]->myData.photonMotherID;
        if (runAction->_photonDetectorID[i] == 1)
            runAction->_pmt1nPhot += 1;
        if (runAction->_photonDetectorID[i] == 2)
            runAction->_pmt2nPhot += 1;
    }

    runAction->_nPhot = runAction->_pmt1nPhot + runAction->_pmt2nPhot;


    // Getting the number of sectors from the constant collection


    runAction->_muIsDecay = _steppingAction->_muIsDecay;
    runAction->_muDecayTime = _steppingAction->_muDecayTime;

    runAction->_EventID = eventNum;

    runAction->_muInitPosX = _primGenerator->generator->position[0] / mm;
    runAction->_muInitPosY = _primGenerator->generator->position[1] / mm;
    runAction->_muInitPosZ = _primGenerator->generator->position[2] / mm;

    runAction->_muInitDirX = _primGenerator->generator->direction[0];
    runAction->_muInitDirY = _primGenerator->generator->direction[1];
    runAction->_muInitDirZ = _primGenerator->generator->direction[2];

    runAction->_muInitEnergy = _primGenerator->generator->energy / MeV;
    runAction->_muPDGid = _primGenerator->generator->pdgID;



    runAction->tree->Fill();

    //	G4cout << "End of event" << G4endl;
}
