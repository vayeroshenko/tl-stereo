/*
 * RunAction.cpp
 *
 *  Created on: Oct 2, 2018
 *      Author: vsevolod
 */

#include "RunAction.h"


RunAction::RunAction() {
    //	_outputFileName = "data.root";
    timer = new G4Timer();


    G4cout << "Run action constructor" << G4endl;
}

RunAction::~RunAction() {
    delete tree;
    delete hfile;
}

void RunAction::BeginOfRunAction(const G4Run* run)
{

    G4cout << "BeginOfRunAction" << G4endl;

    timer->Start();

    // Histogramming

    hfile = new TFile(_outputFileName, "RECREATE", "LHCb Luminometer Simulation Data", 1);

    G4cout << "Output file created" << G4endl;
    if (hfile->IsZombie()) exit(-1);
    tree = new TTree("T", "STEREO Prototype Simulation Data");
    //	tree->SetAutoSave(1000000);

    // Create new event
    TTree::SetBranchStyle(0);

    // Branches filled for each EVENT

    // Event ID
    tree->Branch("EventID",  &_EventID,  "EventID/I");

    tree->Branch("muInitPosX");
    tree->Branch("muInitPosY");
    tree->Branch("muInitPosZ");
    tree->Branch("muInitDirX");
    tree->Branch("muInitDirY");
    tree->Branch("muInitDirZ");

    tree->Branch("muInitEnergy");

    tree->Branch("muIsDecay");

    tree->Branch("pmt1nPhot");
    tree->Branch("pmt2nPhot");

    tree->Branch("pmt1T[pmt1nPhot]");
    tree->Branch("pmt1motherID[pmt1nPhot]");

    tree->Branch("pmt2T[pmt2nPhot]");
    tree->Branch("pmt2motherID[pmt2nPhot]");

    // Number of photons detected in each sector
    tree->Branch("nPhot", _nPhot, "nPhot[nSec]/I");

    // Branches filled for each HIT (commented due to "optimization")


    G4cout << "BeginOfRunAction end" << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run* )
{
    //	hfile = tree->GetCurrentFile();

    G4cout << "End of run action" << G4endl;
    tree->Write();
    hfile->Write();
    tree->Print();
    timer->Stop();
    hfile->Close();

    G4cout << "Time: " << *timer << G4endl;

}
