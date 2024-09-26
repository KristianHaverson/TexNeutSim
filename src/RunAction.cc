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
/// \file RunAction.cc
/// \brief Implementation of the RunAction class
//
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "RunAction.hh"
//#include "Run.hh"
#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"
//#include "HistoManager.hh"
#include "G4AccumulableManager.hh"

#include "G4Run.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"
#include "Randomize.hh"
#include <iomanip>




RunAction::RunAction(DetectorConstruction* det)
  : G4UserRunAction(),
    fDetector(det), fRootFile(0), fTree(0)
{
    //fscoringVolumes  = fDetector->GetScoringVolumes();
//
    //if(!fDetector){
    //  G4cout<< "error in scoring volumes "<<G4endl;
//
    //}else{
    //G4cout<< "loged volumes "<<G4endl;
    //G4cout<< fscoringVolumes.at(0)->GetName()<<G4endl;
    //}
}
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////


RunAction::~RunAction(){}

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////


void RunAction::BeginOfRunAction(const G4Run*){
    
  auto now = std::chrono::system_clock::now();
  auto in_time_t = std::chrono::system_clock::to_time_t(now);
  std::stringstream ss;
  ss << std::put_time(std::localtime(&in_time_t), "%Y%m%d_%H%M%S");
  std::string filename = "simTree_" + ss.str() + ".root";

  fRootFile = new TFile(filename.c_str(), "RECREATE");
  
  fTree = new TTree("simEvents", "simEvents");
  fTree->Branch("ScoringName", &fVolumeNames);
  fTree->Branch("Energy", &fEdepValues);


  fPrimaryTree = new TTree("primaryConditions", "primaryConditions");
  fPrimaryTree->Branch("SourcePosition", &SourcePosition);
  fPrimaryTree->Branch("BeamDirection", &BeamDirection);
  fPrimaryTree->Branch("BeamEnergy", &BeamEnergy);
  fPrimaryTree->Branch("BeamName", &beamName);

  fDetectorTree = new TTree("detectorConditions", "Detector Conditions");
  fDetectorTree->Branch("ScoringName", &ScoringName);
  fDetectorTree->Branch("ScoringLocation", &ScoringLocation);
  fDetectorTree->Branch("ScoringMaterial", &ScoringMaterial);
  fDetectorTree->Branch("ScoringSize", &ScoringSize);


}


////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
void RunAction::EndOfRunAction(const G4Run* run){

  ScoringName.clear();
  ScoringLocation.clear();
  ScoringMaterial.clear();
  ScoringSize.clear();
  ScoringName       = fDetector->scoringHandles;
  ScoringMaterial  =  fDetector-> scoringMaterialNames;


  for (const auto& g4vec : fDetector-> scoringSizes) {
      TVector3 rootVec(g4vec.x(), g4vec.y(), g4vec.z());
      ScoringSize.push_back(rootVec);
  }

  for (const auto& g4vec : fDetector-> scoringPlacements) {
      TVector3 rootVec(g4vec.x(), g4vec.y(), g4vec.z());
      ScoringLocation.push_back(rootVec);
  }


  fDetectorTree->Fill();






  fTree->Write();
  fPrimaryTree->Write();
  fDetectorTree->Write();


  fRootFile->Close();
  return;
}
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////


void RunAction::FillPerEvent(const std::map<G4String, G4double>& edepMap) {

    fVolumeNames.clear();
    fEdepValues.clear();

    for (const auto& entry : edepMap) {
        fVolumeNames.push_back(entry.first);
        fEdepValues.push_back(entry.second);
    }

    // Fill the tree for this event
    fTree->Fill();
}


void RunAction::FillInitialConditions(const G4ThreeVector& Direction,
                                    const G4ThreeVector& Position,
                                    const G4double& Energy,
                                    const std::string name) {

  BeamDirection.Clear();
  SourcePosition.Clear();
  BeamEnergy=-10000;

  BeamDirection  = TVector3(Direction.x(),Direction.y(), Direction.z());
  SourcePosition = TVector3(Position.x(),Position.y(), Position.z());
  BeamEnergy = Energy;
  beamName = name;

  fPrimaryTree->Fill();


}
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

void RunAction::Clear() {
  fEdepMap.clear();
}
