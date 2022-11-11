// ************************************************************************
//
// ZepIII-V05.1 (ZepIIIPhysicsList)
//
// Henrique Araujo, Imperial College London
// 
// ************************************************************************
//
// HISTORY
// 15/11/2005: migrated from ZepIII-V05 to run with geant4.8.1.p02
// 15/11/2006: added G4StepLimiter to charged hadrons
// 15/11/2006: constructing all particles


#include "PhysicsList.hh"

//#include "PhysicsListMessenger.hh"

#include "G4ProcessManager.hh"
#include "G4ProcessVector.hh"

#include "G4ParticleDefinition.hh"
#include "G4ParticleWithCuts.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"

#include "G4UnitsTable.hh"
#include "G4ios.hh"


#include "globals.hh"
#include "G4StepLimiter.hh"
// Constructor /////////////////////////////////////////////////////////////
PhysicsList::PhysicsList() : G4VUserPhysicsList() {

  // range cuts
  defaultCutValue     = 0.5*mm;
  cutForGamma         = defaultCutValue;
  cutForElectron      = defaultCutValue;
  cutForPositron      = defaultCutValue;

  // verbosity
  VerboseLevel = 1;
  SetVerboseLevel(VerboseLevel);

  // physics messenger
  //physicsMessenger = new PhysicsListMessenger(this);

}


// Destructor //////////////////////////////////////////////////////////////
PhysicsList::~PhysicsList() {

//  delete physicsMessenger;

}



/////////////////////////////////////////////////////////////////////////////
// Construct Particles /////////////////////////////////////////////////////
#include "G4BosonConstructor.hh"
#include "G4LeptonConstructor.hh"
#include "G4MesonConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4IonConstructor.hh"
#include "G4ShortLivedConstructor.hh"

void PhysicsList::ConstructParticle() {

  G4BosonConstructor      consBos; consBos.ConstructParticle();
  G4LeptonConstructor     consLep; consLep.ConstructParticle();
  G4MesonConstructor      consMes; consMes.ConstructParticle();
  G4BaryonConstructor     consBar; consBar.ConstructParticle();
  G4IonConstructor        consIon; consIon.ConstructParticle();
  G4ShortLivedConstructor consShL; consShL.ConstructParticle();

}


/////////////////////////////////////////////////////////////////////////////
// Construct Processes //////////////////////////////////////////////////////
void PhysicsList::ConstructProcess() {


  G4cout << "\nConstructing physics processes..." << G4endl;

  AddTransportation();

  ConstructElectromagnetic();

  ConstructDecays();

  ConstructOptical();

  // register neutron processes by UserInterface only!
  ConstructHadronic();

}


/////////////////////////////////////////////////////////////////////////////
// Transportation ///////////////////////////////////////////////////////////
void PhysicsList::AddTransportation() {

  G4VUserPhysicsList::AddTransportation();

}


/////////////////////////////////////////////////////////////////////////////
// Electromagnetic Processes ////////////////////////////////////////////////
// all charged particles
#include "G4MultipleScattering.hh"

// gamma
#include "G4LowEnergyRayleigh.hh" 
#include "G4LowEnergyPhotoElectric.hh"
#include "G4LowEnergyCompton.hh"  
#include "G4LowEnergyGammaConversion.hh" 

// e-
#include "G4LowEnergyIonisation.hh" 
#include "G4LowEnergyBremsstrahlung.hh" 

// e+
#include "G4eIonisation.hh" 
#include "G4eBremsstrahlung.hh" 
#include "G4eplusAnnihilation.hh"

// muons
#include "G4MuIonisation.hh"
#include "G4MuBremsstrahlung.hh"
#include "G4MuPairProduction.hh"
#include "G4MuonMinusCaptureAtRest.hh"

// hadrons
#include "G4hIonisation.hh"
#include "G4ionIonisation.hh"
#include "G4StepLimiter.hh"

