#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UIterminal.hh"

#include "QGSP_BERT.hh"
#include "G4PhysListFactory.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#ifdef G4UI_USE_TCSH
#include "G4UItcsh.hh"
#endif

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif


#include "DetectorConstruction.h"
#include "PrimaryGeneratorAction.h"

#include "RunAction.h"
#include "EventAction.h"
#include "SteppingAction.h"
#include "PhysicsList.h"
#include "StackingAction.hh"


#include "Randomize.hh"

int main(int argc, char** argv)
{

	G4UIExecutive* ui = 0;
    if ( argc == 1 ) {
		ui = new G4UIExecutive(argc, argv);
	}
	G4long myseed = 345354;

	G4Random::setTheEngine(new CLHEP::RanecuEngine);
	G4Random::setTheSeed(myseed);

	// Run manager initialization
	G4RunManager* runManager = new G4RunManager;

    DetectorConstruction* detector = new DetectorConstruction;
	runManager->SetUserInitialization(detector);


    // QGSP_BERT Physics list with Optical processes (HEP, used by ATLAS)

//    G4VModularPhysicsList* physicsList = new QGSP_BERT;
    G4VModularPhysicsList* physicsList = new PhysicsList();
    physicsList->SetVerboseLevel(0);
	runManager->SetUserInitialization(physicsList);



    RunAction* runAction = new RunAction;
    if (argc == 3) runAction->SetOutputFileName(G4String(argv[2]));
    runManager->SetUserAction(runAction);

    PrimaryGeneratorAction* genAction = new PrimaryGeneratorAction();
	runManager->SetUserAction(genAction);

    SteppingAction* stepAction = new SteppingAction(genAction);
	runManager->SetUserAction(stepAction);

    StackingAction* stackingAction = new StackingAction();

    EventAction* eventAction = new EventAction(runAction, stepAction);
    eventAction->SetPrimGenerator(genAction);
    eventAction->SetStackingAction(stackingAction);


	runManager->SetUserAction(eventAction);
    detector->SetEventAction(eventAction);



	G4VisManager* visManager = new G4VisExecutive;
	// G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
	// G4VisManager* visManager = new G4VisExecutive("Quiet");
	visManager->Initialize();

	G4UImanager* UImanager = G4UImanager::GetUIpointer();


	if ( ! ui ) {
		// batch mode
		G4String command = "/control/execute ";
		G4String fileName = argv[1];
		UImanager->ApplyCommand(command+fileName);
	}
	else {
		// interactive mode
		UImanager->ApplyCommand("/control/execute init_vis.mac");
		if (ui->IsGUI()) {
			UImanager->ApplyCommand("/control/execute gui.mac");
		}
		ui->SessionStart();
		delete ui;
	}







	return 0;
}
