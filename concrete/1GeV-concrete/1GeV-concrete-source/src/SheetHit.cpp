//
//  SheetHit.cpp
//
//
//  Created by Joe Winterburn.
//


#include "SheetHit.hh"

#include "G4VVisManager.hh"
#include "G4VisAttributes.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4AttDefStore.hh"
#include "G4AttDef.hh"
#include "G4AttValue.hh"
#include "G4UIcommand.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"

G4ThreadLocal G4Allocator<SheetHit>* SheetHitAllocator;

SheetHit::SheetHit() : G4VHit(), mLocalPos(0), mID(-1), mAngle(0) {}

SheetHit::SheetHit(G4int i) : G4VHit(), mLocalPos(0), mID(i) {}

SheetHit::~SheetHit() {}

