// ************************************************************************
//
// ZepIII-V05 (ZepIIIElectroLuminescence)
//
// Henrique Araujo, Imperial College London
// 
// ************************************************************************
//
// Generation of secondary photons in the gas phase according to the
// energy deposited in the liquid. In the absence of E field map, the
// photon vertex is located above PostStepPoint, with vertical coordinate
// randomly distributed in the gas thickness. If a field map is loaded
// (default), the energy deposited in each step is tracked along the
// field in the liquid with a field-dependent drift speed, and then along
// the gas phase.
//
// ************************************************************************
//
// HISTORY
// 11/01/2006: migrated from ZepIII-V03
// 18/01/2006: introduced ionisation Fano factor: sigma_Ne=sqrt(F*Ne)
// 18/01/2006: resolutionScale applies only to elum: sigma_Y=R*sqrt(Y)


#include "ZepIIIElectroLuminescence.hh"

//#include "ZepIIIElectroLuminescenceMessenger.hh"
#include "ZepIIIFieldMap.hh"

#include "EventAction.h"
#include "G4EventManager.hh"

#include "G4OpticalPhoton.hh"
#include "G4DynamicParticle.hh"
#include "G4ParticleMomentum.hh"
#include "G4ParticleDefinition.hh"

//#include "G4PhysicalVolumeStore.hh"
//#include "G4VPhysicalVolume.hh"
//#include "G4LogicalVolume.hh"
//#include "G4VSolid.hh"
//#include "G4Tubs.hh"

#include "Randomize.hh"
#include "G4Poisson.hh"
#include "G4ios.hh"
#include <cmath>

#include "G4UImanager.hh"
#include "G4VVisManager.hh"
#include "G4Point3D.hh"
#include "G4Polyline.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

#include <fstream>


ZepIIIElectroLuminescence::ZepIIIElectroLuminescence
  (const G4String& processName) : G4VRestDiscreteProcess(processName) {

  G4cout << GetProcessName() << " is created " << G4endl;

  
  // create messenger
 // elumMessenger = new ZepIIIElectroLuminescenceMessenger(this);

  // grab geometrical parameters
  FindGasGeometry();

  // default electroluminescence parameters
  eYield_a        = 0; //30.07;
  eYield_b        = 0.40;
  eYield_c        = 1.0;
  WeValue         = 15.6*eV; //Cham...you did this for the veto.  Default value was '15.6*eV;'
  //
  aYield_a        = 0.;
  aYield_b        = 100.;
  aYield_c        = 0.;
  WaValue         = WeValue/0.22; //Cham...you did this for the veto.  Default value was 'WeValue/0.22;'
  //
  nYield_a        = 0;
  nYield_b        = 2.1;//1.0;
  nYield_c        = 1.0;
  WnValue         = WeValue/0.22; //Cham...you did this for the veto.  Default value was 'WeValue/0.22;'
  //
  stepSize        = 2.5*mm;
  liqDriftSpeed   = 2.5*mm/microsecond;
  ext_a           = 1.1;
  ext_b           = 0.017;
  ext_c           = 3.55;
  //
  extractionScale = 1.;
  //
  gasPcold        = 2.5*bar;
  //
  gasDriftSpeed   = 2.7*mm/microsecond/(volt/cm/(bar/750.));
  //
  yield_a         = 70.;
  yield_b         = 56.;
  //  yield_a         = 137.;
  //  yield_b         = 125.;
  yieldScale      = 1.;
  resolutionScale = 1.;
  FanoFactor      = 1.;
  decayTime       = 20.*ns;
  noOpticsFlag    = false;
  photonEnergy    = 7.0*eV;
  // default field
  tolerance       = 50.*micrometer;
  voltage         = 12.*kilovolt/volt;
  field_liq       = 4.*kilovolt/cm;
  field_gas	  = 6.*kilovolt/cm;
  //
 // SetMap("maps/5mm_gas.map");


}

ZepIIIElectroLuminescence::~ZepIIIElectroLuminescence() {

//  DeleteMap();

  //delete elumMessenger;

}


// ..oo00oo..oo00oo..oo00oo..oo00oo..oo00oo..oo00oo..oo00oo..oo00oo..oo00oo..
// 00oo..oo00oo..oo00oo..oo00oo..oo00oo..oo00oo..oo00oo..oo00oo..oo00oo..ooOO

