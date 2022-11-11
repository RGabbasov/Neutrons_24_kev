#include "PrimaryGeneratorAction.hh"
#include "EventAction.h"

#include "G4ParticleTypes.hh"
#include "G4GeneralParticleSource.hh"
#include "G4SPSAngDistribution.hh"
#include "G4SPSEneDistribution.hh"
#include "G4SPSPosDistribution.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4Event.hh"



PrimaryGeneratorAction::PrimaryGeneratorAction(EventAction* evtact)
{
eventaction=evtact;
ParticleGun = new G4GeneralParticleSource();

G4SPSEneDistribution* EnergyDist = ParticleGun->GetCurrentSource()->GetEneDist();
G4SPSAngDistribution* AngularDist = ParticleGun->GetCurrentSource()->GetAngDist();
G4SPSPosDistribution* PositionDist = ParticleGun->GetCurrentSource()->GetPosDist();

//ParticleGun->SetParticleDefinition(G4Gamma::GammaDefinition());
//ParticleGun->SetParticleDefinition(G4Electron::ElectronDefinition());
//ParticleGun->SetParticleDefinition(G4OpticalPhoton::OpticalPhotonDefinition());
//ParticleGun->SetParticleDefinition(G4MuonMinus::MuonMinusDefinition());
//ParticleGun->SetParticleDefinition(G4Alpha::AlphaDefinition());
ParticleGun->SetParticleDefinition(G4Neutron::NeutronDefinition());

EnergyDist->SetEnergyDisType("Mono");EnergyDist->SetMonoEnergy(2.5*MeV);

//EnergyDist->SetEnergyDisType("Lin");
//EnergyDist-> SetEmin(100*MeV);
//EnergyDist-> SetEmax(300*MeV);
//EnergyDist-> SetGradient(2);
//EnergyDist->SetInterCept(200*MeV);

//PositionDist->SetPosDisType("Point");

//AngularDist->SetAngDistType("iso");


PositionDist->SetPosDisType("Plane");
PositionDist->SetPosDisShape("Circle");
PositionDist->SetRadius(3*cm);
PositionDist->SetCentreCoords(G4ThreeVector(0, -100*cm, -5*cm));
//PositionDist->SetCentreCoords(G4ThreeVector(0.*cm, 40.*cm, -4*cm));
AngularDist->SetAngDistType("iso");

//AngularDist->SetMaxTheta(1.077);
//AngularDist->SetMinTheta(1.077);
AngularDist->SetMaxTheta(pi/2+1/7);
AngularDist->SetMinTheta(pi/2-1/7);
AngularDist->SetMaxPhi(pi/2+1/7);
AngularDist->SetMinPhi(pi/2-1/7);
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* event)
{
ParticleGun->GeneratePrimaryVertex(event);
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
delete ParticleGun;
}




