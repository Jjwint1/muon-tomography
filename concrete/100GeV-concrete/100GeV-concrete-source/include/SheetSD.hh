//
//  SheetSD.hh
//
//
//  Created by Joe Winterburn.
//

#ifndef SheetSD_h
#define SheetSD_h

#include "G4VSensitiveDetector.hh"
#include "SheetHit.hh"

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;


class SheetSD : public G4VSensitiveDetector
{
public:
    SheetSD(G4String name);
    virtual ~SheetSD();
    
    virtual void Initialize(G4HCofThisEvent*HCE);
    virtual G4bool ProcessHits(G4Step*aStep,G4TouchableHistory*ROhist);
    
private:
    SheetHitsCollection* mSheetHitsCollection;
    G4int mHCID;
};


#endif

