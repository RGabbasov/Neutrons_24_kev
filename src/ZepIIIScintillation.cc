// ************************************************************************
//
// ZepIII-V05 (ZepIIIScintillation)
// adapted from G4Scintillation (P. Gumplinger /TRIUMF)
//
// Henrique Araujo, Imperial College London
// 
// ************************************************************************
//
// HISTORY
// 16/06/2005: migrated from ZepIII-V04

#include "ZepIIIScintillation.hh"

//#include "ZepIIIScintillationMessenger.hh"

#include "G4ThreeVector.hh"
#include "G4ParticleMomentum.hh"
#include "G4OpticalPhoton.hh"
#include "G4VPhysicalVolume.hh" 
#include "G4Poisson.hh"
#include "Randomize.hh"
#include "G4ios.hh"
#include <cmath>


ZepIIIScintillation::ZepIIIScintillation
   (const G4String& processName) : G4VRestDiscreteProcess(processName) {

  G4cout << GetProcessName() << " is created " << G4endl;

  // create messenger
  //scintMessenger = new ZepIIIScintillationMessenger(this);

  // default scintillation parameters
  liqElectronYield     = 60.0*0.3; // field quenching
  liqElectronDecayTime = 26.0*ns;  // 
  liqNuclearYield      = 60.0*0.2; // 0.2 quenching factor
  liqNuclearDecayTime  = 16.0*ns;  //
  liqResolutionScale   = 1.0;

  gasYield             = liqElectronYield;
  gasDecayTime         = liqElectronDecayTime;
  gasResolutionScale   = liqResolutionScale;

  photonEnergy         = 7.0*eV;

  s0=0.3;
  field=4.0*kilovolt/cm;
  k=2.1*kilovolt/cm;
}


ZepIIIScintillation::~ZepIIIScintillation() {

  //delete scintMessenger;

}


// ..oo00oo..oo00oo..oo00oo..oo00oo..oo00oo..oo00oo..oo00oo..oo00oo..oo00oo..
// 00oo..oo00oo..oo00oo..oo00oo..oo00oo..oo00oo..oo00oo..oo00oo..oo00oo..ooOO

// G4Process

G4double ZepIIIScintillation::GetMeanFreePath
    (const G4Track&, G4double, G4ForceCondition* condition) {

  *condition = StronglyForced;

  return DBL_MAX;

}

G4double ZepIIIScintillation::GetMeanLifeTime
    (const G4Track&, G4ForceCondition* condition) {

  *condition = Forced;

  return DBL_MAX;
}


// ..oo00oo..oo00oo..oo00oo..oo00oo..oo00oo..oo00oo..oo00oo..oo00oo..oo00oo..
// 00oo..oo00oo..oo00oo..oo00oo..oo00oo..oo00oo..oo00oo..oo00oo..oo00oo..ooOO


G4VParticleChange*
ZepIIIScintillation::AtRestDoIt(const G4Track& aTrack, const G4Step& aStep) {

  return ZepIIIScintillation::PostStepDoIt(aTrack, aStep);

}

// ..oo00oo..oo00oo..oo00oo..oo00oo..oo00oo..oo00oo..oo00oo..oo00oo..oo00oo..
// 00oo..oo00oo..oo00oo..oo00oo..oo00oo..oo00oo..oo00oo..oo00oo..oo00oo..ooOO


