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


#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "G4RunManagerFactory.hh"
#include "Randomize.hh"
#include "G4SteppingVerbose.hh"
#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "ActionInitialization.hh"

int main(int argc, char** argv) {

    G4UIExecutive* ui = nullptr;
    if (argc == 1) ui = new G4UIExecutive(argc, argv);

    G4Random::setTheEngine(new CLHEP::RanecuEngine);

    G4int precision = 4;
    G4SteppingVerbose::UseBestUnit(precision);

    auto runManager = G4RunManagerFactory::CreateRunManager();
    if (argc == 3) {
        G4int nThreads = G4UIcommand::ConvertToInt(argv[2]);
        runManager->SetNumberOfThreads(nThreads);
    }

    auto* det = new DetectorConstruction;
    runManager->SetUserInitialization(det);

    auto* phys = new PhysicsList;
    runManager->SetUserInitialization(phys);

    runManager->SetUserInitialization(new ActionInitialization(det));

    // Initialize visualization
    G4VisManager* visManager = nullptr;
    G4UImanager* UImanager = G4UImanager::GetUIpointer();

    if (ui) {
        visManager = new G4VisExecutive;
        visManager->Initialize();
        ui->SessionStart();
        delete ui;
    } else {
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        UImanager->ApplyCommand(command + fileName);
    }

    // Job termination
    delete visManager;
    delete runManager;
}