// G4Process

G4double ZepIIIElectroLuminescence::GetMeanFreePath
   (const G4Track&, G4double, G4ForceCondition* condition) {

  *condition = StronglyForced;
  
  return DBL_MAX;
  
}


G4double ZepIIIElectroLuminescence::GetMeanLifeTime
   (const G4Track&, G4ForceCondition* condition) {

  *condition = StronglyForced;

  return DBL_MAX;  

}


// 00oo..oo00oo..oo00oo..oo00oo..oo00oo..oo00oo..oo00oo..oo00oo..oo00oo..ooOO
// 00oo..oo00oo..oo00oo..oo00oo..oo00oo..oo00oo..oo00oo..oo00oo..oo00oo..ooOO

G4VParticleChange* ZepIIIElectroLuminescence::AtRestDoIt
   (const G4Track& aTrack, const G4Step& aStep) {

  return ZepIIIElectroLuminescence::PostStepDoIt(aTrack, aStep);
}


// ..oo00oo..oo00oo..oo00oo..oo00oo..oo00oo..oo00oo..oo00oo..oo00oo..oo00oo..
// 00oo..oo00oo..oo00oo..oo00oo..oo00oo..oo00oo..oo00oo..oo00oo..oo00oo..ooOO

G4VParticleChange* ZepIIIElectroLuminescence::PostStepDoIt
(const G4Track& aTrack, const G4Step& aStep) {
//G4cout <<"poststepdoit"<<G4endl;
G4double press=1;


  aParticleChange.Initialize(aTrack);

  // copy step point information (original point in liquid)
  G4StepPoint* theSPoint = new G4StepPoint;
  *theSPoint = *(aStep.GetPostStepPoint());
G4String dname=aTrack.GetDefinition()->GetParticleName();
G4String d2name=aStep.GetTrack()->GetDefinition()->GetParticleName();
  
  // quit if not in liquid field region
  if( !InsideLiquidField(theSPoint->GetPosition()) ) {
    delete theSPoint;
 //  G4cout <<"out of liquid     "<< theSPoint->GetPosition()<< G4endl;
    aParticleChange.SetNumberOfSecondaries(0);
    return G4VRestDiscreteProcess::PostStepDoIt(aTrack, aStep);
  }

 // quit if no gas
  if(GasThickness==0.) {
    delete theSPoint;
	  G4cout <<"No gas   "<<  G4endl;
    aParticleChange.SetNumberOfSecondaries(0);
    return G4VRestDiscreteProcess::PostStepDoIt(aTrack, aStep);
  } 

  // number of electrons generated in liquid which escape recombination;
  // data for electron tracks from Obodobski et al. 1979, parameterised
  // by D.Davidge; see aux/electron_yield_lxe.ps
  const G4DynamicParticle* theParticle = aTrack.GetDynamicParticle();
  G4String theParticleType = theParticle->GetDefinition()->GetParticleType();
  G4String theParticleName = theParticle->GetDefinition()->GetParticleName();
  // get field in liquid (either use constant field or get from field map) 		FIELD_LIQ

  // field = (voltage*volt)/(2.0*GasThickness+ZLiqGas);
G4double  field =field_liq; 
  // carrier extraction from track
G4cout <<" field in liquid kv/cm  "<<field/(kilovolt/cm)<<G4endl;

  G4double NumElectrons = 0.;
  if(theParticleType=="lepton" || theParticleName=="gamma" || theParticleName=="neutron")
   {
	
    // for electron tracks (gammas, electrons, muons)
    NumElectrons =(aStep.GetTotalEnergyDeposit()/WeValue) / 
      (eYield_a/(aTrack.GetVertexKineticEnergy()/keV)
       /(field/(kilovolt/cm)) + eYield_b/(field/(kilovolt/cm)) + eYield_c);}
  else if (theParticleName=="alpha")
    // for alpha particles
    NumElectrons = 
      (aStep.GetTotalEnergyDeposit()/WaValue) / 
      (aYield_a/(aTrack.GetVertexKineticEnergy()/keV)
       /(field/(kilovolt/cm)) + aYield_b/(field/(kilovolt/cm)) + aYield_c);
  else
    // for nuclear tracks (GenericIon, gps/ion)
	
    NumElectrons = (aStep.GetTotalEnergyDeposit()/WnValue) / 
      (nYield_a/(aTrack.GetVertexKineticEnergy()/keV)
       /(field/(kilovolt/cm)) + nYield_b/(field/(kilovolt/cm)) + nYield_c);
  G4cout <<"NumElectrons  "<<NumElectrons<<G4endl;
  // apply Poisson/Gaussian statistics to charge created
  if (NumElectrons>10) 
    NumElectrons = G4int(G4RandGauss::shoot
		     (NumElectrons,std::sqrt(FanoFactor*NumElectrons)));
  else
    NumElectrons = G4int(G4Poisson(NumElectrons));
  //
  // quit if none
  if(NumElectrons<1) {
    delete theSPoint;
    aParticleChange.SetNumberOfSecondaries(0);
    return G4VRestDiscreteProcess::PostStepDoIt(aTrack, aStep);
  }
  
  // HA DEBUG
  //std::ofstream file("charge.out", std::ios::app);
  //file << NumElectrons << G4endl;
  //return G4VRestDiscreteProcess::PostStepDoIt(aTrack, aStep);
  // HA DEBUG

G4cout<<"lumin"<<G4endl;
  // drift PostStepPoint up to liquid surface
  theSPoint = DriftInLiquid(theSPoint);


  // quit if surface not reached
  if(!InsideGasField(theSPoint->GetPosition())) {
    delete theSPoint;
    aParticleChange.SetNumberOfSecondaries(0);
    return G4VRestDiscreteProcess::PostStepDoIt(aTrack, aStep);
  }
//G4cout <<"Numelectrons   "<<NumElectrons<<G4endl;
  // track at liquid surface: extraction to gas phase 
  // see aux/electron_extraction.ps
  G4double surfaceField = (voltage/volt)/(2.0*GasThickness+ZLiqGas);
 // G4cout <<"surface field  "<<surfaceField<<G4endl;

  //  G4ThreeVector point = theSPoint->GetPosition();
    // field point well inside liquid to avoid 
    // getting nearest map point in gas
    //point.setZ(ZLiqGas-tolerance);

 G4double Extraction = extractionScale; //* 
   // (1.-ext_a*std::exp(-ext_b*std::pow(surfaceField/(kilovolt/cm),ext_c)));
  if(Extraction<0) Extraction = 0.;
  

  if(verboseLevel>1)
    G4cout << " ZepIIIElectroLuminescence::DoIt" 
	   << " -- Edep: " << aStep.GetTotalEnergyDeposit()/keV << " keV"
	   << "; Free carriers: " << NumElectrons
	   << "; Extracted: " << Extraction*NumElectrons 
	   << G4endl;  



  // drift point in gas *****************************************************
  // (must navigate first to find total number of secondaries, in order to
  // free memory for the secondaries list with SetNumberOfSecondaries()!!!)

  // step points
  std::vector<G4ThreeVector> point;
  point.push_back(theSPoint->GetPosition());
  point[0].setZ(ZLiqGas);
  // step times
  std::vector<G4double> time;
  time.push_back((theSPoint->GetGlobalTime()));
  // NumPhotons in step (NumPhotons[0] stores total)
  std::vector<G4int> NumPhotons;
  NumPhotons.push_back(0);

  G4double Yield = 0.;
  G4double stepYield = 0.;
  size_t step = 0;
  G4double speed = 0.;

G4double time1=theSPoint->GetGlobalTime();
G4int numph=0;

G4ThreeVector point1=theSPoint->GetPosition();
  // navigation in gas *************************************************
  while(InsideGasField(point[step])) {
    // lookup electric field
	G4double field = field_gas;	//(voltage*volt)/(GasThickness+ZLiqGas/2.0);
        // drift speed in gas: linear approximation from 
	// F. P. Santos et al 1994 J. Phys. D: Appl. Phys 27, 42-48.
	speed = gasDriftSpeed * (field/(gasPcold*(293./173.)));
//G4cout <<"speedcheck  "<<gasDriftSpeed/bar*750/(mm/microsecond)<<"  "<<gasPcold*(293./173.)/bar<<G4endl;
      // navigate to new point
      time.push_back(time[step]+GasThickness/speed);
      point.push_back( point[step] );
      point[step+1].setZ(ZLiqGas+GasThickness); 
	// total number of secondary photons for this step
      	// see aux/elum_yield_gxe.ps,
      Yield = (yield_a*field/(kilovolt/cm)- yield_b*((gasPcold/bar)*(273./173.)));
      stepYield = Yield*(GasThickness/cm);
      stepYield += resolutionScale*G4RandGauss::shoot(0.,std::sqrt(stepYield));
      NumPhotons.push_back(int(0.5+yieldScale *
	(NumElectrons * Extraction * stepYield)/press));

      /*  G4cout <<"speed  "<<speed/(mm/microsecond)<<G4endl;
	G4cout <<"Yield  "<< Yield <<G4endl;
	G4cout <<"stepYield  "<< stepYield<<G4endl;
	G4cout <<"NumberPhotons  "<<numph<<" " <<step<<G4endl;
	G4cout <<"time  "<<time[step+1]<<G4endl;
	G4cout <<"GasTheckness  "<<GasThickness<<G4endl;*/

if(NumPhotons[step+1]>0)
	NumPhotons[0] += NumPhotons[step+1];
      else
	NumPhotons[step+1]=0;
 
    // plot step
   PlotStep(point[step], point[step+1], G4Colour(1,1,0));

    if(verboseLevel>2)
      G4cout << " DriftInGas - new pos: " << point[step+1]/mm 
	     << " mm; \ttime: " << time[step+1]/microsecond << " us" << G4endl;

    step++;

  } // while ***********************************************************

  // correct last point to gas/mirror boundary
  if( point[step].z() > (ZLiqGas + GasThickness) ) {
    point[step].setZ(ZLiqGas + GasThickness);
    G4double delta = ((point[step]-point[step-1]).mag())/stepSize;
    time[step] = time[step-1] + delta*(time[step]-time[step-1]);
    NumPhotons[0] -= NumPhotons[step];
    NumPhotons[step] = (int) (delta*NumPhotons[step]);
    NumPhotons[0] += NumPhotons[step];
    // verbose for debugging
    if(verboseLevel>2)
      G4cout << " DriftInGas - last point corrected to: " << G4endl
	     << " DriftInGas -     pos: " << point[step]/mm 
	     << " mm; \ttime: " << time[step]/microsecond << " us" << G4endl;
  }


  // correct last point to detector radius
  if( point[step].perp() > DetRadius )
    point[step].setPerp(DetRadius);


  // now final number of photons is known
 // if(verboseLevel>1) 
{
    G4cout << " ZepIIIElectroLuminescence::DoIt" 
	   << " -- Photons created: " 
	   <<  NumPhotons[0];
    if(yieldScale!=1.0) G4cout << "; yieldScale: " << yieldScale; 
    G4cout << G4endl;
    if(noOpticsFlag)
      G4cout << " ZepIIIElectroLuminescence::DoIt" 
	     << " -- NO PHOTONS TRACKED!" << G4endl;
  }


  


  // if not tracking, pass total number of photons to ZepIIIEventAction
  if(noOpticsFlag) {
    EventAction* theUserEventAction = (EventAction*)
      G4EventManager::GetEventManager()->GetUserEventAction();
    theUserEventAction->AddUntrackedSecPhotons(NumPhotons[0]);
    delete theSPoint;
    aParticleChange.SetNumberOfSecondaries(0);
    return G4VRestDiscreteProcess::PostStepDoIt(aTrack, aStep);
  }


//G4cout <<"secondaries"<<G4endl;
  // secondaries *******************************************************
  aParticleChange.SetNumberOfSecondaries(NumPhotons[0]);

  // cycle over steps in gas
  for(step=1; step<NumPhotons.size(); ++step) {

    // cycle over secondary photons in step
    for (G4int i=0; i<NumPhotons[step]; i++) {
      // position and time of new photon (random along step)
G4double rand = G4UniformRand();

      G4ThreeVector aSecondaryPosition = point[step-1]
	+ rand * (point[step]-point[step-1]);
	if (step==1)
		 G4ThreeVector aSecondaryPosition = point[step-1];
      G4double aSecondaryTime = time[step-1] 
	+ rand * (time[step]-time[step-1]) - decayTime*log(G4UniformRand());
      // random momentum direction
      G4double cost = 1.*G4UniformRand();
      G4double sint = std::sqrt((1.-cost)*(1.+cost));
      G4double phi = 2*pi*G4UniformRand();
      G4double sinp = std::sin(phi);
      G4double cosp = std::cos(phi);
      G4ParticleMomentum photonMomentum(sint*cosp, sint*sinp, cost);
	//if (step==1)
	//G4cout<<"phot "<<NumPhotons[1]<<"  "<<aSecondaryTime<<G4endl;
	
      // polarization
//G4cout <<"photon momentum  "<<photonMomentum<<G4endl;
      G4ThreeVector photonPol(cost*cosp, cost*sinp, -sint);
      G4ThreeVector perp = photonMomentum.cross(photonPol);
      phi = 2*pi*G4UniformRand();
      sinp = std::sin(phi);
      cosp = std::cos(phi);
      photonPol = cosp * photonPol + sinp * perp;
      photonPol = photonPol.unit();
      // generate new photon
      G4DynamicParticle* aSecondaryPhoton = new G4DynamicParticle
	(G4OpticalPhoton::OpticalPhoton(), photonMomentum);
      aSecondaryPhoton->SetKineticEnergy(photonEnergy);
      aSecondaryPhoton->SetPolarization
	(photonPol.x(), photonPol.y(), photonPol.z());
      // generate new G4Track
      G4Track* aSecondaryTrack = new G4Track
	(aSecondaryPhoton, aSecondaryTime, aSecondaryPosition);
      aSecondaryTrack->SetTouchableHandle((G4VTouchable*)0);
      aSecondaryTrack->SetParentID(aTrack.GetTrackID());
      aParticleChange.AddSecondary(aSecondaryTrack);
    } // for (loop secondaries)

  } // for (loop steps)

  delete theSPoint;

  return G4VRestDiscreteProcess::PostStepDoIt(aTrack, aStep);

}


