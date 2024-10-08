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
/// \file SteppingAction.cc
/// \brief Implementation of the SteppingAction class
//
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "SteppingAction.hh"
//#include "Run.hh"
#include "EventAction.hh"
//#include "HistoManager.hh"
#include "DetectorConstruction.hh"
#include "G4Neutron.hh"
#include "G4Proton.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"

#include "G4RunManager.hh"
                           
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::SteppingAction(EventAction* event,DetectorConstruction* det)
: G4UserSteppingAction(), fEventAction(event), fDetector(det){ }


SteppingAction::~SteppingAction(){ }


void SteppingAction::UserSteppingAction(const G4Step* step) {
    

    G4LogicalVolume* hitVolume 
      = step->GetPreStepPoint()->GetTouchableHandle()
        ->GetVolume()->GetLogicalVolume();

    const G4Track* track = step->GetTrack();
    const G4ParticleDefinition* particle = track->GetParticleDefinition();

    if (particle) {
      for (auto& scoringHanlde : fDetector->scoringHandles) {

          if (hitVolume->GetName() == scoringHanlde) {

              G4double edepStep = step->GetTotalEnergyDeposit();


              if (edepStep > 0.) {
                  fEventAction->AddEdep(hitVolume->GetName(), edepStep);
              }
              break;
          }
      }
    }







}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
