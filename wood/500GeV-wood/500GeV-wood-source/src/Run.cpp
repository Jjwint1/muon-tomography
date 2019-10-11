
#include "Run.hh"

Run::Run() : G4Run() {
    
}

void Run::RecordEvent(const G4Event* evt) {
    G4Run::RecordEvent(evt);
}



