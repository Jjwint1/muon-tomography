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
    G4Material* rho_0_9 = new G4Material("rho_0.9", density=0.9*g/cm3, ncomp=10);
    G4Element* elH  = new G4Element("Hydrogen",symbol="H",z=1.,1.01*g/mole);
    G4Element* elC  = new G4Element("Carbon",symbol="C",z=6., 12.00*g/mole);
    G4Element* elO  = new G4Element("Oxygen",symbol="O",z=8.,16.00*g/mole);
    G4Element* elNa  = new G4Element("Sodium",symbol="Na",z=11.,14.00*g/mole);
    G4Element* elMg  = new G4Element("Magnesium",symbol="Mg",z=12.,24.00*g/mole);
    G4Element* elAl  = new G4Element("Aluminium",symbol="Al",z=13.,27.00*g/mole);
    G4Element* elSi  = new G4Element("Silicon",symbol="Si",z=14.,28.00*g/mole);
    G4Element* elK  = new G4Element("Potassium",symbol="K",z=19.,39.00*g/mole);
    G4Element* elCa  = new G4Element("Calcium",symbol="Ca",z=20.,40.00*g/mole);
    G4Element* elFe  = new G4Element("Iron",symbol="Fe",z=26.,56.00*g/mole);
    
    rho_0_9 -> AddElement(elH, fracMass=0.01*perCent);
    rho_0_9 -> AddElement(elC, fracMass=0.001*perCent);
    rho_0_9 -> AddElement(elO, fracMass=0.529107*perCent);
    rho_0_9 -> AddElement(elNa, fracMass=0.016*perCent);
    rho_0_9 -> AddElement(elMg, fracMass=0.002*perCent);
    rho_0_9 -> AddElement(elAl, fracMass=0.033872*perCent);
    rho_0_9 -> AddElement(elSi, fracMass=0.337021*perCent);
    rho_0_9 -> AddElement(elK, fracMass=0.013*perCent);
    rho_0_9 -> AddElement(elCa, fracMass=0.044*perCent);
    rho_0_9 -> AddElement(elFe, fracMass=0.014*perCent);
    
    //check volume overlap
    G4bool checkOverlaps = true;
    
    //Geometries------------------------------------------------------------------------------
    
    //World volume
    G4VSolid* worldSolid = new G4Box("worldBox", 11.*m, 11.*m, 11.*m);
    G4LogicalVolume* worldLogical = new G4LogicalVolume(worldSolid, air, "worldLogical");
    G4VPhysicalVolume* worldPhysical = new G4PVPlacement(0, G4ThreeVector(), worldLogical, "worldPhysical", 0, false, 0, checkOverlaps);
    
    //rho_0_9Cube
    G4VSolid* rho_0_9CubeSolid = new G4Box("rho_0_9CubeBox", 0.5*m, 0.5*m, 5*m);
    G4LogicalVolume* rho_0_9CubeLogical = new G4LogicalVolume(rho_0_9CubeSolid, rho_0_9, "rho_0_9CubeLogical");
    new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), rho_0_9CubeLogical, "rho_0_9CubePhysical", worldLogical, false, 0, checkOverlaps);
    
    //detectingSheets
    G4VSolid* detectingSheetSolid = new G4Box("detectingSheetBox", 0.5*m, 0.5*m, 0.00001*cm);
    
    for(G4int i = 0; i < 101; i++) {
        mDetectingSheets.push_back(new G4LogicalVolume(detectingSheetSolid, rho_0_9, "detectingSheetLogical"));
        G4double z = (-500.*cm + i * 10.*cm);
        new G4PVPlacement(0, G4ThreeVector(0., 0., z), mDetectingSheets[i], "detectingSheetPhysical", rho_0_9CubeLogical, false, 0, false);
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