void PhysicsList::ConstructElectromagnetic() {

  // processes
  G4LowEnergyPhotoElectric* lowePhot = 
    new G4LowEnergyPhotoElectric("LEPhotoElectric");
  lowePhot->SetCutForLowEnSecPhotons(250*eV);
   lowePhot->ActivateAuger(true);

  G4LowEnergyIonisation* loweIon  = 
    new G4LowEnergyIonisation("LEIonisation");
  loweIon ->SetCutForLowEnSecPhotons(250*eV);
   loweIon->ActivateAuger(true);

  G4LowEnergyBremsstrahlung* loweBrem = 
    new G4LowEnergyBremsstrahlung("LEBremsstrahlung");
  loweBrem->SetCutForLowEnSecPhotons(250*eV);


  theParticleIterator->reset();
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    G4String particleName      = particle->GetParticleName();
    G4String particleType      = particle->GetParticleType();
    G4double particleCharge    = particle->GetPDGCharge();

   if (particleName == "gamma") {
      // gamma
      pmanager->AddDiscreteProcess(lowePhot);
      pmanager->AddDiscreteProcess(new G4LowEnergyRayleigh("LERayleigh"));
      pmanager->AddDiscreteProcess(new G4LowEnergyCompton("LECompton"));
      pmanager->AddDiscreteProcess(new G4LowEnergyGammaConversion("LEConversion"));

    } else if (particleName == "e-") {
      // electron
      G4MultipleScattering* aMultipleScattering = 
	new G4MultipleScattering("MultipleScattering");
      pmanager->AddProcess(aMultipleScattering,     -1, 1, 1);
      pmanager->AddProcess(loweIon,                 -1, 2, 2);
      pmanager->AddProcess(loweBrem,                -1,-1, 3);
      
    } else if (particleName == "e+") {
      // positron
      G4MultipleScattering* aMultipleScattering = 
	new G4MultipleScattering("MultipleScattering");
      pmanager->AddProcess(aMultipleScattering,     -1, 1, 1);
      pmanager->AddProcess(new G4eIonisation
			   ("eplusIonisation"),     -1, 2, 2);
      pmanager->AddProcess(new G4eBremsstrahlung
			   ("eplusBremsstrahlung"), -1,-1, 3);
      pmanager->AddProcess(new G4eplusAnnihilation
			   ("eplusAnnihilation"),    0,-1, 4);

    } else if( particleName == "mu+" || 
               particleName == "mu-"    ) {
      // muon
      G4MultipleScattering* aMultipleScattering = 
	new G4MultipleScattering("MultipleScattering");
      pmanager->AddProcess(aMultipleScattering,     -1, 1, 1);
      pmanager->AddProcess(new G4MuIonisation
			   ("MuIonisation"),        -1, 2, 1);
      pmanager->AddProcess(new G4MuBremsstrahlung
			   ("MuBremsstrahlung"),    -1,-1, 3);
      pmanager->AddProcess(new G4MuPairProduction
			   ("MuPairProduction"),    -1,-1, 4);	
      pmanager->AddProcess(new G4StepLimiter(), -1, -1, 3);
	
      if( particleName == "mu-" )
	pmanager->AddProcess(new G4MuonMinusCaptureAtRest
			   ("MuCaptureAtRest"),      5,-1,-1);

    } else if (particleName == "GenericIon") {
      // ions
      pmanager->AddProcess(new G4MultipleScattering
			   ("MultipleScattering"),  -1, 1, 1);
      pmanager->AddProcess(new G4ionIonisation
 			   ("IonIonisation"),       -1, 2, 2);
      pmanager->AddProcess(new G4StepLimiter(),     -1,-1, 3);
      
    } else  if (!particle->IsShortLived() &&
	       particleCharge != 0. && 
	       particleName   != "chargedgeantino") {
      // all other stable charged particles
      pmanager->AddProcess(new G4MultipleScattering
			   ("MultipleScattering"),  -1, 1, 1);
      pmanager->AddProcess(new G4hIonisation
			   ("HadronIonisation"),    -1, 2, 2); 
      pmanager->AddProcess(new G4StepLimiter(),     -1,-1, 3);
    }
   
  }

}


/////////////////////////////////////////////////////////////////////////////
// Optical Processes ////////////////////////////////////////////////////////

#include "G4OpAbsorption.hh"
#include "G4OpRayleigh.hh"
#include "G4OpBoundaryProcess.hh"

#include "ZepIIIScintillation.hh"
#include "ZepIIIElectroLuminescence.hh"

