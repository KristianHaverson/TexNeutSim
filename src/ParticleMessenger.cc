
#include "ParticleMessenger.hh"
#include "PrimaryGeneratorAction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithABool.hh"

ParticleMessenger::ParticleMessenger(PrimaryGeneratorAction* primGen)
 : G4UImessenger(),
   fPrim(primGen)
{

    ////////////////////////////////////////////////////////////////

    G4bool broadcast = false;

    // Source directory
    fSourceDir = new G4UIdirectory("/source/", broadcast);
    fSourceDir->SetGuidance("Source settings.");

    fDirectionDir = new G4UIdirectory("/source/direction/", broadcast);
    fDirectionDir->SetGuidance("Direction settings.");

    fPositionDir = new G4UIdirectory("/source/position/", broadcast);
    fPositionDir->SetGuidance("Position settings.");


    ////////////////////////////////////////////////////////////////

    // Particle type command
    fSetParticleCmd = new G4UIcmdWithAString("/source/particleName", this);
    fSetParticleCmd->SetGuidance("Set particle type.");
    fSetParticleCmd->SetParameterName("ParticleName", false);
    fSetParticleCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    // Energy commands
    fSetEnergyCmd = new G4UIcmdWithADoubleAndUnit("/source/energy", this);
    fSetEnergyCmd->SetGuidance("Set particle energy.");
    fSetEnergyCmd->SetParameterName("Energy", false);
    fSetEnergyCmd->SetUnitCategory("Energy");
    fSetEnergyCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fSetUniformECmd = new G4UIcmdWithABool("/source/uniformEnergy", this);
    fSetUniformECmd->SetGuidance("Enable uniform energy distribution.");
    fSetUniformECmd->SetParameterName("UniformEnergy", false);
    fSetUniformECmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fSetMinEnergyCmd = new G4UIcmdWithADoubleAndUnit("/source/minEnergy", this);
    fSetMinEnergyCmd->SetGuidance("Set minimum energy for uniform distribution.");
    fSetMinEnergyCmd->SetParameterName("MinEnergy", false);
    fSetMinEnergyCmd->SetUnitCategory("Energy");
    fSetMinEnergyCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fSetMaxEnergyCmd = new G4UIcmdWithADoubleAndUnit("/source/maxEnergy", this);
    fSetMaxEnergyCmd->SetGuidance("Set maximum energy for uniform distribution.");
    fSetMaxEnergyCmd->SetParameterName("MaxEnergy", false);
    fSetMaxEnergyCmd->SetUnitCategory("Energy");
    fSetMaxEnergyCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    // Source position commands
    fSetSourcePositionCmd = new G4UIcmdWith3VectorAndUnit("/source/position", this);
    fSetSourcePositionCmd->SetGuidance("Set the source position.");
    fSetSourcePositionCmd->SetParameterName("X", "Y", "Z", false);
    fSetSourcePositionCmd->SetUnitCategory("Length");
    fSetSourcePositionCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fSetRandomPositionCmd = new G4UIcmdWithABool("/source/position/random", this);
    fSetRandomPositionCmd->SetGuidance("Set random source position (true/false).");
    fSetRandomPositionCmd->SetParameterName("RandomPosition", false);
    fSetRandomPositionCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fSetIsotropicDirectionCmd = new G4UIcmdWithABool("/source/direction/isotropic", this);
    fSetIsotropicDirectionCmd->SetGuidance("Set isotropic emission (true/false).");
    fSetIsotropicDirectionCmd->SetParameterName("Isotropic", false);
    fSetIsotropicDirectionCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fSetMinThetaCmd = new G4UIcmdWithADoubleAndUnit("/source/direction/minTheta", this);
    fSetMinThetaCmd->SetGuidance("Set minimum theta angle.");
    fSetMinThetaCmd->SetParameterName("MinTheta", false);
    fSetMinThetaCmd->SetUnitCategory("Angle");
    fSetMinThetaCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fSetMaxThetaCmd = new G4UIcmdWithADoubleAndUnit("/source/direction/maxTheta", this);
    fSetMaxThetaCmd->SetGuidance("Set maximum theta angle.");
    fSetMaxThetaCmd->SetParameterName("MaxTheta", false);
    fSetMaxThetaCmd->SetUnitCategory("Angle");
    fSetMaxThetaCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fSetMinPhiCmd = new G4UIcmdWithADoubleAndUnit("/source/direction/minPhi", this);
    fSetMinPhiCmd->SetGuidance("Set minimum phi angle.");
    fSetMinPhiCmd->SetParameterName("MinPhi", false);
    fSetMinPhiCmd->SetUnitCategory("Angle");
    fSetMinPhiCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fSetMaxPhiCmd = new G4UIcmdWithADoubleAndUnit("/source/direction/maxPhi", this);
    fSetMaxPhiCmd->SetGuidance("Set maximum phi angle.");
    fSetMaxPhiCmd->SetParameterName("MaxPhi", false);
    fSetMaxPhiCmd->SetUnitCategory("Angle");
    fSetMaxPhiCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
}

ParticleMessenger::~ParticleMessenger()
{
    // Delete directories
    delete fSourceDir;
    delete fDirectionDir;

    // Delete commands
    delete fSetParticleCmd;
    delete fSetEnergyCmd;
    delete fSetUniformECmd;
    delete fSetMinEnergyCmd;
    delete fSetMaxEnergyCmd;
    delete fSetSourcePositionCmd;
    delete fSetRandomPositionCmd;
    delete fSetIsotropicDirectionCmd;
    delete fSetMinThetaCmd;
    delete fSetMaxThetaCmd;
    delete fSetMinPhiCmd;
    delete fSetMaxPhiCmd;
}

void ParticleMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
    if (command == fSetParticleCmd) {
        fPrim->SetParticleName(newValue);
    } else if (command == fSetEnergyCmd) {
        fPrim->SetParticleEnergy(fSetEnergyCmd->GetNewDoubleValue(newValue));
    } else if (command == fSetUniformECmd) {
        fPrim->SetUniformEnergy(fSetUniformECmd->GetNewBoolValue(newValue)); ////
    } else if (command == fSetMinEnergyCmd) {
        fPrim->SetMinEnergy(fSetMinEnergyCmd->GetNewDoubleValue(newValue));
    } else if (command == fSetMaxEnergyCmd) {
        fPrim->SetMaxEnergy(fSetMaxEnergyCmd->GetNewDoubleValue(newValue));
    } else if (command == fSetSourcePositionCmd) {
        fPrim->SetSourcePosition(fSetSourcePositionCmd->GetNew3VectorValue(newValue));
    } else if (command == fSetRandomPositionCmd) {
        fPrim->SetRandomPosition(fSetRandomPositionCmd->GetNewBoolValue(newValue));
    } else if (command == fSetIsotropicDirectionCmd) {
        fPrim->SetIsotropic(fSetIsotropicDirectionCmd->GetNewBoolValue(newValue));
    } else if (command == fSetMinThetaCmd) {
        fPrim->SetMinTheta(fSetMinThetaCmd->GetNewDoubleValue(newValue));
    } else if (command == fSetMaxThetaCmd) {
        fPrim->SetMaxTheta(fSetMaxThetaCmd->GetNewDoubleValue(newValue));
    } else if (command == fSetMinPhiCmd) {
        fPrim->SetMinPhi(fSetMinPhiCmd->GetNewDoubleValue(newValue));
    } else if (command == fSetMaxPhiCmd) {
        fPrim->SetMaxPhi(fSetMaxPhiCmd->GetNewDoubleValue(newValue));
    }
}