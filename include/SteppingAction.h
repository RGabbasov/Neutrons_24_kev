#ifndef SteppingAction_header
#define SteppingAction_header

#include "G4UserSteppingAction.hh"
#include "globals.hh"
#include "G4Track.hh"

class DetectorConstruction;
class EventAction;
class PMTSignalCalculator;

class SteppingAction : public G4UserSteppingAction
{
    public:
	SteppingAction(DetectorConstruction*, EventAction*) ;
	~SteppingAction();

	void UserSteppingAction(const G4Step*);
	void Susp();
    private:
	DetectorConstruction* detector;
	EventAction* eventaction;
	G4int flag,state;
	G4int photnum;
	G4Track* muontr;
	//G4string volname;
};

#endif

