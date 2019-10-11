//
//  DetectorConstruction.hh
//
//
//  Created by Joe Winterburn.
//

#ifndef DetectorConstruction_h
#define DetectorConstruction_h

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"

#include <vector>

class G4VPhysicalVolume;
class G4Material;
class G4VSensitiveDetector;
class G4VisAttributes;
class G4GenericMessenger;

class DetectorConstruction : public G4VUserDetectorConstruction {
public:
    DetectorConstruction();
    virtual ~DetectorConstruction();
    
    virtual G4VPhysicalVolume* Construct();
    virtual void ConstructSDandField();
    
    void ConstructMaterials();
    
    
private:

    G4GenericMessenger* mMessenger;
    
    std::vector<G4VisAttributes*> mVisAttributes;
    
    std::vector<G4LogicalVolume*> mDetectingSheets;
    
    std::vector<G4VSensitiveDetector*> mSensitiveDetectors;
};
#endif
