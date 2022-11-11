#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UIterminal.hh"
#include "G4UItcsh.hh"
#include "G4VisExecutive.hh"

#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "PrimaryGeneratorAction.hh"

#include "RunAction.h"
#include "EventAction.h"
#include "SteppingAction.h"
#include "StackingAction.h"

#include<G4Material.hh>
#include<G4UserRunAction.hh>
#include<G4Run.hh>
#include<iostream>
#include<string>
#include<CLHEP/Random/Random.h>
#include<unistd.h>
#include<time.h>

#include "SteppingVerbose.hh"  //22

#include<SensitiveDetector.hh>
using namespace std;

const char macros[]="vis2.mac";

int main(int argc,char** argv)
{
	//G4VSteppingVerbose::SetInstance(new SteppingVerbose);
	//HepRandom::setTheSeed(time(0)+getpid());
	G4int visul=1;
	G4RunManager * runManager = new G4RunManager;
	DetectorConstruction* detector_c = new DetectorConstruction;
	runManager->SetUserInitialization(detector_c);

	G4VUserPhysicsList *p = new PhysicsList;
	runManager->SetUserInitialization(p);
	
        RunAction* runaction = new RunAction;
        runManager->SetUserAction(runaction);

        EventAction* eventaction = new EventAction(runaction);
        runManager->SetUserAction(eventaction);  
	
	SteppingAction* stepaction=new SteppingAction(detector_c, eventaction);
	runManager->SetUserAction(stepaction);  
        //runManager->SetUserAction(new SteppingAction(detector_c, eventaction));
        //runManager->SetUserAction(new StackingAction(runaction, eventaction));
	runManager->SetUserAction(new StackingAction(runaction, eventaction, stepaction));
	
	runManager->SetUserAction(new PrimaryGeneratorAction(eventaction));

	runManager->Initialize();

	G4VisManager* visManager;
	if (visul>0) 
	{
	visManager = new G4VisExecutive;
	visManager->Initialize();
	}	
      
	G4UImanager * UI = G4UImanager::GetUIpointer();  
	
	
	UI->ExecuteMacroFile(macros);
	G4UIsession* session = new G4UIterminal(new G4UItcsh);
	session->SessionStart();
	delete session;
	delete visManager;
	delete runManager;

	return 0;
}
