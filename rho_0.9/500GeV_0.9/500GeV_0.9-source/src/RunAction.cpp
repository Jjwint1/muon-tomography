//
//  RunAction.cpp
//
//
//  Created by Joe Winterburn.
//

#include "RunAction.hh"
#include "Analysis.hh"

#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include "Run.hh"

#include <iostream>

RunAction::RunAction() : G4UserRunAction(), runNumber(0) {
    
}

RunAction::~RunAction() {
    
    delete G4AnalysisManager::Instance();
}


void RunAction::BeginOfRunAction(const G4Run*) {
    
}

void RunAction::EndOfRunAction(const G4Run* run) {
    runNumber++;
    G4cout << runNumber << G4endl;
    std::cout << runNumber << std::endl;
}

G4Run* RunAction::GenerateRun() {
    return new Run();
}



