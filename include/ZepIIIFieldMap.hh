// ************************************************************************
//
// ZepIII-V03 (ZepIIIFieldMap)
//
// Henrique Araujo, Imperial College London
// 
// ************************************************************************

#ifndef ZepIIIFieldMap_h
#define ZepIIIFieldMap_h 1

#include "G4ios.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"
#include <vector>

class ZepIIIFieldMap {

  public:
    ZepIIIFieldMap();
    ~ZepIIIFieldMap();

    G4bool ReadMap(const G4String&);
    G4ThreeVector GetField(const G4ThreeVector);

  private:
  std::vector<G4ThreeVector> mesh;
  std::vector<G4ThreeVector> field;

};

#endif
