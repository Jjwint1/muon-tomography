//
//  SheetSD.cpp
//
//
//  Created by Joe Winterburn.
//

#include "SheetSD.hh"
#include "SheetHit.hh"

#include "G4HCofThisEvent.hh"
#include "G4TouchableHistory.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"

SheetSD::SheetSD(G4String name) : G4VSensitiveDetector(name), mSheetHitsCollection(0), mHCID(-1) {
    G4String HCname = "sheetColl";
    collectionName.insert(HCname);
}

SheetSD::~SheetSD() {}

void SheetSD::Initialize(G4HCofThisEvent* hce) {
    mSheetHitsCollection = new SheetHitsCollection(SensitiveDetectorName,collectionName[0]);
    if (mHCID<0) {
        
        mHCID = G4SDManager::GetSDMpointer()->GetCollectionID(mSheetHitsCollection);
    }
    hce->AddHitsCollection(mHCID,mSheetHitsCollection);
}

G4bool SheetSD::ProcessHits(G4Step* step, G4TouchableHistory*) {
    
    G4StepPoint* preStepPoint = step->GetPreStepPoint();
    
    G4TouchableHistory* touchable
    = (G4TouchableHistory*)(step->GetPreStepPoint()->GetTouchable());
    
    G4ThreeVector localPos = preStepPoint->GetPosition();
    G4ThreeVector momentumDir = preStepPoint->GetMomentumDirection();
    G4ThreeVector* normal = new G4ThreeVector(0., 0., 1.);
    G4double angle = acos(normal->dot(momentumDir));
    
    SheetHit* hit = new SheetHit();
    hit->SetLocalPos(localPos);
    hit->SetMomentum(momentumDir);
    hit->SetAngle(angle);
    
    mSheetHitsCollection->insert(hit);
    
    return true;
}

