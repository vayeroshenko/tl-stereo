/*
 * PrimaryGeneratorAction.cpp
 *
 *  Created on: Oct 2, 2018
 *      Author: vsevolod
 */

#include "PrimaryGeneratorAction.h"


PrimaryGeneratorAction::PrimaryGeneratorAction() {
    iEv = 0;


    //    G4cout << "Start creating primary generator" << G4endl;

    // Pythia seed is generated from system time
    Int_t pythiaSeed = time(NULL)%10000000;

    // Getting number of event to be run in order to give Pythia
    // an information how many event to generate
    G4RunManager *runManager = G4RunManager::GetRunManager();
    G4int evToGen = runManager->GetNumberOfEventsToBeProcessed();

    generator = new muon_gen();

    // Reading configuration file, setting number of events and seed
    //    pythia.readFile("PythiaSettings.cmnd");
    //    pythia.readString("Main:numberOfEvents = " + std::to_string(evToGen));
    //    pythia.readString("Random:seed = " + std::to_string(pythiaSeed));

    // Starting up the pythia instance
    //    pythia.init();


    //    G4cout << "Primary generator created" << G4endl;
}

PrimaryGeneratorAction::~PrimaryGeneratorAction() {

}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent) {


    //    G4cout << "Generate primaries" << G4endl;

    //    G4LogicalVolume* worldLV
    //            = G4LogicalVolumeStore::GetInstance()->GetVolume("World");

    // If current event is inapropriate trying another time
    //    if (!pythia.next()) GeneratePrimaries(anEvent);

    // filling up class variables with event data from pythia
    //    GetEvent(PythiaEvent);

    generator->generate_muon();


    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition* particle = particleTable->FindParticle(generator->pdgID);
    G4double m = particle->GetPDGMass();
    G4ThreeVector dir = G4ThreeVector(  generator->direction[0],
            generator->direction[1],
            generator->direction[2]);

    G4double momentum = generator->energy;
    G4double Ekin = (TMath::Sqrt(momentum*momentum + m*m) - m);

    _particleGun->SetParticleDefinition(particle);
    _particleGun->SetParticleMomentumDirection(dir);
    _particleGun->SetParticleEnergy(Ekin);
    _particleGun->SetParticleTime(0);
    _particleGun->SetParticlePosition(G4ThreeVector(
                                            generator->position[0],
                                            generator->position[1],
                                            generator->position[2]));

    _particleGun->GeneratePrimaryVertex(anEvent);

    // generating all primaries from event
    //    for (G4int pId = 0; pId < nParticles; ++pId){
    //        G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    //        G4ParticleDefinition* particle = particleTable->FindParticle(pdgID[pId]);
    //        G4double m = particle->GetPDGMass();

    //        G4ThreeVector dir = G4ThreeVector(pX[pId],pY[pId],pZ[pId]);

    //        G4double momentum = TMath::Sqrt(pX[pId]*pX[pId] + pY[pId]*pY[pId] + pZ[pId]*pZ[pId]);
    //        G4double Ekin = (TMath::Sqrt(momentum*momentum + m*m) - m);

    //        _particleGun->SetParticleDefinition(particle);
    //        _particleGun->SetParticleMomentumDirection(dir);
    //        _particleGun->SetParticleEnergy(Ekin);
    //        _particleGun->SetParticleTime(T[pId]);

    //        _particleGun->SetParticlePosition(G4ThreeVector(X[pId], Y[pId], Z[pId]));


    //        // Cut off low-momentum particles (< 20 MeV)
    //        if (momentum < 20.) continue; //////////////////////// Momentum cut ////////////////////////////

    //        _particleGun->GeneratePrimaryVertex(anEvent);

    //			G4cout << "Particle name = " << particle->GetParticleName() << G4endl;
    //}


    //    G4cout << "Primaries generated" << G4endl;
}


// Method for getting an information from pythia into the class variables to pass
// into the particle gun