void PhysicsList::ConstructOptical() {


  // optical processes
  G4OpAbsorption* theAbsorptionProc = new G4OpAbsorption("OpAbsorption");
  theAbsorptionProc->SetVerboseLevel(VerboseLevel-2);

      G4OpRayleigh* theOpRayleighProc = new G4OpRayleigh("OpRayleigh");
      theOpRayleighProc->SetVerboseLevel(VerboseLevel-2);

  G4OpBoundaryProcess* theBoundaryProc = new G4OpBoundaryProcess("OpBoundary");
  theBoundaryProc->SetVerboseLevel(VerboseLevel-2);

   //prompt scintillation in LXe/GXe (primary signal)
  ZepIIIScintillation* thePrimary = new ZepIIIScintillation("Scintillation");
  thePrimary->SetVerboseLevel(VerboseLevel-1);

   //electroluminescence in GXe (secondary signal)
  ZepIIIElectroLuminescence* theSecondary = 
    new ZepIIIElectroLuminescence("ElectroLuminescence");
  theSecondary->SetVerboseLevel(VerboseLevel -1);

  theParticleIterator->reset();
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    G4String particleName = particle->GetParticleName();

    // scintillation
    if (particleName!="opticalphoton" && 
	particleName!="neutron" &&
	particleName!="geantino") {
      pmanager->AddProcess(thePrimary);
     pmanager->SetProcessOrderingToLast(thePrimary,idxAtRest);
      pmanager->SetProcessOrderingToLast(thePrimary,idxPostStep);
    }

    // electroluminescence
   if (particleName!="opticalphoton" &&
      	particleName!="neutron" &&
      	particleName!="geantino") {
      pmanager->AddProcess(theSecondary);
      pmanager->SetProcessOrderingToLast(theSecondary,idxAtRest);
      pmanager->SetProcessOrderingToLast(theSecondary,idxPostStep);
    }

    // optical processes
    if (particleName == "opticalphoton") {
      pmanager->AddDiscreteProcess(theAbsorptionProc);
      pmanager->AddDiscreteProcess(theBoundaryProc);
        pmanager->AddDiscreteProcess(theOpRayleighProc);
    }
  }

}


/////////////////////////////////////////////////////////////////////////////
// Neutron Hadronic Processes //////////////////////////////////////////////
// call from UI only

// G4NeutronHP models using evaluated data libraries.
// http://geant4.web.cern.ch/geant4/G4UsersDocuments/UsersGuides
// /PhysicsReferenceManual/html/node75.html

// processes
#include "G4HadronElasticProcess.hh"
#include "G4HadronCaptureProcess.hh"
#include "G4NeutronInelasticProcess.hh"
#include "G4LElastic.hh"
// neutron HP models (E<19 MeV)
#include "G4NeutronHPElastic.hh"
#include "G4NeutronHPElasticData.hh"
#include "G4NeutronHPCapture.hh"
#include "G4NeutronHPCaptureData.hh"
#include "G4NeutronHPInelastic.hh"
#include "G4NeutronHPInelasticData.hh"


