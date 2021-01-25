#include "PhysicsList.h"


// Physics list constructor inherited from QGSP_BERT
PhysicsList::PhysicsList() : QGSP_BERT()
{
    theCerenkovProcess = 0;
    defaultCutValue = 1.0*mm;
    SetVerboseLevel(0);

    G4cout << "Physics list constructor" << G4endl;
}

PhysicsList::~PhysicsList()
{
    delete theCerenkovProcess;
}

void PhysicsList::ConstructParticle()
{
    //  Constructing HEP particles from base physics list
    //  and adding OpticalPhoton to them

    QGSP_BERT::ConstructParticle();
    G4OpticalPhoton::OpticalPhotonDefinition();
}


void PhysicsList::ConstructProcess()
{
    //  Constructing processes from base physics list and adding
    //  Cherenkov process and optical processes

    QGSP_BERT::ConstructProcess();
    ConstructOp();
}
//void SetCuts();


void PhysicsList::ConstructOp()
{


    G4ParticleTable::G4PTblDicIterator *theParticleIterator = GetParticleIterator();

    G4cout<<" 000 "<<G4endl;

    // Optical Photon Processes
    theCerenkovProcess = new G4Cerenkov("Cerenkov");

    G4cout<<" 111 "<<G4endl;

    SetVerbose(0);

    theCerenkovProcess->SetMaxNumPhotonsPerStep(300);
    theCerenkovProcess->SetTrackSecondariesFirst(true);
    G4cout<<" 111 "<<G4endl;

    G4Decay* theDecayProcess = new G4Decay();
    G4VDecayChannel *mode_plus = new G4MuonDecayChannel("mu+", 1.);
    G4VDecayChannel *mode_minus = new G4MuonDecayChannel("mu-", 1.);

    theParticleIterator->reset();
    while( (*theParticleIterator)() ) {
        G4ParticleDefinition* particle = theParticleIterator->value();
        G4ProcessManager* pmanager = particle->GetProcessManager();
        G4String particleName = particle->GetParticleName();
        if (particleName == "mu+"){
            particle->GetDecayTable()->Insert(mode_plus);
            pmanager->AddProcess(theDecayProcess);
            pmanager->SetProcessOrdering(theDecayProcess, idxPostStep);
            pmanager->SetProcessOrdering(theDecayProcess, idxAtRest);
        }

        if (particleName == "mu-"){
            particle->GetDecayTable()->Insert(mode_minus);
            pmanager->AddProcess(theDecayProcess);
            pmanager->SetProcessOrdering(theDecayProcess, idxPostStep);
            pmanager->SetProcessOrdering(theDecayProcess, idxAtRest);
        }
        if (theCerenkovProcess->IsApplicable(*particle)) {
            G4cout << "Add Cerenkov process to " << particleName << G4endl;
            //            pmanager->AddProcess(theCerenkovProcess);
            pmanager->SetProcessOrdering(theCerenkovProcess, idxPostStep);
        }
        if (particleName == "opticalphoton") {
            G4cout << " AddDiscreteProcess to OpticalPhoton " << G4endl;
            pmanager->AddDiscreteProcess(new G4OpAbsorption());
            pmanager->AddDiscreteProcess(new G4OpRayleigh());
            pmanager->AddDiscreteProcess(new G4OpBoundaryProcess());
        }
    }


    G4cout << "Optics constructed" << G4endl;
}


void PhysicsList::SetVerbose(G4int verbose)
{
    theCerenkovProcess->SetVerboseLevel(verbose);

}

