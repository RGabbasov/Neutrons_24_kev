#ifndef StackingAction_header
#define StackingAction_header

#include "G4UserStackingAction.hh"
#include "globals.hh"
//#include "G4SteppingAction.hh"
#include "G4Track.hh"
#include "G4Navigator.hh"
//#include "SteppingAction.h"




class RunAction;
class EventAction;
class SteppingAction;

class StackingAction: public G4UserStackingAction
{
    public:
	StackingAction(RunAction*, EventAction*, SteppingAction*);
	~StackingAction();
	G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track*);
 	G4double phenergy;
    
    private:
	RunAction* runaction;
	EventAction* eventaction;
	SteppingAction* stepaction;
	G4Navigator* navigator;
	G4int incr; 
};

#endif
