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

    tree->Branch("muInitPosX", &_muInitPosX, "muInitPosX/F");
    tree->Branch("muInitPosY", &_muInitPosY, "muInitPosY/F");
    tree->Branch("muInitPosZ", &_muInitPosZ, "muInitPosZ/F");
    tree->Branch("muInitDirX", &_muInitDirX, "muInitDirX/F");
    tree->Branch("muInitDirY", &_muInitDirY, "muInitDirY/F");
    tree->Branch("muInitDirZ", &_muInitDirZ, "muInitDirZ/F");


    tree->Branch("muPDGid", &_muPDGid, "muPDGid/I");

    tree->Branch("muInitEnergy", &_muInitEnergy, "muInitEnergy/F");
//    tree->Branch("muDecayEnergy");

    tree->Branch("muIsDecay", &_muIsDecay, "muIsDecay/I");
    tree->Branch("muDecayTime", &_muDecayTime, "muDecayTime/F");

    tree->Branch("pmt1nPhot", &_pmt1nPhot, "pmt1nPhot/I");
    tree->Branch("pmt2nPhot", &_pmt2nPhot, "pmt2nPhot/I");
    tree->Branch("nPhot", &_nPhot, "nPhot/I");

    // Number of photons detected in each sector

    // Branches filled for each HIT

    tree->Branch("photonTime", _photonTime, "photonTime[nPhot]/F");
    tree->Branch("photonWavelength", _photonWavelength, "photonWavelength[nPhot]/F");
    tree->Branch("photonParentID" , _photonParentID, "photonParentID[nPhot]/I");
    tree->Branch("photonDetectorID", _photonDetectorID, "photonDetectorID[nPhot]/I");


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
