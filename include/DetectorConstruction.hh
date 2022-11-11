#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include<globals.hh>
#include<G4PVPlacement.hh>
#include<G4VUserDetectorConstruction.hh>
#include<G4VSolid.hh>
#include<G4LogicalVolume.hh>
#include<G4VPhysicalVolume.hh>
#include<G4Material.hh>
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4OpticalSurface.hh"

class World {
protected:
  G4VSolid *solid;
  G4LogicalVolume *logic;
  G4VPhysicalVolume *physic;
  G4Material *mater;
  double sizex, sizey, sizez;
public:
  World(double size_x, double size_y, double size_z, G4Material *mater_=NULL);
  operator G4LogicalVolume*() {return logic;}
  G4LogicalVolume *getLogic() {return logic;}
  G4VSolid *getSolid() {return solid;}
  G4VPhysicalVolume *getPhysic() {return physic;}
};

class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
  DetectorConstruction();
  ~DetectorConstruction();
 G4VPhysicalVolume* Construct();
  
protected:
  World *world;		// Самый внешний объем
private:	
	G4Material* vacuum;
	G4Material* ssteel;
	G4Material* steel;
	G4Material* glass;
	G4Material* LXe;
	G4Material* GXe;
	G4double sigalpha;
//logical volumes	corpus
  G4LogicalVolume* vedro_log;
  G4LogicalVolume *flan_log;
  G4LogicalVolume *ring_log;
// rings in liquid
G4LogicalVolume *ring1g_log;
G4LogicalVolume *ring1l_log;
G4LogicalVolume *ring2_log;
G4LogicalVolume *ring3_log;
// xenon
  G4LogicalVolume *gas_log;
  G4LogicalVolume *gas0_log;
  G4LogicalVolume *liq_log;
  G4LogicalVolume *liq0_log;

  G4LogicalVolume *mirror_log;
  G4LogicalVolume *disp_log;
  G4LogicalVolume* PTM_log;
//defense
 G4LogicalVolume *defense_log;
 G4PVPlacement *defense_phys;

 //physical volumes 
 G4PVPlacement *vedro_phys;
//flanetz top and bottom
 G4PVPlacement *flan1_phys;
 G4PVPlacement *flan2_phys;
 //ring top and bottom
 G4PVPlacement *ringa_phys;
 G4PVPlacement *ringb_phys;
//gas xenon volume
 G4PVPlacement *gas_phys;
 G4PVPlacement *gas0_phys;
 //liquid xenon volume
 G4PVPlacement *liq_phys;
 G4PVPlacement *liq0_phys;
//rings in liquid
 G4PVPlacement *ring1g_phys;
 G4PVPlacement *ring1l_phys;
 G4PVPlacement *ring2a_phys;
 G4PVPlacement *ring2b_phys;
 //G4PVPlacement *ring1l__phys;
 G4PVPlacement *ring3_phys;
 //mirror disk
 G4PVPlacement *mirror_phys;
//bolvanka
 G4PVPlacement *disp_phys;
//PTMs
 G4PVPlacement *PTM0_phys;
 G4PVPlacement *PTM1_phys;
 G4PVPlacement *PTM2_phys;
 G4PVPlacement *PTM3_phys;
 G4PVPlacement *PTM4_phys;
 G4PVPlacement *PTM5_phys;
 G4PVPlacement *PTM6_phys;

	G4OpticalSurface* OpAnoGasSurface;
	G4LogicalBorderSurface*  AnoGasSurface; 
	G4OpticalSurface* OpLiqBoardSurface;
	G4LogicalBorderSurface* LiqBoardSurface ; 
	G4OpticalSurface* OpGasSurface;
	G4LogicalBorderSurface* AGasSurface;
};

#endif
