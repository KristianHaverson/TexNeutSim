#ifndef ParticleMessenger_h
#define ParticleMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class PrimaryGeneratorAction;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWith3VectorAndUnit;
class G4UIcmdWithABool;

class ParticleMessenger : public G4UImessenger
{
public:
    ParticleMessenger(PrimaryGeneratorAction*);
    virtual ~ParticleMessenger();
    
    virtual void SetNewValue(G4UIcommand*, G4String);
    
private:
    PrimaryGeneratorAction* fPrim;
    
    // Directories
    G4UIdirectory* fSourceDir;
    G4UIdirectory* fDirectionDir;
    G4UIdirectory* fPositionDir;

    // Particle commands
    G4UIcmdWithAString* fSetParticleCmd;
    
    // Energy commands
    G4UIcmdWithADoubleAndUnit* fSetEnergyCmd;
    G4UIcmdWithABool* fSetUniformECmd;
    G4UIcmdWithADoubleAndUnit* fSetMinEnergyCmd;
    G4UIcmdWithADoubleAndUnit* fSetMaxEnergyCmd;
    
    // Position commands
    G4UIcmdWith3VectorAndUnit* fSetSourcePositionCmd;
    G4UIcmdWithABool* fSetRandomPositionCmd;

    // Direction commands
    G4UIcmdWithABool* fSetIsotropicDirectionCmd;
    G4UIcmdWithADoubleAndUnit* fSetMinThetaCmd;
    G4UIcmdWithADoubleAndUnit* fSetMaxThetaCmd;
    G4UIcmdWithADoubleAndUnit* fSetMinPhiCmd;
    G4UIcmdWithADoubleAndUnit* fSetMaxPhiCmd;
};

#endif
