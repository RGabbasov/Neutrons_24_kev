// ************************************************************************
//
// ZepIII-V05 (ZepIIIElectroLuminescenceMessenger)
//
// Henrique Araujo, Imperial College London
// 
// ************************************************************************
//
// HISTORY
// 27/02/2005: migrated from ZepIII-V03
// 18/01/2006: added interface for Fano factor


#include "ZepIIIElectroLuminescenceMessenger.hh"

#include "ZepIIIElectroLuminescence.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

ZepIIIElectroLuminescenceMessenger::ZepIIIElectroLuminescenceMessenger
(ZepIIIElectroLuminescence* elum) : theSecondary(elum) {

  elumDir = new G4UIdirectory("/physics/electroluminescence/");
  elumDir->SetGuidance("Set properties for electroluminescence (secondary)");

  verboseCmd = new 
    G4UIcmdWithAnInteger("/physics/electroluminescence/verbose",this);
  verboseCmd->SetGuidance("Set verbose level for electroluminescence process");
  verboseCmd->SetParameterName("verbose",false);
  verboseCmd->SetRange("verbose>=0");
  verboseCmd->AvailableForStates(G4State_Idle);

  // free-carrier production

  secEleYldCmd= new G4UIcmdWith3Vector
    ("/physics/electroluminescence/electronYieldEquation",this);
  secEleYldCmd->SetGuidance("Equation for the probability P of extracting a");
  secEleYldCmd->SetGuidance("free carrier (e-) from an electron track, as a"); 
  secEleYldCmd->SetGuidance("function of field E and particle energy Ee.");
  secEleYldCmd->SetGuidance("(Number of carriers is N = Edep/We * P)");
  secEleYldCmd->SetGuidance("P(E,Ee) = 1/( a/E/Ee + b/E + c)");
  secEleYldCmd->SetGuidance("DEFAULT: a=30.07 b=0.40 c=1.0");
  secEleYldCmd->SetParameterName("a","b","c",false);
  secEleYldCmd->AvailableForStates(G4State_Idle);

  secEleWCmd= new G4UIcmdWithADoubleAndUnit
    ("/physics/electroluminescence/electronWvalue",this);
  secEleWCmd->SetGuidance("Set W-value for electron tracks in LXe.");
  secEleWCmd->SetGuidance("DEFAULT= 15.6 eV");
  secEleWCmd->SetParameterName("We",false);
  secEleWCmd->SetUnitCategory("Energy");
  secEleWCmd->SetRange("We>0.");
  secEleWCmd->AvailableForStates(G4State_Idle);

  secAlphaYldCmd= new G4UIcmdWith3Vector
    ("/physics/electroluminescence/alphaYieldEquation",this);
  secAlphaYldCmd->SetGuidance("Equation for the probability P of extracting");
  secAlphaYldCmd->SetGuidance("a free carrier from an alpha track, as a"); 
  secAlphaYldCmd->SetGuidance("function of field E and particle energy Ea.");
  secAlphaYldCmd->SetGuidance("(Number of carriers is N = Edep/Wa * P)");
  secAlphaYldCmd->SetGuidance("P(E,En) = 1/( a/E/Ea + b/E + c)");
  secAlphaYldCmd->SetGuidance("DEFAULT: a=0 b=100 c=0");
  secAlphaYldCmd->SetParameterName("a","b","c",false);
  secAlphaYldCmd->AvailableForStates(G4State_Idle);

  secAlphaWCmd= new G4UIcmdWithADoubleAndUnit
    ("/physics/electroluminescence/alphaWvalue",this);
  secAlphaWCmd->SetGuidance("Set W-value for alphas in LXe.");
  secAlphaWCmd->SetGuidance("DEFAULT= 71 eV");
  secAlphaWCmd->SetParameterName("Wa",false);
  secAlphaWCmd->SetUnitCategory("Energy");
  secAlphaWCmd->SetRange("Wn>0.");
  secAlphaWCmd->AvailableForStates(G4State_Idle);

  secNucYldCmd= new G4UIcmdWith3Vector
    ("/physics/electroluminescence/nuclearYieldEquation",this);
  secNucYldCmd->SetGuidance("Equation for the probability P of extracting a");
  secNucYldCmd->SetGuidance("free carrier from a nuclear recoil track, as a"); 
  secNucYldCmd->SetGuidance("function of field E and nucleus energy En.");
  secNucYldCmd->SetGuidance("(Number of carriers is N = Edep/Wn * P)");
  secNucYldCmd->SetGuidance("P(E,En) = 1/( a/E/En + b/E + c)");
  secNucYldCmd->SetGuidance("DEFAULT: a=0 b=100 c=0");
  secNucYldCmd->SetParameterName("a","b","c",false);
  secNucYldCmd->AvailableForStates(G4State_Idle);

  // drifting in liquid

  secLiqDriCmd= 
    new G4UIcmdWithADouble("/physics/electroluminescence/liqDriftSpeed",this);
  secLiqDriCmd->SetGuidance("Set saturation drift speed in LXe, in mm/us");
  secLiqDriCmd->SetGuidance("DEFAULT= 2.95");
  secLiqDriCmd->SetParameterName("elumLiqDriftSpeed",false);
  secLiqDriCmd->SetRange("elumLiqDriftSpeed>0.");
  secLiqDriCmd->AvailableForStates(G4State_Idle);

  // extraction to gas

  secExtCmd= new G4UIcmdWith3Vector
    ("/physics/electroluminescence/electronExtractionEquation",this);
  secExtCmd->SetGuidance("Equation for field dependence of the fraction");
  secExtCmd->SetGuidance("of electrons extracted from LXe to GXe.");
  secExtCmd->SetGuidance("Nel(E) = 1-a*exp(-b*E^c)");
  secExtCmd->SetGuidance("DEFAULT: a=1.1 b=0.017 c=3.55");
  secExtCmd->SetParameterName("a","b","c",false);
  secExtCmd->AvailableForStates(G4State_Idle);

  secExtSclCmd= new 
    G4UIcmdWithADouble("/physics/electroluminescence/extractionScale",this);
  secExtSclCmd->SetGuidance("Scale field-dependent electron extraction.");
  secExtSclCmd->SetGuidance("DEFAULT= 1.0");
  secExtSclCmd->SetParameterName("extractionScale",false);
  secExtSclCmd->SetRange("extractionScale>=0. && extractionScale<=1.0");
  secExtSclCmd->AvailableForStates(G4State_Idle);

  // drifting in gas

  secPcoldCmd= new G4UIcmdWithADoubleAndUnit
    ("/physics/electroluminescence/coldGasPressure",this);
  secPcoldCmd->SetGuidance("Set gas pressure at -100C."); 
  secPcoldCmd->SetGuidance("DEFAULT= 2.5 bar");
  secPcoldCmd->SetParameterName("gasPcold",false);
  secPcoldCmd->SetUnitCategory("Pressure");
  secPcoldCmd->SetRange("gasPcold>0.");
  secPcoldCmd->AvailableForStates(G4State_Idle);

  secGasDriCmd= new G4UIcmdWithADouble
    ("/physics/electroluminescence/reducedGasDriftSpeed",this);
  secGasDriCmd->SetGuidance("Set gas drift speed per unit reduced electric");
  secGasDriCmd->SetGuidance("field in mm/us/(V/cm/torr).");
  secGasDriCmd->SetGuidance("DEFAULT= 1.5");
  secGasDriCmd->SetParameterName("elumGasDrift",false);
  secGasDriCmd->SetRange("elumGasDrift>0.");
  secGasDriCmd->AvailableForStates(G4State_Idle);

  // photon production

  secYldCmd= new 
    G4UIcmdWith3Vector("/physics/electroluminescence/lightYieldEquation",this);
  secYldCmd->SetGuidance("Equation for field dependence of light yield (gas)");
  secYldCmd->SetGuidance("Y(E) [photons/el/cm] = a*E[kV/cm] - b*Peq");
  secYldCmd->SetGuidance("(Peq = pressure for same gas density at 273 K,");
  secYldCmd->SetGuidance("calculated from 'coldGasPressure' command.");
  secYldCmd->SetGuidance("DEFAULT: a=137 b=125 c=0");
  secYldCmd->SetParameterName("a","b","c",false);
  secYldCmd->AvailableForStates(G4State_Idle);

  secYldSclCmd= new 
    G4UIcmdWithADouble("/physics/electroluminescence/yieldScale",this);
  secYldSclCmd->SetGuidance("Scale number of secondary photons produced.");
  secYldSclCmd->SetGuidance("Nph = yieldScale * ( <Nph> + resolutionScale*G4GaussRand(0,sqrt(<Nph>)) )"); 
  secYldSclCmd->SetGuidance("DEFAULT= 1.0");
  secYldSclCmd->SetParameterName("yieldScale",false);
  secYldSclCmd->SetRange("yieldScale>=0.");
  secYldSclCmd->AvailableForStates(G4State_Idle);

  secFanCmd= new G4UIcmdWithADouble
    ("/physics/electroluminescence/FanoFactor",this);
  secFanCmd->SetGuidance("Set Fano Factor F for ionisation.");
  secFanCmd->SetGuidance("sigma_Ne = sqrt(F*<Ne>)"); 
  secFanCmd->SetGuidance("DEFAULT= 1.");
  secFanCmd->SetParameterName("fano",false);
  secFanCmd->SetRange("fano>=0.");
  secFanCmd->AvailableForStates(G4State_Idle);

  secResCmd= new G4UIcmdWithADouble
    ("/physics/electroluminescence/resolutionScale",this);
  secResCmd->SetGuidance("Set resolution scale R for electroluminescence.");
  secResCmd->SetGuidance("sigma_Nph/e = R*sqrt(<Nph>)"); 
  secResCmd->SetGuidance("DEFAULT= 1.");
  secResCmd->SetParameterName("elumResScale",false);
  secResCmd->SetRange("elumResScale>=0.");
  secResCmd->AvailableForStates(G4State_Idle);

  secTauCmd= new G4UIcmdWithADoubleAndUnit
    ("/physics/electroluminescence/decayTime",this);
  secTauCmd->SetGuidance("Set decay time for electroluminescence light.");
  secTauCmd->SetGuidance("DEFAULT= 45 ns");
  secTauCmd->SetParameterName("elumTime",false);
  secTauCmd->SetRange("elumTime>=0.");
  secTauCmd->SetUnitCategory("Time");
  secTauCmd->AvailableForStates(G4State_Idle);

  // no optics

  secNoOCmd= 
    new G4UIcmdWithABool("/physics/electroluminescence/noOptics",this);
  secNoOCmd->SetGuidance("Switch electroluminescence tracking on/off ");
  secNoOCmd->SetGuidance("DEFAULT= false");
  secNoOCmd->SetParameterName("elumNoOptics",false);
  secNoOCmd->AvailableForStates(G4State_Idle);

  // voltage

  secVoltsCmd= new G4UIcmdWithADoubleAndUnit
    ("/physics/electroluminescence/setVoltage",this);
  secVoltsCmd->SetGuidance("Set total voltage V between electrodes.");
  secVoltsCmd->SetGuidance("This value will scale the reduced field map."); 
  secVoltsCmd->SetGuidance("In the absence of a map, the field is constant:"); 
  secVoltsCmd->SetGuidance("Field in liq: = V/(2.0*Lgas+Lliq)"); 
  secVoltsCmd->SetGuidance("Field in gas: = V/(Lgas+Lliq/2.0)"); 
  secVoltsCmd->SetGuidance("DEFAULT= 40. kV");
  secVoltsCmd->SetParameterName("voltage",false);
  secVoltsCmd->SetDefaultUnit("kV");
  secVoltsCmd->SetUnitCategory("Electric potential");
  secVoltsCmd->SetRange("voltage>=0.");
  secVoltsCmd->AvailableForStates(G4State_Idle);


  // field maps

  mapSetCmd= 
    new G4UIcmdWithAString("/physics/electroluminescence/loadMap",this);
  mapSetCmd->SetGuidance("Load/replace E field map from file");
  mapSetCmd->SetGuidance("DEFAULT= maps/zIII2d_element.txt");
  mapSetCmd->AvailableForStates(G4State_Idle);

  mapDelCmd= 
    new G4UIcmdWithoutParameter("/physics/electroluminescence/deleteMap",this);
  mapSetCmd->SetGuidance("Delete E field map from memory");
  mapSetCmd->AvailableForStates(G4State_Idle);

  mapTouCmd= 
   new G4UIcmdWith3VectorAndUnit("/physics/electroluminescence/touchMap",this);
  mapTouCmd->SetGuidance("Get E field and drift speed at point x y z");
  mapTouCmd->SetParameterName("X","Y","Z",false);
  mapTouCmd->SetDefaultUnit("mm");
  mapTouCmd->SetUnitCategory("Length");
  mapTouCmd->AvailableForStates(G4State_Idle);

  mapPltCmd= 
    new G4UIcmdWithoutParameter("/physics/electroluminescence/plotMap",this);
  mapPltCmd->SetGuidance("Plot E field lines");
  mapPltCmd->AvailableForStates(G4State_Idle);

  mapStpCmd= 
    new G4UIcmdWithADoubleAndUnit("/physics/electroluminescence/stepMap",this);
  mapStpCmd->SetGuidance("Set step size for tracking in E field");
  mapStpCmd->SetGuidance("DEFAULT= 0.5 mm");
  mapStpCmd->SetUnitCategory("Length");
  mapStpCmd->SetDefaultUnit("mm");
  mapStpCmd->SetParameterName("elumStepSize",false);
  mapStpCmd->SetRange("elumStepSize>0.");
  mapStpCmd->AvailableForStates(G4State_Idle);

  // reset gas geometry

  gasDimCmd= new 
    G4UIcmdWithoutParameter("/physics/electroluminescence/resetGasGap",this);
  gasDimCmd->SetGuidance("Reset dimensions after geometry change");
  gasDimCmd->AvailableForStates(G4State_Idle);


}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

