#ifndef SENSITIVEDETECTOR
#define SENSITIVEDETECTOR

#include<G4VSensitiveDetector.hh>
#include "RunAction.h"
#include "EventAction.h"
#include "TTree.h"
#include "TBranch.h"
#include "TFile.h"

class G4Step;
//class G4TouchableHistory;


class SensitiveDetector: public G4VSensitiveDetector
{
private:
       TFile* file;

	TTree* EventTree;
	TBranch* EventBranch;
          
	TTree* ParticleTree;
	TBranch* ParticleDataBranch;
	TBranch* ParticleNameBranch;
	TBranch* ParticleMaterialNameBranch;

public:
  SensitiveDetector(G4String name);
  ~SensitiveDetector();
  //bool ProcessHits(G4Step *step, G4TouchableHistory *hist);
  void EndOfEvent(G4HCofThisEvent*);
};

#endif /* SENSITIVEDETECTOR */
