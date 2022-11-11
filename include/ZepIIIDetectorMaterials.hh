// ************************************************************************
//
// ZepIII-V05 (ZepIIIDetectorMaterials)
//
// Henrique Araujo, Imperial College London
// 
// ************************************************************************

// HISTORY
// 16/06/2006: decreased LXe density to 2.90 g/cm3
// 16/06/2006: increased GXe density to 13.5 mg/cm3
// 16/06/2006: removed photocathode reflection
// 16/06/2006: matched n for PMT window and removed absorption
// 16/06/2006: decreased grid absorption to 5% (wires, not mesh)
// 17/11/2006: added floor & wall materials
// CHAM TINKERING....(unofficial changes)...
// 08/03/2007: Adding veto materials

void DetectorConstruction::ConstructMaterials(void) {


  // elements
  G4Element* Xe = new G4Element("Xenon",    "Xe", 54., 131.29*g/mole);
  G4Element* Cu = new G4Element("Copper",   "Cu", 29., 63.55*g/mole);  
  G4Element* Si = new G4Element("Silicon",  "Si", 14., 28.09*g/mole);
  G4Element* O  = new G4Element("Oxygen",   "O",   8., 16.00*g/mole);
  G4Element* Fe = new G4Element("Iron",     "Fe", 26., 55.85*g/mole);
  G4Element* C  = new G4Element("Carbon",   "C",   6., 12.01*g/mole);
  G4Element* Co = new G4Element("Cobalt",   "Co", 27., 58.93*g/mole);
  G4Element* Pb = new G4Element("lead",     "Pb", 82., 207.2*g/mole);
  G4Element* H  = new G4Element("Hydrogen", "H",   1., 1.0079*g/mole);
  G4Element* Mn = new G4Element("Manganese","Mn", 25., 54.938*g/mole);
  G4Element* P  = new G4Element("Phosphoros","P", 15., 30.9738*g/mole);
  G4Element* S  = new G4Element("Sulfur",    "S", 16., 32.06*g/mole);
  G4Element* Gd = new G4Element("Gadolinium","Gd", 64., 157.25*g/mole);
  G4Element* N = new G4Element("Nitrogen", "N", 7., 14.0067*g/mole);
  G4Element* Cl = new G4Element("Chlorine", "Cl",17., 35.453*g/mole);
  G4Element* Na = new G4Element("Sodium", "Na", 11., 22.9897*g/mole);

  // steel (AISI 1005) wall
  steel = new G4Material("steel", 7.872*g/cm3, 5);
  steel->AddElement(C, 0.0006);
  steel->AddElement(Fe,0.995);
  steel->AddElement(Mn,0.0035);
  steel->AddElement(P, 0.0004);
  steel->AddElement(S, 0.0005);

  // stainless steel floor
  ssteel = new G4Material("ssteel", 7.7*g/cm3, 3);
  ssteel->AddElement(C, 0.04);
  ssteel->AddElement(Fe, 0.88);
  ssteel->AddElement(Co, 0.08);




   // optical properties
  G4double vscint_PP[2]   = {  2.5*eV,  3.5*eV };  //peak 425nm ~ 2.9eV
  G4double vscint_RIND[2] = {    1.58,    1.58 };
  G4double vscint_ABSL[2] = { 210.0*cm, 210.0*cm };
  vscint_mt = new G4MaterialPropertiesTable();
  vscint_mt->AddProperty("RINDEX",        vscint_PP, vscint_RIND,  2);
  vscint_mt->AddProperty("ABSLENGTH",     vscint_PP, vscint_ABSL,  2);
  vscint->SetMaterialPropertiesTable(vscint_mt);

 
// vacuum
  vacuum = new G4Material("Vacuum", 
    1., 1.*g/mole, 1.e-20*g/cm3, kStateGas, 0.1*kelvin, 1.e-20*bar);

  // liquid xenon
  LXe = new G4Material("LXe", 
    2.90*g/cm3, 1, kStateLiquid, 178*kelvin, 2.0*bar);
  LXe->AddElement(Xe, 1);
  // optical properties (V.Chepel et al IDM2002)
  G4double LXe_PP[2]   = {  6.5*eV,  7.5*eV };
  G4double LXe_RIND[2] = {    1.69,    1.69 };
  G4double LXe_ABSL[2] = { 36.4*cm, 36.4*cm };
  // Check angular dependence of Rayleigh scattering!
  // G4double LXe_RAYL[2] = { 999.*cm, 999.*cm };
  LXe_mt = new G4MaterialPropertiesTable();
  LXe_mt->AddProperty("RINDEX",        LXe_PP, LXe_RIND,  2);
  LXe_mt->AddProperty("ABSLENGTH",     LXe_PP, LXe_ABSL,  2);
  //  LXe_mt->AddProperty("RAYLEIGH",      LXe_PP, LXe_RAYL,  2);
  LXe->SetMaterialPropertiesTable(LXe_mt);


  // xenon gas
  GXe = new G4Material("GXe", 13.5*mg/cm3, 1, kStateGas, 178.*kelvin, 1.4*bar);
  GXe->AddElement(Xe, 1);
  // optical properties
  G4double GXe_PP[2]   = {  6.5*eV,  7.5*eV };
  G4double GXe_RIND[2] = {   1.004,   1.004 };
  G4double GXe_ABSL[2] = { 1000*cm, 1000*cm };
  G4MaterialPropertiesTable* GXe_mt = new G4MaterialPropertiesTable();
  GXe_mt->AddProperty("RINDEX",        GXe_PP, GXe_RIND,  2);
  GXe_mt->AddProperty("ABSLENGTH",     GXe_PP, GXe_ABSL,  2);
  GXe->SetMaterialPropertiesTable(GXe_mt);

  
  // GLISUR MODEL: 0(ground/diffuse) <=  polish  <= 1(polished/specular)
  // UNIFIED MODEL: 0.*deg(specular)  <= sigalpha <= ~45.*deg(~iso)
  sigalpha = 20.*deg;


  // copper ware
  copper = new G4Material("copper", 8.920*g/cm3, 1);
  copper->AddElement(Cu, 1);
  // optical properties: all copper surfaces except mirrors
  // CRC Handbook of Chemistry & Physics, p. 12-120
  // REDUCED TO MORE REALISTIC 15%
  // NOTE: copper_surface is attached to the surface not the material!
  G4double copper_PP[2]   = {  6.5*eV,  7.5*eV };
  G4double copper_REFL[2] = {    0.15,    0.15 };
  copper_surface = new G4MaterialPropertiesTable();
  copper_surface->AddProperty("REFLECTIVITY", copper_PP, copper_REFL, 2);


  // top mirror optical properties
  // NOTE: mirror_surface is attached to the surface not the material! 
  G4double mir_REFL[2] = {    0.15,    0.15 };
  mir_surface = new G4MaterialPropertiesTable();
  mir_surface->AddProperty("REFLECTIVITY", copper_PP, mir_REFL, 2);


  // pmt mirror optical properties
  // NOTE: mirror_surface is attached to the surface not the material!
  G4double pmt_mir_REFL[2] = {    0.15,    0.15 };
  pmt_mir_surface = new G4MaterialPropertiesTable();
  pmt_mir_surface->AddProperty("REFLECTIVITY", copper_PP, pmt_mir_REFL, 2);
  
  
  // copper/lxe mixture for voltage divider disks
  // 15 copper plates (2~mm) + 14 LXe gaps (0.5 mm)
  xencu = new G4Material("xencu", 8.9*g/cm3, 2);
  xencu->AddMaterial(LXe,    0.073);
  xencu->AddMaterial(copper, 0.927);

  // stainless steel
  //    G4Material* ssteel = new G4Material("ssteel", 7.7*g/cm3, 3);
  //    ssteel->AddElement(C, 0.04);
  //    ssteel->AddElement(Fe, 0.88);
  //    ssteel->AddElement(Co, 0.08);

  // glassware: window is Spectrosil B;
  glass = new G4Material("glass", 2.21*g/cm3, 2);
  glass->AddElement(Si, 1);
  glass->AddElement(O , 2);
  // optical properties (don't remember where from)
  G4double glass_PP[3]   = {  5.0*eV,   6.69*eV,   7.50*eV };
  G4double glass_RIND[3] = {    1.69,      1.69,      1.69 };
  //  G4double glass_RIND[3] = {    1.51,      1.57,      1.61 };
  G4double glass_ABSL[3] = { 999.0*cm, 999.0*cm, 999.0*cm };
  //  G4double glass_ABSL[3] = {  5.0*cm,    5.0*cm,    5.0*cm };
  G4MaterialPropertiesTable* glass_mt = new G4MaterialPropertiesTable();
  glass_mt->AddProperty("RINDEX",    glass_PP, glass_RIND, 3);
  glass_mt->AddProperty("ABSLENGTH", glass_PP, glass_ABSL, 3);
  glass->SetMaterialPropertiesTable(glass_mt);


}
