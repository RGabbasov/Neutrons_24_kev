#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "EventAction.h"

class G4GeneralParticleSource;
class G4Event;

 class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    PrimaryGeneratorAction(EventAction*);    
   ~PrimaryGeneratorAction();
   void GeneratePrimaries(G4Event*);

  private:

    G4GeneralParticleSource* ParticleGun;
    EventAction* eventaction;
 
};


#endif