// ..oo00oo..oo00oo..oo00oo..oo00oo..oo00oo..oo00oo..oo00oo..oo00oo..oo00oo..
// 00oo..oo00oo..oo00oo..oo00oo..oo00oo..oo00oo..oo00oo..oo00oo..oo00oo..ooOO

// This method avoids passing the detector pointer to 
// ZepIIIPhysicsList and from there to ZepIIIElectroLuminescence
void ZepIIIElectroLuminescence::FindGasGeometry(void) {


 /* G4PhysicalVolumeStore *physStore = G4PhysicalVolumeStore::GetInstance();
  G4VPhysicalVolume* aVolume = NULL;
  

  for (G4int i=physStore->size()-1; i>=0; --i) {
    aVolume = (*physStore)[i];
    if(aVolume->GetName()=="gas_phys")
      gasThick  = 2*((G4Tubs*)aVolume->GetLogicalVolume()
		     ->GetSolid())->GetZHalfLength();
    else if(aVolume->GetName()=="mir_phys")
      mirThick  = 2*((G4Tubs*)aVolume->GetLogicalVolume()
		     ->GetSolid())->GetZHalfLength();
    else if(aVolume->GetName()=="liq_phys") {
      liqOff    = aVolume->GetObjectTranslation().z();
      liqThick  = 2*((G4Tubs*)aVolume->GetLogicalVolume()
		     ->GetSolid())->GetZHalfLength();
    } 
   
 //else if(aVolume->GetName()=="ano_rin_gas_phys")
       //((G4Tubs*)aVolume->GetLogicalVolume() ->GetSolid())->GetInnerRadius();
  }*/
G4double gasThick=0, mirThick=0, liqThick=0, liqOff=0;
DetRadius =5.2*cm;
gasThick=0.5*cm;
liqThick=2.2*cm;
liqOff=-4.6*cm;
ZLiqGas=-3.5*cm;

  GasThickness = gasThick;
 // ZLiqGas      = 0.5*liqThick - gasThick + liqOff;
  
// if(verboseLevel>1) {
    G4cout << " GasThickness: " << GasThickness/mm<< G4endl;
    G4cout << " DetRadius   : " << DetRadius/mm << G4endl;
    G4cout << " ZLiqGas     : " << ZLiqGas/mm << G4endl;
  //}

}


