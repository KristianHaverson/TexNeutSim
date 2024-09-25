//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
#include "DetectorConstruction.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4RunManager.hh"
#include "DetectorMessenger.hh"
#include "G4VisAttributes.hh"
#include "G4SubtractionSolid.hh"

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////

DetectorConstruction::DetectorConstruction(){
  
  // INIT //
  fWorldLength     = 0.1*m;
  fCrystalSize     = 2.0*cm;
  fCrystalsPerBar  = 6;
  fGreaseThickness = 1.0*mm;
  fNumberOfBars    = 2;
  fBarSpacing      = 5.0*cm;
  fCoverThickness=1*mm;
  
  DefineMaterials(); 

  // READ-IN //
  fDetectorMessenger = new DetectorMessenger(this);

  fBarLength = fCrystalsPerBar * fCrystalSize + (fCrystalsPerBar - 1) * fGreaseThickness;
}


///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////

DetectorConstruction::~DetectorConstruction(){
    delete fDetectorMessenger;
}

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////

void DetectorConstruction::DefineMaterials(){

    G4NistManager* nistManager = G4NistManager::Instance();

    SetWorldMaterial("G4_AIR");
    SetCrystalMaterial("G4_TERPHENYL");


    G4double a, z, density;
    G4int ncomponents, natoms;
    G4Element* H  = new G4Element("Hydrogen", "H", z=1.,  a=1.01*g/mole);
    G4Element* C  = new G4Element("Carbon",   "C", z=6.,  a=12.01*g/mole);
    G4Element* O  = new G4Element("Oxygen",   "O", z=8.,  a=16.00*g/mole);
    G4Element* Si = new G4Element("Silicon",  "Si", z=14., a=28.09*g/mole);
    density = 1.1*g/cm3; 
    G4Material* silicone = new G4Material("Silicone", density, ncomponents=4);
    silicone->AddElement(C, natoms=2);
    silicone->AddElement(H, natoms=6);
    silicone->AddElement(O, natoms=1);
    silicone->AddElement(Si, natoms=1);
    SetGreaseMaterial("Silicone");

    SetCoverMaterial("G4_Al");

}

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////

void DetectorConstruction::SetCrystalSize(G4double size) {
  fCrystalSize = size;
  fBarLength = fCrystalsPerBar * fCrystalSize + (fCrystalsPerBar - 1) * fGreaseThickness;

}

void DetectorConstruction::SetGreaseThickness(G4double thickness) {
  fGreaseThickness = thickness;
  fBarLength = fCrystalsPerBar * fCrystalSize + (fCrystalsPerBar - 1) * fGreaseThickness;

}

void DetectorConstruction::SetBarSpacing(G4double spacing) {
  fBarSpacing = spacing;
  LogGeometryChange();

}

void DetectorConstruction::SetCrystalsPerBar(G4int number){
  fCrystalsPerBar = number;
  fBarLength = fCrystalsPerBar * fCrystalSize + (fCrystalsPerBar - 1) * fGreaseThickness;

}

void DetectorConstruction::SetNumberOfBars(G4int number){
  fNumberOfBars = number;

}



void DetectorConstruction::SetCoverThickness(G4double thickness){
  fCoverThickness = thickness;
  LogGeometryChange();


}
void DetectorConstruction::SetWorldSize(G4double size){
  fWorldLength = size;

}


void DetectorConstruction::SetWorldMaterial(G4String materialChoice){

  fWorldMaterial = G4NistManager::Instance()->FindOrBuildMaterial(materialChoice);   
  

  if(fWorldMaterial!=nullptr) { 
  }else {
    G4cout << G4endl;
    G4cout << "\n--> warning from DetectorConstruction::SetWorldMaterial : "
           << materialChoice << " not found" << G4endl;
    G4cout << G4endl;
  } 


}

void DetectorConstruction::SetCrystalMaterial(G4String materialChoice) {
    fCrystalMaterial = G4NistManager::Instance()->FindOrBuildMaterial(materialChoice);
    if (fCrystalMaterial) {
        UpdateLogicalVolumes(fLCrystals, fCrystalMaterial);
    } else {
    G4cout << G4endl;

        G4cout << "\n--> warning from DetectorConstruction::SetCrystalMaterial : "
               << materialChoice << " not found" << G4endl;
    G4cout << G4endl;

    }

}

