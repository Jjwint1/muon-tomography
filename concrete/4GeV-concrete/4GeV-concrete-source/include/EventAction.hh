//
//  EventAction.hh
//
//
//  Created by Joe Winterburn.
//

#ifndef EventAction_h
#define EventAction_h


#include "G4UserEventAction.hh"
#include "globals.hh"
#include "G4THitsCollection.hh"
#include "SheetHit.hh"

#include <vector>


class EventAction : public G4UserEventAction
{
public:
    EventAction();
    virtual ~EventAction();
    
    virtual void BeginOfEventAction(const G4Event*);
    virtual void EndOfEventAction(const G4Event*);
    
private:
    
    std::vector<G4int> mHCIDs;
    
    std::vector<SheetHitsCollection*> mHitsCollections;
    
};


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

