#include "SteppingAction.h"

#include "DetectorConstruction.hh"
#include "EventAction.h"
#include "G4ParticleDefinition.hh"

//#include "G4Track.hh"
#include "G4VPhysicalVolume.hh"
//#include "G4NavigationHistory.hh"  // Needed to make the way of gettig the local position work
//#include "globals.hh"

SteppingAction::SteppingAction (DetectorConstruction* det, EventAction* evt)
{
detector = det;
eventaction = evt;
state=0;
flag=1;
photnum=0;
}

SteppingAction::~SteppingAction()
{

}

void SteppingAction::UserSteppingAction(const G4Step* step)
{
G4TouchableHandle handle,handle1;
G4double destep ;
G4String phname,phname1;
G4ParticleDefinition* particle =step->GetTrack()->GetDefinition();
G4String pname = particle->GetParticleName();
handle = step->GetPreStepPoint()->GetTouchableHandle();
phname=handle->GetVolume()->GetName();





if  (pname=="neutron")
	{
	
//G4ThreeVector point = step->GetPostStepPoint()->GetPosition();
G4double En = (step->GetTrack()->GetDynamicParticle()->GetKineticEnergy())/MeV;

//G4cout <<"vol "<<phname<<"  "<<(strncmp(phname, "liq0", 4)!=0)<< G4endl;
//G4cout<<G4endl;

//G4cout <<"uugugu   "<<flag<<G4endl;	
	//if ((point.z()<-3.5*cm) && (point.z()>-5.7*cm) && (point.x()>-7.2*cm) *(point.x()<7.2*cm) && (point.y()<(100+7,2)*cm) && (point.y()>(100-7,2)*cm))
	if  ((strncmp(phname, "World", 5)==0))
		{
		state=0;
		//flag=0;
		}
	if  ((strncmp(phname, "defense", 5)==0) || (strncmp(phname, "liq_phys", 7)==0))
		{
	//G4cout <<"final "<<phname<<G4endl;}
		state=1;
		//flag=1;
		}
	if (state>0)
		{
		handle1 = step->GetPostStepPoint()->GetTouchableHandle();
		phname1 = handle1->GetVolume()->GetName();
		}
	if  ((strncmp(phname1, "liq0", 4)==0))
		{
		state=2;
			eventaction->SetInitialEnergy(En);
			//G4cout <<"ss_initial  "<<phname1<<G4endl;
			flag=2;
			
		//G4cout <<"initial "<<phname<<G4endl;	
		}
	if ((strncmp(phname, "liq0", 4)==0) && (strncmp(phname1, "liq0", 4)!=0) && (state==2))
		{
		state=1;
		//G4cout <<"finalyy "<<phname1<<G4endl;
		eventaction->SetFinalEnergy(En);
		step->GetTrack()->SetTrackStatus(fStopAndKill);
		}
	//
/*	 
	
	 if (flag==1)
		{ //destep = step->GetTotalEnergyDeposit();
		//G4cout <<destep<<"   dessip " <<flag<<G4endl;
	 	//eventaction->AddDeposit(destep);
		}
	}	else if (flag==1)
		{ 
		//G4cout <<En<<"   final" <<G4endl;
		flag=0;
		step->GetTrack()->SetTrackStatus(fStopAndKill);
		}*/
	}

if (pname=="opticalphoton")
	{
	
	//step->GetTrack()->SetTrackStatus(fStopAndKill);
	G4ThreeVector point1 = step->GetPreStepPoint()->GetPosition();
	G4int copy=handle->GetVolume()->GetCopyNo();
	
	if (point1.z()<-65*mm)
		step->GetTrack()->SetTrackStatus(fStopAndKill);
	//G4cout <<"fgggg  "<<point1<<G4endl;}
	//if  ((pname=="opticalphoton") && (strncmp(phname, "liq", 3)==0))
  	if ((strncmp(phname, "PTM", 3)==0) && (copy>0) )
    	 {
	G4double time=step->GetTrack()->GetGlobalTime();
	eventaction->incphotonnumber();
	eventaction->PMTsig(1,time,copy);
	//photnum++;
	
	
	 
	 }
	}

}
void SteppingAction::Susp()
{
//muontr->SetTrackStatus(fSuspend);
}






