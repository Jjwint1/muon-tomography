//
//  PrimaryGeneratorAction.hh
//
//
//  Created by Joe Winterburn.
//



#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"

class G4ParticleGun;
class G4GenericMessenger;
class G4Event;
class G4ParticleDefinition;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
    
public:
    PrimaryGeneratorAction();
    virtual ~PrimaryGeneratorAction();

    virtual void GeneratePrimaries(G4Event*);

    void SetEnergy(G4double val) { mEnergy = val; }
    G4double GetEnergy() const { return mEnergy; }

private:
    void DefineCommands();
    
    G4ParticleGun* mParticleGun;
    G4GenericMessenger* mMessenger;
    G4ParticleDefinition* mMuon;
    G4double mEnergy;
    
};

#endif
