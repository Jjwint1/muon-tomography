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
    G4double z;
    G4double density;
    G4int ncomp;
    G4double fracMass;
    G4String symbol;
    G4Material* wood = new G4Material("wood", density=0.6*g/cm3, ncomp=4);
    G4Element* elH  = new G4Element("Hydrogen",symbol="H",z=1.,1.01*g/mole);
    G4Element* elO  = new G4Element("Oxygen",symbol="O",z=8.,16.00*g/mole);
    G4Element* elC  = new G4Element("Carbon",symbol="C",z=6., 12.00*g/mole);
    G4Element* elN  = new G4Element("Nitrogen",symbol="N",z=7.,14.00*g/mole);
    wood -> AddElement(elH, fracMass=6.0*perCent);
    wood -> AddElement(elO, fracMass=41*perCent);
    wood -> AddElement(elC, fracMass=50*perCent);
    wood -> AddElement(elN, fracMass=3*perCent);
    //check volume overlap
    G4bool checkOverlaps = true;
    
    //Geometries------------------------------------------------------------------------------
    
    //World volume
    G4VSolid* worldSolid = new G4Box("worldBox", 11.*m, 11.*m, 11.*m);
    G4LogicalVolume* worldLogical = new G4LogicalVolume(worldSolid, air, "worldLogical");
    G4VPhysicalVolume* worldPhysical = new G4PVPlacement(0, G4ThreeVector(), worldLogical, "worldPhysical", 0, false, 0, checkOverlaps);
    
    //woodCube
    G4VSolid* woodCubeSolid = new G4Box("woodCubeBox", 0.5*m, 0.5*m, 5*m);
    G4LogicalVolume* woodCubeLogical = new G4LogicalVolume(woodCubeSolid, wood, "woodCubeLogical");
    new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), woodCubeLogical, "woodCubePhysical", worldLogical, false, 0, checkOverlaps);
    
    //detectingSheets
    G4VSolid* detectingSheetSolid = new G4Box("detectingSheetBox", 0.5*m, 0.5*m, 0.00001*cm);
    
    for(G4int i = 0; i < 101; i++) {
        mDetectingSheets.push_back(new G4LogicalVolume(detectingSheetSolid, wood, "detectingSheetLogical"));
        G4double z = (-500.*cm + i * 10.*cm);
        new G4PVPlacement(0, G4ThreeVector(0., 0., z), mDetectingSheets[i], "detectingSheetPhysical", woodCubeLogical, false, 0, false);
    }
    
    
    

    
    G4VisAttributes* visAttributes = new G4VisAttributes(G4Colour(1.0,1.0,1.0));
    visAttributes->SetVisibility(false);
    worldLogical->SetVisAttributes(visAttributes);
    mVisAttributes.push_back(visAttributes);
    
    visAttributes = new G4VisAttributes(G4Colour(0.0, 0.0, 0.9));
    mDetectingSheets[0]->SetVisAttributes(visAttributes);
    mVisAttributes.push_back(visAttributes);
    
    visAttributes = new G4VisAttributes(G4Colour(0.0, 0.0, 0.9));
    mDetectingSheets[100]->SetVisAttributes(visAttributes);
    mVisAttributes.push_back(visAttributes);

    return worldPhysical;
}

void DetectorConstruction::ConstructSDandField() {

    G4String filterName, particleName;
    
    G4SDParticleFilter* muonFilter = new G4SDParticleFilter(filterName="muonFilter", particleName="mu+");
    
    G4SDManager* SDman = G4SDManager::GetSDMpointer();
    G4String SDname;
    
    for(G4int i = 0; i < 101; i++) {
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
}

