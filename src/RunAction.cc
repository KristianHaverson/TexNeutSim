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




RunAction::RunAction(DetectorConstruction* det, PrimaryGeneratorAction* prim)
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


void RunAction::BeginOfRunAction(const G4Run*)
{
  fRootFile = new TFile("output.root", "RECREATE");
  fTree = new TTree("events", "Energy Deposition");
  //fTree->Branch("EdepMap", &fEdepMap);
   fTree->Branch("VolumeNames", &fVolumeNames);
  fTree->Branch("EdepValues", &fEdepValues);
}


////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
void RunAction::EndOfRunAction(const G4Run* run){
  fTree->Write();
  fRootFile->Close();
  return;
}
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////


//void RunAction::FillPerEvent(const std::map<G4String, G4double>& edepMap) {
//    Clear();
//    fEdepMap = edepMap;
//    fTree->Fill();
//}
void RunAction::FillPerEvent(const std::map<G4String, G4double>& edepMap) {
    // Clear previous data
    fVolumeNames.clear();
    fEdepValues.clear();

    // Populate vectors with current event's data
    for (const auto& entry : edepMap) {
        fVolumeNames.push_back(entry.first);
        fEdepValues.push_back(entry.second);
    }

    // Fill the tree for this event
    fTree->Fill();
}

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

void RunAction::Clear() {
  fEdepMap.clear();
}
