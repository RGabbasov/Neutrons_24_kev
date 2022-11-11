#include"DetectorConstruction.hh"
//#include"SensitiveDetector.hh"
//#include<G4SDManager.hh>
#include<G4Box.hh>
#include<G4Tubs.hh>
#include "G4VisAttributes.hh"
#include<G4SubtractionSolid.hh>
#include<G4UnionSolid.hh>
#include "G4Transform3D.hh"
#include "G4UserLimits.hh"
using namespace std;

World::World(double size_x, double size_y, double size_z,  G4Material *mater_): mater(mater_), sizex(size_x), sizey(size_y), sizez(size_z)
{
	//double size05 = size/2;
	solid = new G4Box("world", sizex/2, sizey/2, sizez/2);
	logic = new G4LogicalVolume( solid, mater, "World", 0, 0, 0);
	physic = new G4PVPlacement(0, G4ThreeVector(), logic, "World",
				   0, false, 0); 
}

DetectorConstruction::DetectorConstruction()
{
}

DetectorConstruction::~DetectorConstruction()
{
}


G4VPhysicalVolume* DetectorConstruction::Construct()
{
// elements
  G4Element* Xe = new G4Element("Xenon",    "Xe", 54., 131.29*g/mole);
 // G4Element* Cu = new G4Element("Copper",   "Cu", 29., 63.55*g/mole);  
  G4Element* Si = new G4Element("Silicon",  "Si", 14., 28.09*g/mole);
  G4Element* O  = new G4Element("Oxygen",   "O",   8., 16.00*g/mole);
  G4Element* Fe = new G4Element("Iron",     "Fe", 26., 55.85*g/mole);
  G4Element* C  = new G4Element("Carbon",   "C",   6., 12.01*g/mole);
  G4Element* Co = new G4Element("Cobalt",   "Co", 27., 58.93*g/mole);
  //G4Element* Pb = new G4Element("lead",     "Pb", 82., 207.2*g/mole);
  G4Element* H  = new G4Element("Hydrogen", "H",   1., 1.0079*g/mole);
  G4Element* Mn = new G4Element("Manganese","Mn", 25., 54.938*g/mole);
  G4Element* P  = new G4Element("Phosphoros","P", 15., 30.9738*g/mole);
  G4Element* S  = new G4Element("Sulfur",    "S", 16., 32.06*g/mole);
  //G4Element* Gd = new G4Element("Gadolinium","Gd", 64., 157.25*g/mole);
  //G4Element* N = new G4Element("Nitrogen", "N", 7., 14.0067*g/mole);
  //G4Element* Cl = new G4Element("Chlorine", "Cl",17., 35.453*g/mole);
  //G4Element* Na = new G4Element("Sodium", "Na", 11., 22.9897*g/mole);

sigalpha =1; //0.01*deg;

//vacuum
  vacuum = new G4Material("Vacuum",1., 1.*g/mole, 1.e-20*g/cm3, kStateGas, 0.1*kelvin, 1.e-20*bar);

 // steel (AISI 1005) wall
  steel = new G4Material("steel", 7.872*g/cm3, 5);
  steel->AddElement(C, 0.0006);
  steel->AddElement(Fe,0.995);
  steel->AddElement(Mn,0.0035);
  steel->AddElement(P, 0.0004);
steel->AddElement(S, 0.0005);
 //steel->AddElement(Fe,1);
  // stainless steel floor
  ssteel = new G4Material("ssteel", 7.7*g/cm3, 3);
  ssteel->AddElement(C, 0.04);
  ssteel->AddElement(Fe, 0.88);
  ssteel->AddElement(Co, 0.08);

// glassware: window is Spectrosil B;
  glass = new G4Material("glass", 2.21*g/cm3, 2);
  glass->AddElement(Si, 1);
  glass->AddElement(O , 2);
//parafin
 G4Material *parafin=new G4Material("parafin",2.*g/cm3, 2);
parafin->AddElement(C, 1);
parafin->AddElement(H, 2);

// liquid xenon
  LXe = new G4Material("LXe",2.90*g/cm3, 1, kStateLiquid, 173*kelvin, 2.5*bar);
  LXe->AddElement(Xe, 1);

// xenon gas
  GXe = new G4Material("GXe", 13.5*mg/cm3, 1, kStateGas, 173.*kelvin, 1.4*bar);
  GXe->AddElement(Xe, 1);

//G4double steel_PP[3]   = {  6.5*eV,  7.*eV, 7.5*eV };
//G4double glass_PP[3]   = {  5.0*eV,   7.*eV,   7.50*eV };
//G4double LXe_PP[3]   = {  6.5*eV, 7.*eV, 7.5*eV };
//G4double GXe_PP[3]   = {  6.5*eV, 7.*eV, 7.5*eV };
G4double enegries [3]   = {  6.5*eV, 7.*eV, 7.5*eV };

G4double steel_REFL[3] = {    0.15,    0.15 ,0.15};
G4double mir_REFL[3] = { 0.86, 0.86 ,  0.86 };

G4double glass_RIND[3] = {    1.69,      1.69,      1.69 };
G4double glass_ABSL[3] = { 999.0*cm, 999.0*cm, 999.0*cm };

G4double LXe_RIND[3] = {    1.69,    1.69 ,1.69};
G4double LXe_ABSL[3] = { 36.4*cm, 36.4*cm, 36.4*cm };

G4double GXe_RIND[3] = {   1.004, 1.004,  1.004 };
G4double GXe_ABSL[3] = { 1000*cm, 1000*cm,1000*cm };

G4MaterialPropertiesTable* steel_surface = new G4MaterialPropertiesTable();
  steel_surface->AddProperty("REFLECTIVITY",  enegries, steel_REFL, 3);
  steel->SetMaterialPropertiesTable(steel_surface);

G4MaterialPropertiesTable* mir_surface = new G4MaterialPropertiesTable();
  mir_surface->AddProperty("REFLECTIVITY",  enegries, mir_REFL, 3);

 G4MaterialPropertiesTable* glass_mt = new G4MaterialPropertiesTable();
  glass_mt->AddProperty("RINDEX",     enegries, glass_RIND, 3);
  glass_mt->AddProperty("ABSLENGTH",  enegries, glass_ABSL, 3);
  glass->SetMaterialPropertiesTable(glass_mt);

// optical properties (V.Chepel et al IDM2002)
G4MaterialPropertiesTable* LXe_mt = new G4MaterialPropertiesTable();
  LXe_mt->AddProperty("RINDEX",         enegries, LXe_RIND,  3);
  LXe_mt->AddProperty("ABSLENGTH",      enegries, LXe_ABSL,  3);
  LXe->SetMaterialPropertiesTable(LXe_mt);

 // optical properties
 G4MaterialPropertiesTable* GXe_mt = new G4MaterialPropertiesTable();
  GXe_mt->AddProperty("RINDEX",         enegries, GXe_RIND,  3);
  GXe_mt->AddProperty("ABSLENGTH",      enegries, GXe_ABSL,  3);
  GXe->SetMaterialPropertiesTable(GXe_mt); 

G4double length =16.5*cm;
G4double lq_l=14.5*mm; 

world = new World(100*cm, 500*cm, 100*cm, vacuum);	
			//solid
//vedro
 G4Tubs*ironvedro = new G4Tubs("ironvedro",7.2*cm,7.5*cm,length,0.*deg,360.*deg);
//flanetz 
 G4Tubs*flan = new G4Tubs("flan",0.*cm,7.7*cm,0.5*cm,0.*deg,360.*deg);
//corp ring
 G4Tubs*ring = new G4Tubs("ring",7.5*cm,7.7*cm,0.5*cm,0.*deg,360.*deg);
//gas xenon volume
 G4Tubs*gas = new G4Tubs("gas",0.*cm,7.2*cm,10.*cm,0.*deg,360.*deg);
// using gas volume
 G4Tubs*gas0 = new G4Tubs("gas0",0.*cm,5.25*cm,2.5*mm,0.*deg,360.*deg);
//liquid xenon volume
 G4Tubs* liq = new G4Tubs("liq",0.*cm,7.2*cm,6.5*cm,0.*deg,360.*deg);
// using liquid volume
 G4Tubs* liq0 = new G4Tubs("liq0",0.*cm,5.25*cm,11.*mm,0.*deg,360.*deg);
//rings in liquid
G4Tubs*ring1g = new G4Tubs("ring1g",5.25*cm,6.6*cm,2.5*mm,0.*deg,360.*deg);
G4Tubs*ring1l = new G4Tubs("ring1l",5.25*cm,6.6*cm,1.5*mm,0.*deg,360.*deg); 
G4Tubs*ring2 = new G4Tubs("ring2",5.25*cm,6.6*cm,4.*mm,0.*deg,360.*deg);
G4Tubs*ring3 = new G4Tubs("ring3",4.45*cm,5.3*cm,0.5*mm,0.*deg,360.*deg);
//mirror disk
 G4Tubs *mirror = new G4Tubs("mirror",0.*cm,7.2*cm,0.5*mm,0.*deg,360.*deg);
//bolvanka for PMT 
 G4Tubs* uuu = new G4Tubs("uuu",0.*cm,7.2*cm,4.5*cm,0.*deg,360.*deg);
//Holes in bolvanka
G4Tubs* PTMtube = new G4Tubs("PTMtube",0.*cm,1.7*cm,4.6*cm,0.*deg,360.*deg);
//PTM
G4Tubs* PTM = new G4Tubs("PTM",0.*cm,1.5*cm,4.5*cm,0.*deg,360.*deg);
		//FOR NEUTRONS
//defense
G4Box *def=new G4Box("def",20.*cm,25*cm,2*length);
G4Box *collimator1=new G4Box("collimator1", 2.5*cm, 32*cm,1.2*cm);
G4Box *source=new G4Box("source",20*cm,20*cm,20*cm);

			// subtractions
  G4double PTMrad=3.5*cm;
  G4double sq32=sqrt(3)/2;
  G4ThreeVector Trans0(0.*PTMrad,0.*PTMrad,0.*cm);
  G4ThreeVector Trans1(1.*PTMrad,0.*PTMrad,0.*cm);
  G4ThreeVector Trans2(0.5*PTMrad,sq32*PTMrad,0.*cm);
  G4ThreeVector Trans3(-0.5*PTMrad,sq32*PTMrad,0.*cm);
  G4ThreeVector Trans4(-1.*PTMrad,0.*PTMrad,0.*cm);
  G4ThreeVector Trans5(-0.5*PTMrad,-sq32*PTMrad,0.*cm);
  G4ThreeVector Trans6(0.5*PTMrad,-sq32*PTMrad,0.*cm);
 G4RotationMatrix NoRot;
  G4Transform3D transform(NoRot,Trans0);
      //for PTM
  G4SubtractionSolid *disp0=new G4SubtractionSolid("disp0",uuu,PTMtube,G4Transform3D (NoRot,Trans0));
  G4SubtractionSolid *disp1=new G4SubtractionSolid("disp1",disp0,PTMtube,G4Transform3D (NoRot,Trans1));
  G4SubtractionSolid *disp2=new G4SubtractionSolid("disp2",disp1,PTMtube,G4Transform3D (NoRot,Trans2));
  G4SubtractionSolid *disp3=new G4SubtractionSolid("disp3",disp2,PTMtube,G4Transform3D (NoRot,Trans3));
  G4SubtractionSolid *disp4=new G4SubtractionSolid("disp4",disp3,PTMtube,G4Transform3D (NoRot,Trans4));
  G4SubtractionSolid *disp5=new G4SubtractionSolid("disp5",disp4,PTMtube,G4Transform3D (NoRot,Trans5));
  G4SubtractionSolid *disp6=new G4SubtractionSolid("disp6",disp5,PTMtube,G4Transform3D (NoRot,Trans6));
 //for defense
 G4SubtractionSolid *defense=new G4SubtractionSolid("defense", def, collimator1, G4Transform3D (NoRot, G4ThreeVector(0,0,-5.*cm)));

 			//logical volumes
//vedro
 vedro_log= new G4LogicalVolume(ironvedro, steel,"vedro_log");
//G4LogicalVolume* gh=new G4LogicalVolume(hf,  steel ,"flan_log");
//flanetz 
 flan_log= new G4LogicalVolume(flan,  steel ,"flan_log");
//ring
 ring_log= new G4LogicalVolume(ring, steel,"ring_log");
//gas xenon volume
 gas_log= new G4LogicalVolume(gas, GXe,"gas_log");
// using gas volume
 gas0_log= new G4LogicalVolume(gas0, GXe,"gas0_log");
//liquid xenon volume
 liq_log= new G4LogicalVolume(liq,LXe,"liq_log");
// using liquid volume
 liq0_log= new G4LogicalVolume(liq0,LXe,"liq0_log");
//rings in liquid
ring1g_log= new G4LogicalVolume(ring1g, steel,"ring1g_log");
ring1l_log= new G4LogicalVolume(ring1l, steel,"ring1l_log");
ring2_log= new G4LogicalVolume(ring2, steel,"ring2_log");
ring3_log= new G4LogicalVolume(ring3, steel,"ring2_log");
//mirror disk
 mirror_log= new G4LogicalVolume(mirror,  steel,"krish_log"); 
//bolvanka
 disp_log= new G4LogicalVolume(disp6, steel,"disp_log");
//PTM
 PTM_log= new G4LogicalVolume(PTM, glass,"PTM_log");
//defense 
 defense_log= new G4LogicalVolume(defense,  parafin,"defense__log");
G4LogicalVolume *source_log= new G4LogicalVolume(source, steel,"source_log");
G4double stlim=1.* mm;
//liq0_log->SetUserLimits(new G4UserLimits(stlim));
liq0_log->SetUserLimits(new G4UserLimits(stlim));
PTM_log->SetUserLimits(new G4UserLimits(stlim));
		//physical volumes
G4LogicalVolume *world_log=world->getLogic();
//vedro
vedro_phys = new G4PVPlacement(0, G4ThreeVector(0,0*cm,0) , vedro_log, "vedro_phys",world_log , false, 0);
//flanetz top and bottom
 flan1_phys = new G4PVPlacement(0,G4ThreeVector(0,0*cm, length+0.5*cm), flan_log, "flan1_phys", world_log, false, 0);
 flan2_phys = new G4PVPlacement(0,G4ThreeVector(0,0*cm,-length-0.5*cm), flan_log, "flan2_phys", world_log, false, 0);
 //ring top and bottom
 ringa_phys = new G4PVPlacement(0,G4ThreeVector(0,0*cm, length-0.5*cm) , ring_log, "ringa_phys", world_log, false, 0);
 ringb_phys = new G4PVPlacement(0,G4ThreeVector(0,0*cm,-length+0.5*cm), ring_log, "ringb_phys", world_log, false, 0);
//gas xenon volume
 gas_phys = new G4PVPlacement(0, G4ThreeVector(0,0*cm,length-10.*cm) ,gas_log, "gas_phys", world_log, false, 0);
gas0_phys = new G4PVPlacement(0, G4ThreeVector(0,0,2.5*mm-10.*cm) ,gas0_log, "gas_phys", gas_log, false, 0);
//mirror disk
 mirror_phys = new G4PVPlacement(0, G4ThreeVector(0,0,-9.45*cm), mirror_log, "mirror_phys", gas_log, false, 0);
 //liquid xenon volume
 liq_phys = new G4PVPlacement(0,G4ThreeVector(0,0*cm,-length+6.5*cm), liq_log,"liq_phys", world_log, false, 0);
 liq0_phys = new G4PVPlacement(0,G4ThreeVector(0,0,6.5*cm-1.1*cm), liq0_log,"liq0_phys", liq_log, false, 0);
 //bolvanka
G4ThreeVector ztrans=G4ThreeVector(0,0,6.5*cm-3.*cm-4.5*cm);
disp_phys = new G4PVPlacement(0, ztrans, disp_log, "disp_phys", liq_log, false, 0);
//PTMs
 PTM0_phys = new G4PVPlacement(0, Trans0+ztrans, PTM_log,"PTM0_phys", liq_log, false, 1);
 PTM1_phys = new G4PVPlacement(0, Trans1+ztrans, PTM_log,"PTM1_phys", liq_log, false, 2);
 PTM2_phys = new G4PVPlacement(0, Trans2+ztrans, PTM_log,"PTM2_phys", liq_log, false, 3);
 PTM3_phys = new G4PVPlacement(0, Trans3+ztrans, PTM_log,"PTM3_phys", liq_log, false, 4);
 PTM4_phys = new G4PVPlacement(0, Trans4+ztrans, PTM_log,"PTM4_phys", liq_log, false, 5);
 PTM5_phys = new G4PVPlacement(0, Trans5+ztrans, PTM_log,"PTM5_phys", liq_log, false, 6);
 PTM6_phys = new G4PVPlacement(0, Trans6+ztrans, PTM_log,"PTM6_phys", liq_log, false, 7);
//rings in liquid
ring1g_phys = new G4PVPlacement(0, G4ThreeVector(0,0,-10*cm+2.5*mm), ring1g_log, "ring1g_phys", gas_log, false, 0);
ring1l_phys = new G4PVPlacement(0, G4ThreeVector(0,0,6.5*cm-1.5*mm), ring1l_log, "ring1l_phys", liq_log, false, 0);
ring2a_phys = new G4PVPlacement(0, G4ThreeVector(0,0,6.5*cm-10.*mm), ring2_log,  "ring2a_phys", liq_log, false, 0);
ring2b_phys = new G4PVPlacement(0, G4ThreeVector(0,0,6.5*cm-21.*mm), ring2_log,  "ring2b_phys", liq_log, false, 0);
//ring1l__phys = new G4PVPlacement(0, G4ThreeVector(0,0,6.5*cm-22.*mm-1.5*mm), ring1l_log, "ring1l__phys", liq_log, false, 0);
ring3_phys = new G4PVPlacement(0, G4ThreeVector(0,0,6.5*cm-2*lq_l-0.5*mm), ring3_log, "ring3_phys", liq_log, false, 0); 
//defense
defense_phys = new G4PVPlacement(0, G4ThreeVector(0,00*cm-32.7*cm,0), defense_log,"defense_phys",world_log , false, 0);
//G4PVPlacement *source_p=new G4PVPlacement(0, G4ThreeVector(0,-120*cm,0), source_log,"source_p",world_log , false, 0);
 //G4cout<< "yh88hrrf    "<< gas0_phys->GetMotherLogical()->GetName() <<G4endl;*/

// optical surface
  OpAnoGasSurface = new G4OpticalSurface("OpAnoGasSurface", 
      unified, polished , dielectric_metal, sigalpha);
OpAnoGasSurface->SetMaterialPropertiesTable(mir_surface);
  AnoGasSurface = new G4LogicalBorderSurface("AnoGasSurface", 
      gas0_phys, mirror_phys, OpAnoGasSurface);

#define white   G4Colour(1.0, 1.0, 1.0)
#define grey    G4Colour(0.5, 0.5, 0.5)
#define lgrey   G4Colour(.75, .75, .75)
#define red     G4Colour(1.0, 0.0, 0.0)
#define blue    G4Colour(0.0, 0.0, 1.0)
#define cyan    G4Colour(0.0, 1.0, 1.0)
#define magenta G4Colour(1.0, 0.0, 1.0) 
#define yellow  G4Colour(1.0, 1.0, 0.0)
#define lblue   G4Colour(0.0, 0.0, .75)
#define black   G4Colour(0.0, 0.0, 0.0)
#define green   G4Colour(0.0, 1.0, 0.0) 

G4VisAttributes* magenta_c = new G4VisAttributes(magenta);
  magenta_c->SetVisibility(true);
  magenta_c->SetForceSolid(true);

G4VisAttributes* grey_c = new G4VisAttributes(grey);
 grey_c->SetVisibility(false);
G4VisAttributes* lgrey_c = new G4VisAttributes(lgrey);
 lgrey_c->SetVisibility(false);
G4VisAttributes* green_c = new G4VisAttributes(green);
 green_c->SetVisibility(false);
G4VisAttributes* yellow_c = new G4VisAttributes(yellow);
 yellow_c->SetVisibility(false);
G4VisAttributes* blue_c = new G4VisAttributes(blue);
 blue_c->SetVisibility(true);
G4VisAttributes* white_c = new G4VisAttributes(white);
 white_c->SetVisibility(true);
G4VisAttributes* cyan_c = new G4VisAttributes(cyan);
 cyan_c->SetVisibility(true);
G4VisAttributes* red_c = new G4VisAttributes(red);
 red_c->SetVisibility(true);
vedro_log->SetVisAttributes(grey_c);
flan_log->SetVisAttributes(grey_c);
ring_log->SetVisAttributes(grey_c);
	gas_log->SetVisAttributes(lgrey_c);
	liq_log->SetVisAttributes(lgrey_c);
	gas0_log->SetVisAttributes(red_c);
	disp_log->SetVisAttributes(lgrey_c);
	liq0_log->SetVisAttributes(cyan_c);
ring1g_log->SetVisAttributes(green_c);
ring1l_log->SetVisAttributes(green_c);
ring2_log->SetVisAttributes(green_c);
ring3_log->SetVisAttributes(green_c);
	PTM_log->SetVisAttributes(magenta_c);
	mirror_log->SetVisAttributes(white_c);
	defense_log->SetVisAttributes(yellow_c);
//defense_log->SetVisAttributes (G4VisAttributes::Invisible);
//disp_log->SetVisAttributes (G4VisAttributes::Invisible);
//PTM_log->SetVisAttributes (G4VisAttributes::Invisible);*/

world->getLogic()->SetVisAttributes (G4VisAttributes::Invisible);
   
  return world->getPhysic();
}