// ..oo00oo..oo00oo..oo00oo..oo00oo..oo00oo..oo00oo..oo00oo..oo00oo..oo00oo..
// 00oo..oo00oo..oo00oo..oo00oo..oo00oo..oo00oo..oo00oo..oo00oo..oo00oo..ooOO


/*
void ZepIIIElectroLuminescence::TouchMap(G4ThreeVector point) {

  G4ThreeVector field;
  G4double constField;
  G4double redField;
  G4double speed;

  if( !InsideLiquidField(point) && !InsideGasField(point))
    G4cout << "ZepIIIElectroLuminescence::GetField"
	   << "-- Point outside field region!" << G4endl;

  // field map present
  if(aFieldMap) {
    if(InsideLiquidField(point)) {
      field = voltage*(aFieldMap->GetField(point));
      speed = liqDriftSpeed * 
	      (1.-std::exp(-0.061*std::pow(field.mag()/(volt/cm),0.48)));
      G4cout << "Point in Liquid: E=" << field.mag()/(volt/cm) 
	     << " V/cm;  (Ex,Ey,Ez)=" << field/(volt/cm) 
	     << ";  Drift Speed: "  << speed/(mm/microsecond) << " mm/us" 
	     << G4endl;
    }
    else if(InsideGasField(point)) {
      field    = voltage*(aFieldMap->GetField(point));
      redField = field.mag()/(gasPcold*(293./173.));
      speed    = gasDriftSpeed * redField;
      G4cout << "Point in Gas: E=" << field.mag()/(volt/cm) 
	     << " V/cm;  (Ex,Ey,Ez)=" << field/(volt/cm) << G4endl
	     << "Pcold=" << gasPcold/bar << " bar; " 
	     << "P273K=" << gasPcold/bar*(273./173.) << " bar; "
	     << "P293K=" << gasPcold/bar*(293./173.) << " bar" << G4endl
	     << "Er=" << redField/(volt/cm/(bar/750.)) << " V/cm/torr; " 
	     << "Drift Speed: " << speed/(mm/microsecond) << " mm/us; " 
	     << "S2 width=" << (GasThickness/speed)/microsecond << " us"
	     << G4endl;
    }
  }
  // using constant field
  else {
    G4cout << "ZepIIIElectroLuminescence::GetField"
	   << " -- No map in memory! Using constant field." << G4endl;
    if(InsideLiquidField(point)) {
      constField = (voltage*volt)/(2.0*GasThickness+ZLiqGas);
      speed = liqDriftSpeed * 
	      (1.-std::exp(-0.061*std::pow(constField/(volt/cm),0.48)));
      G4cout << "Point in Liquid: E=" << constField/(volt/cm) 
	     << " V/cm; Drift Speed: " << speed/(mm/microsecond) << " mm/us" 
	     << G4endl;
    }
    else if(InsideGasField(point)) {
      constField = (voltage*volt)/(GasThickness+ZLiqGas/2.0);
      redField   = constField/(gasPcold*(293./173.));
      speed      = gasDriftSpeed * redField;
      G4cout << "Point in Gas: E=" << constField/(volt/cm) << " V/cm" << G4endl
	     << "Pcold=" << gasPcold/bar << " bar; " 
	     << "P273K=" << gasPcold/bar*(273./173.) << " bar; "
	     << "P293K=" << gasPcold/bar*(293./173.) << " bar" << G4endl
	     << "Er=" << redField/(volt/cm/(bar/750.)) << " V/cm/torr; "
	     << "Drift Speed: " << speed/(mm/microsecond) << " mm/us; " 
	     << "S2 width=" << (GasThickness/speed)/microsecond << " us"
	     << G4endl;
    }
  }
}*/
/*
void ZepIIIElectroLuminescence::PlotMap(void) {

  if(!G4VVisManager::GetConcreteInstance()) {
    G4cout << "ZepIIIElectroLuminescence::PlotMap" 
	   << " -- Visualisation is off!" << G4endl;
    return;
  }

  if(!aFieldMap) {
    G4cout << "ZepIIIElectroLuminescence::PlotMap"
	   << " -- No map in memory!" << G4endl;
    return;
  }

  // set viewer
 // G4UImanager::GetUIpointer()->ApplyCommand("/vis/scene/notifyHandlers");
 // G4UImanager::GetUIpointer()
 //   ->ApplyCommand("/vis/viewer/set/viewpointThetaPhi 90.0001 90");

  G4double spacing = 3.*mm;
  G4StepPoint* theSPoint = new G4StepPoint;

  // lines from Z=0
  for(G4double r=0; r<DetRadius; r+=spacing) {
    theSPoint->SetPosition(G4ThreeVector(r, 0., tolerance));
    theSPoint = DriftInLiquid(theSPoint);
    theSPoint = DriftInGas(theSPoint);
  }

  // some extra lines from R=DetRadius
  for(G4double z=0; z<0.5*(ZLiqGas+GasThickness); z+=spacing) {
    theSPoint->SetPosition(G4ThreeVector(DetRadius-tolerance, 0., z));
    theSPoint = DriftInLiquid(theSPoint);
    theSPoint = DriftInGas(theSPoint);
  }

  delete theSPoint;

}*/


