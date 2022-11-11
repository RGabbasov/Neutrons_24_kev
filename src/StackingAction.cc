#include "StackingAction.h"
//#include "ConfigArray.h"

#include "RunAction.h"
#include "EventAction.h"
#include "SteppingAction.h"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4TransportationManager.hh"

#include <math.h>

StackingAction::StackingAction(RunAction* runact, EventAction* evact, SteppingAction* stepact )
{
runaction = runact;
eventaction = evact;
stepaction=stepact;
navigator = G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking();
incr=0;
}

StackingAction::~StackingAction()
{
}

G4ClassificationOfNewTrack StackingAction::ClassifyNewTrack(const G4Track* Track)
{
G4ClassificationOfNewTrack classification = fWaiting;
//G4ClassificationOfNewTrack classification = fUrgent;

G4ParticleDefinition* particle = Track->GetDefinition();
G4String pname = particle->GetParticleName();
G4String ptype = particle->GetParticleType();
//if (pname=="mu-")
 //eventaction->SetParticleEnergy  (Track->GetKineticEnergy());
//if (pname=="mu-")
	
	

if ((pname=="deuteron") || (pname=="triton") || (pname=="He3") || (pname=="alpha") || (pname=="gamma"))
    {
    classification = fKill;
    eventaction->UnelasticFlag();
    }

if ((0==1) && ((pname=="e-") || (pname=="e+")))
    {
    classification = fKill;
    //eventaction->AddEnergyDeposit(Track->GetKineticEnergy());
    }
if (pname=="opticalphoton")
{
//incr++;
//G4cout<<"Photon"<<G4endl;
//if (incr==50000)
//{
//incr=0;
//stepaction-> Susp();
//stackManager->ReClassify();
//G4double time=Track->GetGlobalTime()/microsecond;
eventaction->incphotonnumber();
//phenergy=(Track->GetKineticEnergy());
//G4String vol= Track->GetVolume()->GetLogicalVolume()->GetName();
//G4cout <<"volume "<<time <<G4endl;
//runaction->AddPhotonsignal(phenergy/eV,time);

//classification = fKill;
//}
//else { }
// Adding particle to our tree
}
if (pname=="neutron") 
    {
	//G4double ParticleEnergy=Track->GetKineticEnergy();
 	//eventaction->SetInitialEnergy(ParticleEnergy);
	//G4cout <<"initial "<<ParticleEnergy<<G4endl;
   }
return classification;
}

