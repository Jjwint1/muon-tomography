//
//  DetectorConstruction.cpp
//
//
//  Created by Joe Winterburn.
//

#include "DetectorConstruction.hh"
#include "G4TransportationManager.hh"

#include "G4SDParticleFilter.hh"

#include "SheetSD.hh"

#include "G4Material.hh"
#include "G4Element.hh"
#include "G4MaterialTable.hh"
#include "G4NistManager.hh"

#include "G4VSolid.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4UserLimits.hh"

#include "G4SDManager.hh"
#include "G4VSensitiveDetector.hh"
#include "G4RunManager.hh"
#include "G4GenericMessenger.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4ios.hh"
#include "G4SystemOfUnits.hh"

#include <vector>
using namespace std;

DetectorConstruction::DetectorConstruction()
: G4VUserDetectorConstruction(), mMessenger(0), mVisAttributes(), mDetectingSheets(0), mSensitiveDetectors(0)

{
 //   DefineCommands();
}

DetectorConstruction::~DetectorConstruction() {
    delete mMessenger;
    
    for (G4int i = 0; i < G4int(mVisAttributes.size()); i++) {
        delete mVisAttributes[i];
    }
}

G4VPhysicalVolume* DetectorConstruction::Construct() {
    
    //Construct Materials
    ConstructMaterials();
    G4Material* air = G4Material::GetMaterial("G4_AIR");
    G4Material* concrete = G4Material::GetMaterial("G4_CONCRETE");
    
    //check volume overlap
    G4bool checkOverlaps = true;
    
    //Geometries------------------------------------------------------------------------------
    
    //World volume
    G4VSolid* worldSolid = new G4Box("worldBox", 11.*m, 11.*m, 11.*m);
    G4LogicalVolume* worldLogical = new G4LogicalVolume(worldSolid, air, "worldLogical");
    G4VPhysicalVolume* worldPhysical = new G4PVPlacement(0, G4ThreeVector(), worldLogical, "worldPhysical", 0, false, 0, checkOverlaps);
    
    //ConcreteCube
    G4VSolid* concreteCubeSolid = new G4Box("concreteCubeBox", 0.5*m, 0.5*m, 1*m);
    G4LogicalVolume* concreteCubeLogical = new G4LogicalVolume(concreteCubeSolid, concrete, "concreteCubeLogical");
    new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), concreteCubeLogical, "concreteCubePhysical", worldLogical, false, 0, checkOverlaps);
    
    //detectingSheets
    G4VSolid* detectingSheetSolid = new G4Box("detectingSheetBox", 0.5*m, 0.5*m, 0.00001*cm);
    
    for(G4int i = 0; i < 21; i++) {
        mDetectingSheets.push_back(new G4LogicalVolume(detectingSheetSolid, concrete, "detectingSheetLogical"));
        G4double z = (-100.*cm + i * 10.*cm);
        new G4PVPlacement(0, G4ThreeVector(0., 0., z), mDetectingSheets[i], "detectingSheetPhysical", concreteCubeLogical, false, 0, false);
    }
    
    
    

    
    G4VisAttributes* visAttributes = new G4VisAttributes(G4Colour(1.0,1.0,1.0));
    visAttributes->SetVisibility(false);
    worldLogical->SetVisAttributes(visAttributes);
    mVisAttributes.push_back(visAttributes);
    
 /*   visAttributes = new G4VisAttributes(G4Colour(0.0, 0.0, 0.9));
    mDetectingSheets[0]->SetVisAttributes(visAttributes);
    mVisAttributes.push_back(visAttributes);
    
    visAttributes = new G4VisAttributes(G4Colour(0.0, 0.0, 0.9));
    mDetectingSheets[100]->SetVisAttributes(visAttributes);
    mVisAttributes.push_back(visAttributes);*/

    return worldPhysical;
}

void DetectorConstruction::ConstructSDandField() {

    G4String filterName, particleName;
    
    G4SDParticleFilter* muonFilter = new G4SDParticleFilter(filterName="muonFilter", particleName="mu+");
    
    G4SDManager* SDman = G4SDManager::GetSDMpointer();
    G4String SDname;
    
    for(G4int i = 0; i < 21; i++) {
        string name = "sheetSD" + to_string(i);
        mSensitiveDetectors.push_back(new SheetSD(SDname=name));
        mSensitiveDetectors[i]->SetFilter(muonFilter);
        SDman->AddNewDetector(mSensitiveDetectors[i]);
        mDetectingSheets[i]->SetSensitiveDetector(mSensitiveDetectors[i]);
    }
    
}

void DetectorConstruction::ConstructMaterials() {
    
    G4NistManager* nistManager = G4NistManager::Instance();
    
    //air
    nistManager->FindOrBuildMaterial("G4_AIR");
    
    //concrete
    nistManager->FindOrBuildMaterial("G4_CONCRETE");
}

