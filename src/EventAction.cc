#include "EventAction.h"
#include "RunAction.h"

#include "G4Event.hh"
//#include "TNtuple.h"
#include "globals.hh"
#include <iostream>
//#include <fstream>
//#include <math.h>

EventAction::EventAction(RunAction* run) 
{
//G4cout << "rfnfr"<< "hqwjduhuieierfhwuie"<< G4endl;
runaction=run;
numberofphotons=0;

}

EventAction::~EventAction()
{
}

void EventAction::BeginOfEventAction(const G4Event* event)
{ 
 G4int j;
 sig_st.numberph=0;
 for (j=0;j<2000;j++)
	{  
	sig_st.sig0[j]=0; 
	sig_st.sig1[j]=0; 
	sig_st.sig2[j]=0; 
	sig_st.sig3[j]=0; 
	sig_st.sig4[j]=0; 
	sig_st.sig5[j]=0; 
	sig_st.sig6[j]=0; 
	}
	numberofphotons=0;
	sema=1;flag=1;
neutr.energy_i=0;
neutr.energy_f=0;
}
 
void EventAction::EndOfEventAction(const G4Event* event)
{ 
 G4int EventID = event->GetEventID() + 1;
// 
//runaction->AddParticle_inf(muon_st);
//G4cout <<"final "<<neutr.energy_f <<G4endl;
if ((sema>0 ) && (neutr.energy_i-neutr.energy_f>0))
	{ 
	runaction->AddEvent(neutr, sig_st);
	//runaction->AddPhotonsignal(); 
 	G4cout << "end of event. Photons: "<< numberofphotons<< G4endl;
	}	else
	G4cout << "Unelastic"<<G4endl;
}


void EventAction::AddMuoninf (G4double destep, G4ThreeVector point, G4double time)
{
muon_st.deposit[inc]=destep;
muon_st.X[inc]=point.x();
muon_st.Y[inc]=point.y();
muon_st.Z[inc]=point.z();
muon_st.T[inc]=time;
inc++;
	//G4cout<<"dfghdfghdfgh "<<destep<<" "<<point<<G4endl;
}

void EventAction::SetInitialEnergy(G4double ParticleEnergy)
{
	if (flag)
	{
	neutr.energy_i = ParticleEnergy / MeV;
	G4cout <<"initial"<<   ParticleEnergy/ MeV<<G4endl;
	flag=0;
	}
}

void EventAction::SetFinalEnergy(G4double ParticleEnergy)
{
neutr.energy_f = ParticleEnergy / MeV;
}

void EventAction::incphotonnumber() 
{
numberofphotons++;
sig_st.numberph++;
}

void EventAction::PMTsig(G4double energy, G4double time, G4int copy)
{
G4double Time=time;
//G4double al[7]={1.,1.,1.,1.,1.,1.,1.};
G4int j;
G4double gap=5*nanosecond;

for (j=0; j<3000;j++)
 if ((Time>j*gap) && (Time<(j+1)*gap))
  { 
	switch(copy-1) {
	case 0:	sig_st.sig0[j]+=1;break;
	case 1:	sig_st.sig1[j]+=1;break;
	case 2:	sig_st.sig2[j]+=1;break;
	case 3:	sig_st.sig3[j]+=1;break;
	case 4:	sig_st.sig4[j]+=1;break;
	case 5:	sig_st.sig5[j]+=1;break;
	case 6:	sig_st.sig6[j]+=1;break;
 	default : G4cout<<"ne tyda popal "<<copy<<G4endl;
			}
	
  } 
}
void EventAction::AddDeposit(G4double dep)
{
//G4cout <<dep<<"   dessip "<<G4endl;
//neutr.deposit=neutr.deposit+dep;
}

void EventAction::UnelasticFlag()
{ 
sema=0;
}
/*
G4int EventAction::Getsus()
{
return sus
};*/






