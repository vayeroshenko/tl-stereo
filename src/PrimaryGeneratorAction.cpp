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

    generator = new muon_gen();


    //    G4cout << "Primary generator created" << G4endl;
}

PrimaryGeneratorAction::~PrimaryGeneratorAction() {

}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent) {


    generator->generate_cosmic();
//    generator->generate_debug();


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

    //    G4cout << "Primaries generated" << G4endl;
}


// Method for getting an information from pythia into the class variables to pass
// into the particle gun
