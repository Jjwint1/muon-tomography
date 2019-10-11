//
//  500GeV-concrete.cpp
//  
//
//  Created by Joe Winterburn on 3/09/2019.
//

#include "DetectorConstruction.hh"
#include "ActionInitialization.hh"

#include "G4RunManager.hh"

#include "G4UImanager.hh"
#include "G4StepLimiterPhysics.hh"

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "G4PhysListFactory.hh"

#include "G4ScoringManager.hh"


int main(int argc, char** argv) {
    
    //Detect interactive mode (if no argument) and define UI session
    G4UIExecutive* ui = 0;
    if ( argc == 1 ) { //No commands line argument
        //Let G4UIExecutive guess what is the best available UI
        ui = new G4UIExecutive(argc,argv);
    }
    
    G4ScoringManager * scManager = G4ScoringManager::GetScoringManager();
    scManager->SetVerboseLevel(1);
    
    G4RunManager* runManager = new G4RunManager;
    
    // Mandatory user initialization classes
    
    //====================
    //The Geometry
    
    DetectorConstruction* detector = new DetectorConstruction();
    runManager->SetUserInitialization(detector);
    
    //====================
    //The Physics
    G4PhysListFactory *physListFactory = new G4PhysListFactory();
    G4VUserPhysicsList *physicsList =
    physListFactory->GetReferencePhysList("QGSP_BERT");
    runManager->SetUserInitialization(physicsList);
    
    //====================
    // User action initialization
    runManager->SetUserInitialization(new ActionInitialization());
    
    // Visualization manager construction
    G4VisManager* visManager = new G4VisExecutive;
    visManager->Initialize();
    
    // Get the pointer to the User Interface manager
    G4UImanager* UImanager = G4UImanager::GetUIpointer();
    
    if (argc>1) {
        // execute an argument macro file if exist
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        UImanager->ApplyCommand(command+fileName);
    }
    else {
        //We have visualization, initialize defaults: look in init_vis.mac macro
        UImanager->ApplyCommand("/control/execute init_vis.mac");
        UImanager->ApplyCommand("/vis/verbose 0");
        UImanager->ApplyCommand("/gun/energy 20 GeV");
        if (ui->IsGUI() ) {
            UImanager->ApplyCommand("/control/execute gui.mac");
        }
        ui->SessionStart();
        delete ui;
    }

    
    
    // Job termination
    // Free the store: user actions, physics_list and detector_description are
    // owned and deleted by the run manager, so they should not be deleted
    // in the main() program !
    
    delete visManager;
    delete runManager;
    
    return 0;
    
}