G4VParticleChange*
ZepIIIScintillation::PostStepDoIt(const G4Track& aTrack, const G4Step& aStep) {
G4int press=1;
  aParticleChange.Initialize(aTrack);

  // scintillation properties according to particle and material
  const G4DynamicParticle* theParticle = aTrack.GetDynamicParticle();
  G4String theParticleType = theParticle->GetDefinition()->GetParticleType();
  G4String theParticleName = theParticle->GetDefinition()->GetParticleName();
  G4String thePhysVolume = aTrack.GetVolume()->GetName();

  G4double ScintillationYield = 0;
  G4double ScintillationTime  = 0;
  G4double ResolutionScale    = 0;
  if(thePhysVolume=="liq0_phys") {
    // scintillation in liquid Xenon
    if(theParticleType=="lepton" || theParticleName=="gamma") {
      // for electron tracks (gammas, electrons, muons)
      ScintillationYield = liqElectronYield;
      ScintillationTime  = liqElectronDecayTime;
      ResolutionScale    = liqResolutionScale;
    } else {
      // for nuclear tracks (GenericIon, gps/ion, protons, alphas, etc)
      ScintillationYield = liqNuclearYield;
      ScintillationTime  = liqNuclearDecayTime;
      ResolutionScale    = liqResolutionScale;
    }
  } else if (thePhysVolume=="gas0_phys") {
    // scintillation in Xenon gas
    ScintillationYield   = gasYield;
    ScintillationTime    = gasDecayTime;
    ResolutionScale      = gasResolutionScale;
  } else {
    // not in scintillator
    aParticleChange.SetNumberOfSecondaries(0);
    return G4VRestDiscreteProcess::PostStepDoIt(aTrack, aStep);
  }


  if (verboseLevel>1)
    G4cout << " ZepIIIScintillation::DoIt "
	   << " -- Volume: " << thePhysVolume
	   << "; particle: " << theParticleName
	   << "; yield: " << ScintillationYield << " ph/keV"
	   << "; tau: " << ScintillationTime/ns << " ns" << G4endl;


  // step information
  G4StepPoint* pPreStepPoint  = aStep.GetPreStepPoint();
  G4StepPoint* pPostStepPoint = aStep.GetPostStepPoint();
  G4ThreeVector x0 = pPreStepPoint->GetPosition();
  G4ThreeVector p0 = aStep.GetDeltaPosition().unit();
  G4double      t0 = pPreStepPoint->GetGlobalTime();


  // number of photons
  G4double TotalEnergyDeposit = aStep.GetTotalEnergyDeposit();
  G4double MeanNumPhotons = TotalEnergyDeposit/keV * ScintillationYield*(s0+(1-s0)*(1-1/(1+k/field))) ;
  // Gaussian distribution above 10 photons
  G4double sigma = ResolutionScale * std::sqrt(MeanNumPhotons);
  G4int NumPhotons = G4int(0.5+(G4RandGauss::shoot(MeanNumPhotons, sigma))/press);  // <-------
  // Poisson distribution below 10 photons
  if(MeanNumPhotons <= 10. && ResolutionScale != 0.0)
    NumPhotons = G4int(G4Poisson(MeanNumPhotons));
  // quit if none
  if (NumPhotons <= 0) {
    aParticleChange.SetNumberOfSecondaries(0);
    return G4VRestDiscreteProcess::PostStepDoIt(aTrack, aStep);
  }
  aParticleChange.SetNumberOfSecondaries(NumPhotons);
//NumPhotons=0;

  // secondaries
  for (G4int i=0; i<NumPhotons; i++) {
    // position and time of new photon
    G4double rand = G4UniformRand();
    if (theParticleName=="gamma") rand = 1.0;
    G4double delta = rand * aStep.GetStepLength();
    G4double deltaTime = delta /
      ((pPreStepPoint->GetVelocity()+pPostStepPoint->GetVelocity())/2.);
    deltaTime = deltaTime - ScintillationTime * std::log( G4UniformRand() );
    G4double aSecondaryTime = t0 + deltaTime;
    G4ThreeVector aSecondaryPosition = x0 + rand * aStep.GetDeltaPosition();
    // random momentum direction
    G4double cost = 1. - 2.*G4UniformRand();
    G4double sint = std::sqrt((1.-cost)*(1.+cost));
    G4double phi = 2*pi*G4UniformRand();
    G4double sinp = std::sin(phi);
    G4double cosp = std::cos(phi);
    G4ParticleMomentum photonMomentum(sint*cosp, sint*sinp, cost);
    // polarization
    G4ThreeVector photonPol(cost*cosp, cost*sinp, -sint);
    G4ThreeVector perp = photonMomentum.cross(photonPol);
    phi = 2*pi*G4UniformRand();
    sinp = std::sin(phi);
    cosp = std::cos(phi);
    photonPol = cosp * photonPol + sinp * perp;
    photonPol = photonPol.unit();
    // generate a new photon
    G4DynamicParticle* aScintillationPhoton = new G4DynamicParticle
      (G4OpticalPhoton::OpticalPhoton(), photonMomentum);
    aScintillationPhoton->SetKineticEnergy(photonEnergy);
    aScintillationPhoton->SetPolarization
      (photonPol.x(), photonPol.y(), photonPol.z());
    // Generate new G4Track
    G4Track* aSecondaryTrack = 
      new G4Track(aScintillationPhoton,aSecondaryTime,aSecondaryPosition);
    aSecondaryTrack->SetTouchableHandle((G4VTouchable*)0);
    aSecondaryTrack->SetParentID(aTrack.GetTrackID());
    aParticleChange.AddSecondary(aSecondaryTrack);
  }


  //if (verboseLevel>0)
    G4cout << " ZepIIIScintillation::DoIt "
	   << " -- Photons created: " 
	   << aParticleChange.GetNumberOfSecondaries() << G4endl;

 
  return G4VRestDiscreteProcess::PostStepDoIt(aTrack, aStep);

}


