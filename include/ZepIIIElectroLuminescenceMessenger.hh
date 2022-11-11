// ************************************************************************
//
// ZepIII-V05 (ZepIIIElectroLuminescenceMessenger)
//
// Henrique Araujo, Imperial College London
// 
// ************************************************************************

#ifndef ZepIIIElectroLuminescenceMessenger_h
#define ZepIIIElectroLuminescenceMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWith3Vector.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include <sstream>

class ZepIIIElectroLuminescence;


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

class ZepIIIElectroLuminescenceMessenger: public G4UImessenger {

  public:
    ZepIIIElectroLuminescenceMessenger(ZepIIIElectroLuminescence*);
    ~ZepIIIElectroLuminescenceMessenger();
    
    void SetNewValue(G4UIcommand*, G4String);
    
  private:
    ZepIIIElectroLuminescence* theSecondary;

    G4UIdirectory*             elumDir;

    G4UIcmdWithAnInteger*      verboseCmd;

    G4UIcmdWith3Vector*        secEleYldCmd;
    G4UIcmdWithADoubleAndUnit* secEleWCmd;
    G4UIcmdWith3Vector*        secAlphaYldCmd;
    G4UIcmdWithADoubleAndUnit* secAlphaWCmd;
    G4UIcmdWith3Vector*        secNucYldCmd;
    G4UIcmdWith3Vector*        secYldCmd;
    G4UIcmdWithADouble*        secYldSclCmd;
    G4UIcmdWithADouble*        secFanCmd; 
    G4UIcmdWithADouble*        secResCmd; 
    G4UIcmdWithADoubleAndUnit* secTauCmd;
    G4UIcmdWithADouble*        secLiqDriCmd;
    G4UIcmdWithADouble*        secGasDriCmd;
    G4UIcmdWithADoubleAndUnit* secPcoldCmd;
    G4UIcmdWith3Vector*        secExtCmd;
    G4UIcmdWithADouble*        secExtSclCmd;
    G4UIcmdWithADoubleAndUnit* secVoltsCmd;

    G4UIcmdWithABool*          secNoOCmd;

    G4UIcmdWithAString*        mapSetCmd;
    G4UIcmdWithoutParameter*   mapDelCmd;
    G4UIcmdWith3VectorAndUnit* mapTouCmd;
    G4UIcmdWithoutParameter*   mapPltCmd;
    G4UIcmdWithADoubleAndUnit* mapStpCmd;

    G4UIcmdWithoutParameter*   gasDimCmd;

};

#endif

