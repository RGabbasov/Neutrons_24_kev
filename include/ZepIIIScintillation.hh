// ************************************************************************
//
// ZepIII-V03 (ZepIIIScintillation)
//
// Henrique Araujo, Imperial College London
// 
// ************************************************************************

#ifndef ZepIIIScintillation_h
#define ZepIIIScintillation_h 1

#include "G4VRestDiscreteProcess.hh"
#include "G4DynamicParticle.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "globals.hh"

class ZepIIIScintillationMessenger;

class ZepIIIScintillation : public G4VRestDiscreteProcess {

  public:

    ZepIIIScintillation(const G4String& processName = "Scintillation");
  
    ~ZepIIIScintillation();	

  public:

    G4double GetMeanFreePath(const G4Track& aTrack,G4double,G4ForceCondition*);

    G4double GetMeanLifeTime(const G4Track& aTrack, G4ForceCondition*);

    G4VParticleChange* PostStepDoIt(const G4Track& aTrack,const G4Step& aStep);

    G4VParticleChange* AtRestDoIt (const G4Track& aTrack, const G4Step& aStep);


  // inlines
  public :

    inline G4bool IsApplicable(const G4ParticleDefinition& aParticleType)
       { if(aParticleType.GetParticleName()=="opticalphoton") return false;
       else return true; };

    inline void SetYieldLiqElec(const G4double val)
       { liqElectronYield = val; }

    inline void SetTimeLiqElec(const G4double val)
       { liqElectronDecayTime = val; }

    inline void SetResScaleLiq(const G4double val)
       { liqResolutionScale = val; }

    inline void SetYieldLiqNuc(const G4double val)
       { liqNuclearYield = val; }

    inline void SetTimeLiqNuc(const G4double val)
       { liqNuclearDecayTime = val; }

    inline void SetYieldGas(const G4double val)
       { gasYield = val; }

    inline void SetTimeGas(const G4double val)
       { gasDecayTime = val; }

    inline void SetResScaleGas(const G4double val)
       { gasResolutionScale = val; }
  


  private:

  // messenger
  ZepIIIScintillationMessenger* scintMessenger;

  // scintillation parameters
  G4double liqNuclearYield;
  G4double liqNuclearDecayTime;
  G4double liqResolutionScale;

  G4double liqElectronYield;
  G4double liqElectronDecayTime;

  G4double gasYield;
  G4double gasDecayTime;
  G4double gasResolutionScale;

  G4double photonEnergy;
  G4double s0;
  G4double field;
  G4double k;
};


#endif /* ZepIIIScintillation_h */
