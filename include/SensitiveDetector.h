/*
 * SensitiveDetector.h
 *
 *  Created on: Oct 3, 2018
 *      Author: vsevolod
 */

#pragma once

#include "G4HCofThisEvent.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include <G4VSensitiveDetector.hh>


#include "Hit.h"
#include "HitDataStructure.hh"
#include "EventAction.h"

class G4HCofThisEvent;
class G4Step;
class G4TouchableHistory;

class SensitiveDetector: public G4VSensitiveDetector {
public:
    SensitiveDetector(G4String name, EventAction *evAct);
    virtual ~SensitiveDetector();


    void Initialize(G4HCofThisEvent*);

	G4bool ProcessHits(G4Step*, G4TouchableHistory*);

    G4bool ProcessHitsL(G4Step*, G4TouchableHistory*);

	void EndOfEvent(G4HCofThisEvent*);
private:

    G4String SDname;
    EventAction* _eventAction;

    HitsCollection* OpticalPhotonCollection;

};

// The method stolen from internet for separation words inside the string
// and returning a vector containig string of words
template <class Container>
void splitName(const std::string& str, Container& cont)
{
    std::istringstream iss(str);
    std::copy(std::istream_iterator<std::string>(iss),
         std::istream_iterator<std::string>(),
         std::back_inserter(cont));
}