void DetectorConstruction::SetGreaseMaterial(G4String materialChoice) {
    fGreaseMaterial = G4NistManager::Instance()->FindOrBuildMaterial(materialChoice);
    if (fGreaseMaterial) {
        UpdateLogicalVolumes(fLGrease, fGreaseMaterial);
    } else {
    G4cout << G4endl;

        G4cout << "\n--> warning from DetectorConstruction::SetGreaseMaterial : "
               << materialChoice << " not found" << G4endl;
      G4cout << G4endl;

    }
          itt=11;
  G4cout << "HERE " << itt << G4endl;

}


void DetectorConstruction::SetCoverMaterial(G4String materialChoice) {
    fCoverMaterial = G4NistManager::Instance()->FindOrBuildMaterial(materialChoice);
    if (fCoverMaterial) {
      UpdateLogicalVolumes(fLCover, fCoverMaterial);
    } else {
    G4cout << G4endl;

        G4cout << "\n--> warning from DetectorConstruction::SetGreaseMaterial : "
               << materialChoice << " not found" << G4endl;
      G4cout << G4endl;

    }
              itt=12;
  G4cout << "HERE " << itt << G4endl;

}



void DetectorConstruction::UpdateLogicalVolumes(std::vector<G4LogicalVolume*>& logicalVolumes, G4Material* newMaterial) {
    for (auto* volume : logicalVolumes) {
        volume->SetMaterial(newMaterial);
    }

}
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////





G4VPhysicalVolume* DetectorConstruction::Construct(){
    return ConstructVolumes();
}


G4VPhysicalVolume* DetectorConstruction::ConstructVolumes(){


  Clean();

  G4Box* solidWorld = new G4Box("World", 0.5*fWorldLength, 0.5*fWorldLength, 0.5*fWorldLength);
  fLWorld = new G4LogicalVolume(solidWorld, fWorldMaterial, "World");
  fPWorld = new G4PVPlacement(0, G4ThreeVector(), fLWorld, "World", 0, false, 0, true);


  // penultimate is copy number
  
  PlaceBars();

  PrintParameters();


  return fPWorld;
}


void DetectorConstruction::PlaceBars(){
  G4double totalWidth = fNumberOfBars * fCrystalSize + (fNumberOfBars - 1) * fBarSpacing;
  G4double startPosition = -totalWidth / 2 + fCrystalSize / 2;
  
  for (G4int i = 0; i < fNumberOfBars; i++){
      G4double positionX = startPosition + i * (fCrystalSize + fBarSpacing);
      G4ThreeVector position(positionX, 0, 0);
      ConstructBar(position, i);
  }
}


void DetectorConstruction::ConstructBar(G4ThreeVector position, G4int barIndex) {

    fLCover.push_back(CreateCover(position, barIndex));

    for (G4int i = 0; i < fCrystalsPerBar; i++) {
        G4double crystalPositionY = (i - (fCrystalsPerBar - 1) / 2.0) * (fCrystalSize + fGreaseThickness);
        G4ThreeVector crystalPosition(position.x(), crystalPositionY, 0);

        CreateCrystal(fLWorld, crystalPosition, barIndex, i);
        //CreateCrystal(fLCover.back(), crystalPosition, barIndex, i);

        if (i < fCrystalsPerBar - 1) {
            G4double greasePositionY = crystalPositionY + 0.5 * fCrystalSize + 0.5 * fGreaseThickness;
            G4ThreeVector greasePosition(position.x(), greasePositionY, 0);
            CreateGrease(fLWorld, greasePosition, barIndex, i);
            //CreateGrease(fLCover.back() , greasePosition, barIndex, i);
        }
    }
}


