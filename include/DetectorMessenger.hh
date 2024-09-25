#ifndef DetectorMessenger_h
#define DetectorMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class DetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithAnInteger;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;

class DetectorMessenger: public G4UImessenger
{
  public:
    DetectorMessenger(DetectorConstruction*);
    virtual ~DetectorMessenger();
    
    virtual void SetNewValue(G4UIcommand*, G4String);
    
  private:
    DetectorConstruction* fDetector;
    
    G4UIdirectory*        fDetDir;
    
    G4UIcmdWithAnInteger* fSetNumberOfBarsCmd;
    G4UIcmdWithAnInteger* fSetCrystalsPerBarCmd;
    G4UIcmdWithAString*   fSetWorldMaterialCmd;
    G4UIcmdWithAString*   fSetCrystalMaterialCmd;
    G4UIcmdWithAString*   fSetGreaseMaterialCmd;
    G4UIcmdWithAString*   fSetCoverMaterialCmd;

    G4UIcmdWithADoubleAndUnit* fCrystalSizeCmd;
    G4UIcmdWithADoubleAndUnit* fGreaseThicknessCmd;
    G4UIcmdWithADoubleAndUnit* fBarSpacingCmd;
    G4UIcmdWithADoubleAndUnit* fCoverThicknessCmd;
    G4UIcmdWithADoubleAndUnit* fWorldSizeCmd;


    

    
    
};

#endif