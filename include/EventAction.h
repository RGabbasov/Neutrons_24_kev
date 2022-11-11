#ifndef EventAction_header
#define EventAction_header

#include "G4UserEventAction.hh"
#include "G4ThreeVector.hh"
#include "globals.hh"
//#include<iostream>
//#include<fstream>
typedef struct {
G4int numberph;
G4int sig0[2000];
G4int sig1[2000];
G4int sig2[2000];
G4int sig3[2000];
G4int sig4[2000];
G4int sig5[2000];
G4int sig6[2000];
} Signal_st;


typedef struct {
G4double energy;
G4double deposit[100];
G4double X[100];
G4double Y[100];
G4double Z[100];
G4double T[100];
} Muon_inf;

typedef struct {
G4double energy_i;
G4double energy_f;

} Neutron_inf;

class RunAction;

class EventAction: public G4UserEventAction
{
    public:
	EventAction(RunAction*);
	~EventAction();
	
	void BeginOfEventAction(const G4Event*);
	void EndOfEventAction(const G4Event*);	

	void incphotonnumber();
	
	void PMTsig (G4double,G4double,G4int);
	void SetInitialEnergy(G4double);
	void SetFinalEnergy(G4double);
	void UnelasticFlag();
	void AddMuoninf(G4double ,G4ThreeVector,G4double); 

	void AddDeposit(G4double);

inline void AddUntrackedSecPhotons(const G4int val) 
                                                 {untrackedSecPhotons += val;}
	
    private:

	RunAction* runaction;
	G4int numberofphotons;
        G4int untrackedSecPhotons;
            int inc;
	Signal_st sig_st;
	Muon_inf muon_st;	
	Neutron_inf neutr;
	G4int sema,flag;
	G4double tfirst,tlast;
};

#endif