ZepIIIElectroLuminescenceMessenger::~ZepIIIElectroLuminescenceMessenger() {

  delete elumDir;
  delete verboseCmd;
  delete secEleYldCmd;
  delete secEleWCmd;
  delete secAlphaYldCmd;
  delete secAlphaWCmd;
  delete secNucYldCmd;
  delete secYldCmd;
  delete secYldSclCmd;
  delete secFanCmd;
  delete secResCmd;
  delete secTauCmd;
  delete secGasDriCmd;
  delete secPcoldCmd;
  delete secLiqDriCmd;
  delete secExtCmd;
  delete secExtSclCmd;
  delete secVoltsCmd;
  delete secNoOCmd;
  delete mapSetCmd;
  delete mapDelCmd;
  delete mapTouCmd;
  delete mapPltCmd;
  delete mapStpCmd;
  delete gasDimCmd;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void ZepIIIElectroLuminescenceMessenger::SetNewValue
   (G4UIcommand* command, G4String newValue) { 

  if(command == verboseCmd) 
    {theSecondary->SetVerboseLevel(verboseCmd->GetNewIntValue(newValue));}

  else if(command == secEleYldCmd)
    {theSecondary->SetElecYieldEquation
       (secEleYldCmd->GetNew3VectorValue(newValue));}
  else if(command == secEleWCmd)
    {theSecondary->SetWeValue(secEleWCmd->GetNewDoubleValue(newValue));}
  else if(command == secAlphaYldCmd)
    {theSecondary->SetAlphaYieldEquation
       (secAlphaYldCmd->GetNew3VectorValue(newValue));}
  else if(command == secAlphaWCmd)
    {theSecondary->SetWaValue(secAlphaWCmd->GetNewDoubleValue(newValue));}
  else if(command == secNucYldCmd)
    {theSecondary->SetNucYieldEquation
       (secNucYldCmd->GetNew3VectorValue(newValue));}
  else if(command == secYldCmd)
    {theSecondary->SetYieldEquation(secYldCmd->GetNew3VectorValue(newValue));}
  else if(command == secYldSclCmd)
    {theSecondary->SetYieldScale(secYldSclCmd->GetNewDoubleValue(newValue));}
  else if(command == secFanCmd)
    {theSecondary->SetFanoFactor(secFanCmd->GetNewDoubleValue(newValue));}
  else if(command == secResCmd)
    {theSecondary->SetResolutionScale(secResCmd->GetNewDoubleValue(newValue));}
  else if(command == secTauCmd)
    {theSecondary->SetDecayTime(secTauCmd->GetNewDoubleValue(newValue));}
  else if(command == secLiqDriCmd)
    {theSecondary->SetLiqDriftSpeed
       (secLiqDriCmd->GetNewDoubleValue(newValue));}
  else if(command == secPcoldCmd)
    {theSecondary->SetColdGasPressure
       (secPcoldCmd->GetNewDoubleValue(newValue));}
  else if(command == secGasDriCmd)
    {theSecondary->SetGasDriftSpeed
       (secGasDriCmd->GetNewDoubleValue(newValue));}
  else if(command == secExtCmd)
    {theSecondary->SetExtractionEquation
       (secExtCmd->GetNew3VectorValue(newValue));}
  else if(command == secExtSclCmd)
    {theSecondary->SetExtractionScale
       (secExtSclCmd->GetNewDoubleValue(newValue));}
  else if(command == secVoltsCmd)
    {theSecondary->SetVoltage(secVoltsCmd->GetNewDoubleValue(newValue));}

  else if(command == secNoOCmd) {
    const char* nv = (const char*)newValue;
    std::istringstream is(nv);
    G4int vl;
    is >> vl;
    theSecondary->SetNoOptics(vl!=0);
  }

  else if(command == mapSetCmd)  
    {theSecondary->SetMap(newValue);}
  else if(command == mapDelCmd)  
    {theSecondary->DeleteMap();}
  else if(command == mapTouCmd)  
    {theSecondary->TouchMap(mapTouCmd->GetNew3VectorValue(newValue));}
  else if(command == mapPltCmd)  
    {theSecondary->PlotMap();}
  else if(command == mapStpCmd)
    {theSecondary->SetStepSize(mapStpCmd->GetNewDoubleValue(newValue));}

  else if(command == gasDimCmd)  
    {theSecondary->FindGasGeometry();}

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