// ..oo00oo..oo00oo..oo00oo..oo00oo..oo00oo..oo00oo..oo00oo..oo00oo..oo00oo..
// 00oo..oo00oo..oo00oo..oo00oo..oo00oo..oo00oo..oo00oo..oo00oo..oo00oo..ooOO

// Navigation in field for liquid phase
G4StepPoint* ZepIIIElectroLuminescence::DriftInLiquid(G4StepPoint* sPoint) {
//G4cout <<"dirftuuuuuuuuuuuuuuuuuu"<<G4endl;
  G4ThreeVector thePoint = sPoint->GetPosition();
  G4ThreeVector newPoint = thePoint;
  G4double time = sPoint->GetGlobalTime();
  G4double speed;

  // navigation in liquid
  //while(InsideLiquidField(thePoint)) 
{
    
      // Drift speed in liquid 
      // Scale down saturated drift velocity in liquid as a function
      // of field; parameterised from L.S.Miller, S. Howe and W.E. Spear,
      // Phys. Rev. 166(3), 871 (1968). see aux/drift_velocity.ps
      speed = liqDriftSpeed; //* (1.-std::exp(-0.061*std::pow(field_liq/(volt/cm),0.48)));
             // (1.-std::exp(-0.061*std::pow(field.mag()/(volt/cm),0.48)));	
      // Navigate to new point
      newPoint.setZ(ZLiqGas);
      time += (ZLiqGas-thePoint.z()) / speed;
    }

    // plot step
    PlotStep(thePoint, newPoint, G4Colour(1,1,1));

    // verbose for debugging
    if(verboseLevel>2)
      G4cout << " DriftInLiq - new pos: " << newPoint/mm 
	     << " mm; \ttime: " << time/microsecond << " us" << G4endl;

    thePoint = newPoint;


  // correct last point to liq/gas boundary
  if(thePoint.z() > ZLiqGas) {
    thePoint.setZ(ZLiqGas);
    time -= (thePoint-newPoint).mag()/speed;
    // verbose for debugging
    if(verboseLevel>1)
      G4cout << " DriftInLiq - last point corrected to: " << G4endl
	     << " DriftInLiq -     pos: " << thePoint/mm 
	     << " mm; \ttime: " << time/microsecond << " us" << G4endl;
  }

  sPoint->SetPosition(thePoint);
  sPoint->AddGlobalTime(time);
  
  return sPoint;

}

// Navigation in field for gas phase (for plotMap only!!!)
G4StepPoint* ZepIIIElectroLuminescence::DriftInGas(G4StepPoint* sPoint) {

  // leave if surface not reached
  if(sPoint->GetPosition().z()<ZLiqGas) return sPoint;

  G4ThreeVector thePoint = sPoint->GetPosition();
  thePoint.setZ(ZLiqGas);
  G4ThreeVector newPoint = thePoint; 
  
  do {   // navigation in gas

      newPoint.setZ(ZLiqGas+GasThickness);
    // plot step
    PlotStep(thePoint, newPoint, G4Colour(1,1,0));

    thePoint = newPoint;

  } while(InsideGasField(thePoint));

  
  sPoint->SetPosition(thePoint);

  return sPoint;

}

// plot step 
void ZepIIIElectroLuminescence::PlotStep(const G4ThreeVector start,
  const G4ThreeVector end, G4Colour colour) {

  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();

  if(pVVisManager) {
    G4VisAttributes attribs(colour);
    G4Polyline polyline;
    polyline.SetVisAttributes(attribs);
    polyline.push_back(G4Point3D(start));
    polyline.push_back(G4Point3D(end));
    pVVisManager->Draw(polyline);
  }
  
}
