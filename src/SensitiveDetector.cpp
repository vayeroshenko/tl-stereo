/*
 * SensitiveDetector.cpp
 *
 *  Created on: Oct 3, 2018
 *      Author: vsevolod
 */

#include "SensitiveDetector.h"



SensitiveDetector::SensitiveDetector(G4String name, EventAction *evAct) :
    G4VSensitiveDetector(name), _eventAction(evAct){

    //  G4RunManager* runManager = G4RunManager::GetRunManager();
    G4String HCname;
    collectionName.insert(HCname="OpticalPhotonCollection");

    G4cout << "Sensitive detector created" << G4endl;
}

SensitiveDetector::~SensitiveDetector() {
}

void SensitiveDetector::Initialize(G4HCofThisEvent* HCE)
{

    OpticalPhotonCollection =
            new HitsCollection(SensitiveDetectorName, collectionName[0]);
    static G4int HCID = -1;
    if (HCID < 0) {
        HCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
    }
    HCE->AddHitsCollection(HCID, OpticalPhotonCollection);


}


G4bool SensitiveDetector::ProcessHitsL(G4Step* aStep,
                                       G4TouchableHistory* hist) {
    return ProcessHits(aStep, hist);
}

G4bool SensitiveDetector::ProcessHits(G4Step* aStep,
                                      G4TouchableHistory*)
{


    //    G4cout << "Hit!" << G4endl;

    // Getting a track from hit
    G4Track* aTrack = aStep->GetTrack();
    // Getting the hit position
    G4ThreeVector globalPosition = aStep->GetPostStepPoint()->GetPosition();


    G4StepPoint *aPostPoint = aStep->GetPostStepPoint();
    G4StepPoint *aPrevPoint = aStep->GetPreStepPoint();
    if (!aPostPoint->GetPhysicalVolume()) return false;

    G4LogicalVolume *PostVolume = aPostPoint->GetPhysicalVolume()->GetLogicalVolume();
    G4LogicalVolume *PrevVolume = aPrevPoint->GetPhysicalVolume()->GetLogicalVolume();


    // Names of previous and next volume
    G4String PreName = PrevVolume->GetName();
    G4String PostName = PostVolume->GetName();

    // For simplicity of using long lines
    const G4DynamicParticle *aParticle = aTrack->GetDynamicParticle();

    // Handling only optical photons
    if (aParticle->GetDefinition()->GetParticleName() != "opticalphoton")
        return false;

    if ( (PostName == "pmtSphere1" || PostName == "pmtSphere2") &&
         (PreName == "acrylicCupWater1" || PreName == "acrylicCupWater2")) {

        HitData hitInfo;
        myHit *newHit = new myHit();
        newHit->myData = hitInfo;

        newHit->myData.photonTime = aPostPoint->GetGlobalTime();
        newHit->myData.photonWavelength = hbarc * twopi / aParticle->GetTotalEnergy();
        newHit->myData.photonMotherID = aTrack->GetParentID();

        if ( PostName == "pmtSphere1")
            newHit->myData.photonDetID = 1;
        if ( PostName == "pmtSphere2")
            newHit->myData.photonDetID = 2;

        OpticalPhotonCollection->insert(newHit);
    }

    return true;
}

void SensitiveDetector::EndOfEvent(G4HCofThisEvent*)
{}


