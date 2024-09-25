
#include "DetectorMessenger.hh"

#include "DetectorConstruction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcommand.hh"
#include "G4UIparameter.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithAnInteger.hh"



DetectorMessenger::DetectorMessenger(DetectorConstruction* Det)
 : G4UImessenger(),
   fDetector(Det)
{

  G4bool broadcast = false;
  fDetDir = new G4UIdirectory("/detector/",broadcast);
  fDetDir->SetGuidance("Detector construction control");

  fSetNumberOfBarsCmd = new G4UIcmdWithAnInteger("/detector/setNumberOfBars", this);
  fSetNumberOfBarsCmd->SetGuidance("Set number of bars.");
  fSetNumberOfBarsCmd->SetParameterName("nBars", false);
  fSetNumberOfBarsCmd->SetRange("nBars>0");
  fSetNumberOfBarsCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

  fSetCrystalsPerBarCmd = new G4UIcmdWithAnInteger("/detector/setCrystalsPerBar", this);
  fSetCrystalsPerBarCmd->SetGuidance("Set number of crystals per bar.");
  fSetCrystalsPerBarCmd->SetParameterName("nCrystals", false);
  fSetCrystalsPerBarCmd->SetRange("nCrystals>0");
  fSetCrystalsPerBarCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

  fSetWorldMaterialCmd = new G4UIcmdWithAString("/detector/setWorldMaterial", this);
  fSetWorldMaterialCmd->SetGuidance("Set material of the world.");
  fSetWorldMaterialCmd->SetParameterName("choice", false);
  fSetWorldMaterialCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

  fSetCrystalMaterialCmd = new G4UIcmdWithAString("/detector/setCrystalMaterial", this);
  fSetCrystalMaterialCmd->SetGuidance("Set material of the crystal.");
  fSetCrystalMaterialCmd->SetParameterName("choice", false);
  fSetCrystalMaterialCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

  fSetGreaseMaterialCmd = new G4UIcmdWithAString("/detector/setGreaseMaterial", this);
  fSetGreaseMaterialCmd->SetGuidance("Set material of the grease.");
  fSetGreaseMaterialCmd->SetParameterName("choice", false);
  fSetGreaseMaterialCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

  fSetCoverMaterialCmd = new G4UIcmdWithAString("/detector/setCoverMaterial", this);
  fSetCoverMaterialCmd->SetGuidance("Set material of the cover.");
  fSetCoverMaterialCmd->SetParameterName("choice", false);
  fSetCoverMaterialCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

  fCoverThicknessCmd = new G4UIcmdWithADoubleAndUnit("/detector/setCoverThickness", this);
  fCoverThicknessCmd->SetGuidance("Set cover thickness.");
  fCoverThicknessCmd->SetParameterName("choice", false);
  fCoverThicknessCmd->AvailableForStates(G4State_PreInit, G4State_Idle);


  fCrystalSizeCmd = new G4UIcmdWithADoubleAndUnit("/detector/setCrystalSize", this);
  fCrystalSizeCmd->SetGuidance("Set material of the grease.");
  fCrystalSizeCmd->SetParameterName("choice", false);
  fCrystalSizeCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

  fGreaseThicknessCmd = new G4UIcmdWithADoubleAndUnit("/detector/setGreaseThickness", this);
  fGreaseThicknessCmd->SetGuidance("Set material of the grease.");
  fGreaseThicknessCmd->SetParameterName("choice", false);
  fGreaseThicknessCmd->AvailableForStates(G4State_PreInit, G4State_Idle);


  fBarSpacingCmd = new G4UIcmdWithADoubleAndUnit("/detector/setBarSpacing", this);
  fBarSpacingCmd->SetGuidance("Set material of the grease.");
  fBarSpacingCmd->SetParameterName("choice", false);
  fBarSpacingCmd->AvailableForStates(G4State_PreInit, G4State_Idle);



  fWorldSizeCmd = new G4UIcmdWithADoubleAndUnit("/detector/setWorldSize", this);
  fWorldSizeCmd->SetGuidance("Set world size.");
  fWorldSizeCmd->SetParameterName("choice", false);
  fWorldSizeCmd->AvailableForStates(G4State_PreInit, G4State_Idle);


}
DetectorMessenger::~DetectorMessenger()
{
  delete fSetNumberOfBarsCmd;
  delete fSetCrystalsPerBarCmd;
  delete fSetWorldMaterialCmd;
  delete fSetCrystalMaterialCmd;
  delete fSetGreaseMaterialCmd;
  delete fSetCoverMaterialCmd;
  delete fDetDir;
}

void DetectorMessenger::SetNewValue(G4UIcommand* command, G4String newValue){
  
  if (command == fSetNumberOfBarsCmd) {
    G4int nBars = fSetNumberOfBarsCmd->GetNewIntValue(newValue);
    fDetector->SetNumberOfBars(nBars);
    //G4cout << "Number of bars set to: " << nBars << G4endl;  // Debug statement
  }
  else if (command == fSetCrystalsPerBarCmd) {
    G4int nCrystals = fSetCrystalsPerBarCmd->GetNewIntValue(newValue);
    fDetector->SetCrystalsPerBar(nCrystals);
    //G4cout << "Number of crystals per bar set to: " << nCrystals << G4endl;  // Debug statement
  }
  else if (command == fSetWorldMaterialCmd) {
    fDetector->SetWorldMaterial(newValue);
    //G4cout << "World material set to: " << newValue << G4endl;  // Debug statement
  }
  else if (command == fSetCrystalMaterialCmd) {
    fDetector->SetCrystalMaterial(newValue);
    //G4cout << "Crystal material set to: " << newValue << G4endl;  // Debug statement
  }
  else if (command == fSetCoverMaterialCmd) {
    fDetector->SetCoverMaterial(newValue);
    //G4cout << "Crystal material set to: " << newValue << G4endl;  // Debug statement
  }
  else if (command == fSetGreaseMaterialCmd) {
    fDetector->SetGreaseMaterial(newValue);
    //G4cout << "Grease material set to: " << newValue << G4endl;  // Debug statement
  }  else  if(command == fCrystalSizeCmd) {
    fDetector->SetCrystalSize(fCrystalSizeCmd->GetNewDoubleValue(newValue));

  } else if (command == fGreaseThicknessCmd) {
    fDetector->SetGreaseThickness(fGreaseThicknessCmd->GetNewDoubleValue(newValue));

  } else if (command == fBarSpacingCmd) {
    fDetector->SetBarSpacing(fBarSpacingCmd->GetNewDoubleValue(newValue));
    
  }else if (command == fCoverThicknessCmd) {
    fDetector->SetCoverThickness(fBarSpacingCmd->GetNewDoubleValue(newValue));
  }else if (command == fWorldSizeCmd) {
    fDetector->SetWorldSize(fBarSpacingCmd->GetNewDoubleValue(newValue));
  }


}