void PhysicsList::ConstructHadronic() {

  G4cout << "PhysicsList::ConstructHadronic"
	 << " -- Constructing neutron processes" << G4endl;

  G4ProcessManager* pManager = G4Neutron::Neutron()->GetProcessManager();

  // elastic scattering
  G4cout << "PhysicsList::ConstructHadronic"
	 << " -- G4NeutronHPElastic" << G4endl;
  // see aux/n_elastic_ang.ps, n_elastic_xe129_ang.ps for angular distribution
  // see aux/n_elastic_recoil.ps, n_elastic_xe129_recoil.ps for recoil energies
  G4HadronElasticProcess* theNeutronElasticProcess = 
    new G4HadronElasticProcess("neutronElastic");
  G4NeutronHPElastic* theNeutronElasticModel = new G4NeutronHPElastic;
  theNeutronElasticProcess->RegisterMe(theNeutronElasticModel);
  G4NeutronHPElasticData* theNeutronData1 = new G4NeutronHPElasticData;
  theNeutronElasticProcess->AddDataSet(theNeutronData1);
  pManager->AddDiscreteProcess(theNeutronElasticProcess);

 /* // inelastic scattering
  G4cout << "PhysicsList::ConstructHadronic"
	 << " -- G4NeutronHPInelastic" << G4endl;
  // see aux/n_inelastic_gammas.ps for spectrum of emitted gammas 
  G4NeutronInelasticProcess* theNeutronInelasticProcess =
    new G4NeutronInelasticProcess("neutronInelastic");
  G4NeutronHPInelastic* theNeutronInelasticModel = new G4NeutronHPInelastic;
  theNeutronInelasticProcess->RegisterMe(theNeutronInelasticModel);
  G4NeutronHPInelasticData* theNeutronData2 = new G4NeutronHPInelasticData;
  theNeutronInelasticProcess->AddDataSet(theNeutronData2);
  pManager->AddDiscreteProcess(theNeutronInelasticProcess);

  // capture
  G4cout << "PhysicsList::ConstructHadronic"
	 << " -- G4NeutronHPCapture" << G4endl;
  // see aux/n_capture_gammas.ps for spectrum of emitted gammas
  G4HadronCaptureProcess* theNeutronCaptureProcess =
    new G4HadronCaptureProcess("neutronCapture");
  G4NeutronHPCapture* theNeutronCaptureModel = new G4NeutronHPCapture;
  theNeutronCaptureProcess->RegisterMe(theNeutronCaptureModel);
  G4NeutronHPCaptureData* theNeutronData3 = new G4NeutronHPCaptureData;
  theNeutronCaptureProcess->AddDataSet(theNeutronData3);
  pManager->AddDiscreteProcess(theNeutronCaptureProcess);*/


}


/////////////////////////////////////////////////////////////////////////////
// Decays ///////////////////////////////////////////////////////////////////
#include "G4Decay.hh"
#include "G4RadioactiveDecay.hh"
#include "G4IonTable.hh"
#include "G4Ions.hh"

void PhysicsList::ConstructDecays() {

  // Add Decay Process
  G4Decay* theDecayProcess = new G4Decay();
  theParticleIterator->reset();
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();

    if (theDecayProcess->IsApplicable(*particle)) { 
      pmanager ->AddProcess(theDecayProcess);
      // set ordering for PostStepDoIt and AtRestDoIt
      pmanager ->SetProcessOrdering(theDecayProcess, idxPostStep);
      pmanager ->SetProcessOrdering(theDecayProcess, idxAtRest);
    }
  }

  // Declare Radioactive Decay to the GenericIon in the IonTable.
  const G4IonTable *theIonTable = 
    G4ParticleTable::GetParticleTable()->GetIonTable();
  
  G4RadioactiveDecay *theRadioactiveDecay = 
    new G4RadioactiveDecay("RadioactiveDecay");
  
  for (G4int i=0; i<theIonTable->Entries(); i++) {
    G4String particleName = theIonTable->GetParticle(i)->GetParticleName();
    if (particleName == "GenericIon") {
      G4ProcessManager* pmanager = 
	theIonTable->GetParticle(i)->GetProcessManager();
      pmanager->SetVerboseLevel(VerboseLevel);
      pmanager ->AddProcess(theRadioactiveDecay);
      pmanager ->SetProcessOrdering(theRadioactiveDecay, idxPostStep);
      pmanager ->SetProcessOrdering(theRadioactiveDecay, idxAtRest);
    } 
  }
}


/////////////////////////////////////////////////////////////////////////////
// Cuts /////////////////////////////////////////////////////////////////////
void PhysicsList::SetCuts() {

  if (verboseLevel>0){
    G4cout << "PhysicsList::SetCuts - DefaultCutLength : " 
	   << G4BestUnit(defaultCutValue,"Length") << G4endl;
  }

  // set cuts
  G4double lowlimit=1*eV;
  G4ProductionCutsTable::GetProductionCutsTable()
    ->SetEnergyRange(lowlimit, 1.*GeV);

  SetCutValue(cutForGamma, "gamma");
  SetCutValue(cutForElectron, "e-");
  SetCutValue(cutForPositron, "e+");

  if (verboseLevel>0) DumpCutValuesTable();
}

void PhysicsList::SetVerbosities(G4int val) { 

  SetVerboseLevel(val);

}


// end //////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

