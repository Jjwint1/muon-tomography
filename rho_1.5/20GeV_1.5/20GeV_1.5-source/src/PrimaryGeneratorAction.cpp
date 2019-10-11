//
//  PrimaryGeneratorAction.cpp
//
//
//  Created by Joe Winterburn.
//

#include "PrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4GenericMessenger.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"


PrimaryGeneratorAction::PrimaryGeneratorAction()
: G4VUserPrimaryGeneratorAction(), mParticleGun(0), mMessenger(0), mMuon(0), mEnergy(2*GeV) {
    
    G4int n_particle = 1;
    mParticleGun  = new G4ParticleGun(n_particle);
    
    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4String particleName;
    mMuon = particleTable->FindParticle(particleName="mu+");
    
    mParticleGun->SetParticlePosition(G4ThreeVector(0.,0.,-5.*m));
    mParticleGun->SetParticleDefinition(mMuon);
    
//    DefineCommands();
    
}

PrimaryGeneratorAction::~PrimaryGeneratorAction() {
    delete mParticleGun;
    delete mMessenger;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* event) {
    
    mParticleGun->SetParticleMomentumDirection(G4ThreeVector(0, 0, 1));
    
//    mParticleGun->SetParticleEnergy(mEnergy);
    
    mParticleGun->GeneratePrimaryVertex(event);
}

/*void PrimaryGeneratorAction::DefineCommands() {
    
    mMessenger = new G4GenericMessenger(this, "/muonTom/generator", "Primary generator control");
    
    G4GenericMessenger::Command& energyCmd = mMessenger->DeclarePropertyWithUnit("energy", "GeV", mEnergy, "Energy of primary muons");
    energyCmd.SetParameterName("E", true);
    energyCmd.SetRange("E>=0.");
    energyCmd.SetDefaultValue("1.");
}*/


