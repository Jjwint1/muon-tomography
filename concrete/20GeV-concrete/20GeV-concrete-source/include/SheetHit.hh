//
//  SheetHit.hh
//
//
//  Created by Joe Winterburn on 12/08/2019.
//

#ifndef SheetHit_h
#define SheetHit_h

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"

class SheetHit : public G4VHit {
    
public:
    
    SheetHit();
    SheetHit(G4int i);
    
    ~SheetHit();
    
    void SetLocalPos(G4ThreeVector xyz) { mLocalPos = xyz; }
    G4ThreeVector GetLocalPos() const { return mLocalPos; }
    void SetMomentum(G4ThreeVector xyz) {mMomentum = xyz;}
    G4ThreeVector GetMomentum() {return mMomentum;}
    void SetAngle(G4double a) {mAngle = a;}
    G4double GetAngle() {return mAngle;}
    
    inline void *operator new(size_t);
    inline void operator delete(void*aHit);
    
private:
    G4ThreeVector mLocalPos;
    G4ThreeVector mMomentum;
    G4double mAngle;
    G4int mID;
};

typedef G4THitsCollection<SheetHit> SheetHitsCollection;

extern G4ThreadLocal G4Allocator<SheetHit>* SheetHitAllocator;

inline void* SheetHit::operator new(size_t) {
    if(!SheetHitAllocator) {
        SheetHitAllocator = new G4Allocator<SheetHit>;
    }
    return (void*)SheetHitAllocator->MallocSingle();
}

inline void SheetHit::operator delete(void*aHit) {
    SheetHitAllocator->FreeSingle((SheetHit*) aHit);
}

#endif

