//
//  EventAction.cpp
//
//
//  Created by Joe Winterburn.
//



#include "EventAction.hh"
#include "SheetHit.hh"
#include "Analysis.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4EventManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4VHitsCollection.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"

#include "DetectorConstruction.hh"

#include <iostream>
#include <fstream>

using namespace std;

EventAction::EventAction() : G4UserEventAction(), mHCIDs(0) {}

EventAction::~EventAction() {}

void EventAction::BeginOfEventAction(const G4Event*) {
    
    
    if(mHCIDs.size() == 0) {
        
        G4SDManager* sdManager = G4SDManager::GetSDMpointer();
        
        for(G4int i = 0; i < 21; i++) {
            string collectionName = "sheetSD" + to_string(i) + "/sheetColl";
            mHCIDs.push_back(sdManager->GetCollectionID(collectionName));
        }
    }
    
}

void EventAction::EndOfEventAction(const G4Event* event) {
    
    G4HCofThisEvent* hce = event->GetHCofThisEvent();
    
    for(G4int i = 0; i < 21; i++) {
        
        SheetHitsCollection* sHC = static_cast<SheetHitsCollection*>(hce->GetHC(mHCIDs[i]));
        
        if (sHC->entries() != 0) {
            SheetHit* hit = (*sHC)[0];
            G4ThreeVector localPos = hit->GetLocalPos();
            G4ThreeVector momentum = hit->GetMomentum();
            G4double angle = hit->GetAngle();
            ofstream myFileX;
            string filename = "1GeV-wood-data/detectingSheet" + to_string(i*10) + "cm.txt";
            myFileX.open(filename, ios::out | ios::app);
            myFileX << localPos.x()/cm << " " << localPos.y()/cm << " " << angle << endl;
            myFileX.close();
        } else {
            ofstream myFileX;
            string filename = "1GeV-wood-data/detectingSheet" + to_string(i*10) + "cm.txt";
            myFileX.open(filename, ios::out | ios::app);
            myFileX << "No" << endl;
            myFileX.close();
        }
    }

}