G4LogicalVolume* DetectorConstruction::CreateCover(G4ThreeVector position, G4int barIndex) {
    
    G4double totalBarLengthY = fCrystalsPerBar * fCrystalSize + (fCrystalsPerBar - 1) * fGreaseThickness;
    
    G4double innerSizeX = fCrystalSize;      // Width along the X-axis
    G4double innerSizeY = totalBarLengthY;   // Height along the Y-axis
    G4double innerSizeZ = fCrystalSize;      // Thickness along the Z-axis

    G4double outerSizeX = innerSizeX + fCoverThickness; // Outer width
    G4double outerSizeY = innerSizeY  - 0.009 * mm;                   // Outer height (same as inner height)
    G4double outerSizeZ = innerSizeZ + fCoverThickness; // Outer thickness

    // Create the outer and inner boxes for the cover
    G4Box* solidOuterBox = new G4Box("OuterBox", 0.5 * outerSizeX, 0.5 * outerSizeY, 0.5 * outerSizeZ);
    G4Box* solidInnerBox = new G4Box("InnerBox", 0.5 * innerSizeX, 0.5 * innerSizeY, 0.5 * innerSizeZ);

    G4SubtractionSolid* solidHollowCuboid = new G4SubtractionSolid("HollowCuboid", solidOuterBox, solidInnerBox, 0, G4ThreeVector(0, 0, 0));

    G4LogicalVolume* logicCompleteCover = new G4LogicalVolume(solidHollowCuboid, fCoverMaterial, "LogicalCompleteCover_" + std::to_string(barIndex));

    G4VisAttributes* coverVisAtt = new G4VisAttributes(G4Colour(0.0, 1.0, 0.0, 0.6)); 
    coverVisAtt->SetVisibility(true);
    coverVisAtt->SetForceSolid(true); 
    logicCompleteCover->SetVisAttributes(coverVisAtt);

    // Place the cover in the world volume
    new G4PVPlacement(0, position, logicCompleteCover, "PhysicalCompleteCover_" + std::to_string(barIndex), fLWorld, false, barIndex * 100 + 200, true);

    return logicCompleteCover;
}



void DetectorConstruction::CreateCrystal(G4LogicalVolume* parentVolume, G4ThreeVector position, G4int barIndex, G4int crystalIndex) {

    G4Box* solidCrystal = new G4Box("Crystal", 0.5 * fCrystalSize, 0.5 * fCrystalSize, 0.5 * fCrystalSize);
    scoringHandles.push_back("LogicalCrystal_" + std::to_string(barIndex) + "_" + std::to_string(crystalIndex));
    G4LogicalVolume* logicCrystal = new G4LogicalVolume(solidCrystal, fCrystalMaterial, scoringHandles.back());


    new G4PVPlacement(0, position, logicCrystal,"PhysicalCrystal_" + std::to_string(barIndex) + "_" + std::to_string(crystalIndex), parentVolume, false, barIndex * 100 + crystalIndex, true);
    G4cout<< " construction = "<< logicCrystal->GetName()<<G4endl;
    fLCrystals.push_back(logicCrystal);
}

void DetectorConstruction::CreateGrease(G4LogicalVolume* parentVolume, G4ThreeVector position, G4int barIndex, G4int greaseIndex) {

  G4Box* solidGrease = new G4Box("Grease", 0.5 * fCrystalSize, 0.5 * fGreaseThickness, 0.5 * fCrystalSize);
  G4LogicalVolume* logicGrease = new G4LogicalVolume(solidGrease, fGreaseMaterial, "LogicalGrease_" + std::to_string(barIndex) + "_" + std::to_string(greaseIndex));

  new G4PVPlacement(0, position, logicGrease, "PhysicalGrease_" + std::to_string(barIndex) + "_" + std::to_string(greaseIndex), parentVolume, false, barIndex * 100 + 50 + greaseIndex, true);
  fLGrease.push_back(logicGrease);
}



void DetectorConstruction::Clean(){
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();
}

void DetectorConstruction::LogGeometryChange() {
}





void DetectorConstruction::PrintParameters() {
    G4cout << " ============================================ " << G4endl;
    G4cout << " World Material: " << (fWorldMaterial ? fWorldMaterial->GetName() : "Not defined") << G4endl;
    G4cout << " Crystal Material: " << (fCrystalMaterial ? fCrystalMaterial->GetName() : "Not defined") << G4endl;
    G4cout << " Grease Material: " << (fGreaseMaterial ? fGreaseMaterial->GetName() : "Not defined") << G4endl;
    G4cout << " Cover Material: " << (fCoverMaterial ? fCoverMaterial->GetName() : "Not defined") << G4endl;
    G4cout << " Number of Bars: " << fNumberOfBars << G4endl;
    G4cout << " Crystals per Bar: " << fCrystalsPerBar << G4endl;
    G4cout << " Crystal Size: " << fCrystalSize / cm << " cm" << G4endl;
    G4cout << " Grease Thickness: " << fGreaseThickness / mm << " mm" << G4endl;
    G4cout << " Bar Spacing: " << fBarSpacing / cm << " cm" << G4endl;
    G4cout << " Cover Layer Thickness: " << fCoverThickness / mm << " mm" << G4endl;
    G4cout << "World length: " << fWorldLength / cm << " cm" << G4endl;
    G4cout << " ============================================ " << G4endl;
}

