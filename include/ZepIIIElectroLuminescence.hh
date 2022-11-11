// ************************************************************************
//
// ZepIII-V05 (ZepIIIElectroLuminescence)
//
// Henrique Araujo, Imperial College London
// 
// ************************************************************************

#ifndef ZepIIIElectroLuminescence_h
#define ZepIIIElectroLuminescence_h 1


#include "G4VRestDiscreteProcess.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4StepPoint.hh"
#include "G4ThreeVector.hh"
#include "globals.hh"

class ZepIIIElectroLuminescenceMessenger;
class ZepIIIFieldMap;
class G4Colour;

class ZepIIIElectroLuminescence : public G4VRestDiscreteProcess {

  public:
    ZepIIIElectroLuminescence(const 
			      G4String& processName = "ElectroLuminescence");
    ~ZepIIIElectroLuminescence();

  public:

    G4double GetMeanFreePath(const G4Track& aTrack,G4double,G4ForceCondition*);

    G4double GetMeanLifeTime(const G4Track& aTrack,G4ForceCondition*);

    G4VParticleChange* PostStepDoIt(const G4Track& aTrack,const G4Step& aStep);

    G4VParticleChange* AtRestDoIt (const G4Track& aTrack, const G4Step& aStep);

    void SetMap(const G4String&);

    void DeleteMap(void);

    void PlotMap(void);

    void TouchMap(G4ThreeVector);

    void FindGasGeometry(void);

  private:

    inline G4bool InsideLiquidField(const G4ThreeVector& p)
       { return (p.z()>-5.7*cm && p.z()<ZLiqGas && p.perp()<DetRadius) ; }

    inline G4bool InsideGasField(const G4ThreeVector& p) 
       { return (p.z()>=ZLiqGas && p.z()<(ZLiqGas+GasThickness) && p.perp()<DetRadius) ; }

    G4StepPoint* DriftInLiquid(G4StepPoint*);

    G4StepPoint* DriftInGas(G4StepPoint*);

    void PlotStep(const G4ThreeVector, const G4ThreeVector, G4Colour);
  

  // inlines 
  public:
    inline G4bool IsApplicable(const G4ParticleDefinition& aParticleType)
       { if(aParticleType.GetParticleName()=="opticalphoton") return false;
       else return true; };


    inline void SetElecYieldEquation(const G4ThreeVector val) 
       { eYield_a = val.x(); eYield_b = val.y(); eYield_c = val.z(); }

    inline void SetWeValue(const G4double val)         
       { WeValue = val; }

    inline void SetAlphaYieldEquation(const G4ThreeVector val) 
       { aYield_a = val.x(); aYield_b = val.y(); aYield_c = val.z(); }

    inline void SetWaValue(const G4double val)         
       { WaValue = val; }

    inline void SetNucYieldEquation(const G4ThreeVector val) 
       { nYield_a = val.x(); nYield_b = val.y(); nYield_c = val.z(); }

  //  inline void SetNucYieldEquation(const G4ThreeVector val) 
  //     { nYield_a = val.x(); nYield_b = val.y(); }

    inline void SetStepSize(const G4double val)        
       { stepSize = val; }

    inline void SetLiqDriftSpeed(const G4double val)   
       { liqDriftSpeed   = val*mm/microsecond; }

    inline void SetExtractionEquation(const G4ThreeVector val) 
       { ext_a = val.x(); ext_b = val.y(); ext_c = val.z(); }

    inline void SetExtractionScale(const G4double val) 
       { extractionScale = val; }

    inline void SetColdGasPressure(const G4double val)
       { gasPcold = val; }

    inline void SetGasDriftSpeed(const G4double val)
       { gasDriftSpeed = val*mm/microsecond/(volt/cm/(bar/750.)); }

    inline void SetYieldEquation(const G4ThreeVector val)
       { yield_a = val.x(); yield_b = val.y(); /* yield_p = val.z(); */ }

    inline void SetYieldScale(const G4double val)    
       { yieldScale = val; }

    inline void SetFanoFactor(const G4double val) 
       { FanoFactor = val; }

    inline void SetResolutionScale(const G4double val) 
       { resolutionScale = val; }

    inline void SetDecayTime(const G4double val)
       { decayTime  = val; }

    inline void SetVoltage(const G4double val)
       { voltage  = val/volt; 
         G4cout << "ZepIIIElectroLuminescence::SetVoltage";
         if(aFieldMap) G4cout << " -- Scaling existing field map." << G4endl;
         else G4cout << " -- No map in memory! Using constant field." <<G4endl;
       }

    inline void SetNoOptics(const G4bool val)
       { noOpticsFlag    = val; }


  private:

    // messenger
    ZepIIIElectroLuminescenceMessenger* elumMessenger;

    // gas phase parameters
    G4double ZLiqGas;
    G4double DetRadius;
    G4double GasThickness;

    // electroluminescence parameters
    G4double eYield_a;
    G4double eYield_b;
    G4double eYield_c;
    G4double WeValue;
    G4double aYield_a;
    G4double aYield_b;
    G4double aYield_c;
    G4double WaValue;
    G4double nYield_a;
    G4double nYield_b;
    G4double nYield_c;
    G4double WnValue;
    G4double stepSize;
    G4double liqDriftSpeed;
    G4double ext_a;
    G4double ext_b;
    G4double ext_c;
    G4double extractionScale;
    G4double gasPcold;
    G4double gasDriftSpeed;
    G4double yield_a;
    G4double yield_b;
    // G4double yield_p;
    G4double yieldScale;
    G4double FanoFactor;
    G4double resolutionScale;
    G4double decayTime;
    G4double voltage;
    G4double field_liq;
    G4double field_gas;	

    G4bool noOpticsFlag;

    G4double photonEnergy;

    G4double tolerance;

    ZepIIIFieldMap* aFieldMap;

};

#endif

