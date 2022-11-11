// ************************************************************************
//
// ZepIII-V03 (ZepIIIFieldMap)
//
// Henrique Araujo, Imperial College London
// 
// ************************************************************************
//

#include "ZepIIIFieldMap.hh"

#include <fstream>
#include <sstream>
#include <iomanip>
#include <cmath>


ZepIIIFieldMap::ZepIIIFieldMap() {

//  G4cout << "ZepIIIFieldMap::Created" << G4endl;
}


ZepIIIFieldMap::~ZepIIIFieldMap() {

  //G4cout << "ZepIIIFieldMap::Deleted" << G4endl;
}




// Read ANSYS E field map for fiducial region
// Map is calculated for 1V across electrodes
// *** file in V/mm ***
/*G4bool ZepIIIFieldMap::ReadMap(const G4String& filename) {

  // test filename and open
  std::ifstream fieldmap(filename, std::ios::in);
  if(!fieldmap.is_open()) {
    G4cout << "ZepIIIFieldMap::ReadMap"
	   << " -- Could not find " << filename << G4endl;
    return false;
  }

  // read data vectors; 2D map (R,Z)
  G4double x, z, Ex, Ez;
  while(fieldmap >> x >> z >> Ex >> Ez) {
    mesh.push_back(G4ThreeVector(x,0.,z));
    field.push_back(G4ThreeVector(Ex,0.,Ez));
  }
  
  // check vector sizes
  if(mesh.size()!=field.size() || field.size()==0) {
    G4cout << "ZepIIIFieldMap::ReadMap"
	   << " -- Error reading " << filename << G4endl;
    return false;
  }

  // done
  fieldmap.close();

  G4cout << "ZepIIIFieldMap::ReadMap -- " << field.size()
	 << " entries read from " << filename << G4endl;
  
  return true;
  
}*/

// ..oo00oo..oo00oo..oo00oo..oo00oo..oo00oo..oo00oo..oo00oo..oo00oo..oo00oo..
// 00oo..oo00oo..oo00oo..oo00oo..oo00oo..oo00oo..oo00oo..oo00oo..oo00oo..ooOO

G4ThreeVector ZepIIIFieldMap::GetField(const G4ThreeVector point) {

  G4ThreeVector newField=G4ThreeVector(0.,0.,-1.);

return newField;

}
