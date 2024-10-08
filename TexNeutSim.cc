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
#include "G4UIExecutive.hh"
#include "G4VisExecutive.hh"
#include "G4RunManagerFactory.hh"
#include "Randomize.hh"
#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "ActionInitialization.hh"

int main(int argc, char** argv) {
    // Detect interactive mode (if no arguments) and define UI session
    G4UIExecutive* ui = nullptr;
    if (argc == 1) {
        ui = new G4UIExecutive(argc, argv);  // Interactive mode if no arguments
    }

    // Choose the random engine
    G4Random::setTheEngine(new CLHEP::RanecuEngine);

    // Initialize the run manager
    auto runManager = G4RunManagerFactory::CreateRunManager();

    // Set mandatory initialization classes
    DetectorConstruction* det = new DetectorConstruction;
    runManager->SetUserInitialization(det);

    PhysicsList* phys = new PhysicsList;
    runManager->SetUserInitialization(phys);

    runManager->SetUserInitialization(new ActionInitialization(det));

    // Initialize the visualization manager
    G4VisManager* visManager = nullptr;
    if (ui) {
        visManager = new G4VisExecutive;
        visManager->Initialize();
    }

    // Get the pointer to the User Interface manager
    G4UImanager* UImanager = G4UImanager::GetUIpointer();

    // If interactive mode, execute vis.mac and then start the UI session
    if (ui) {
        // Execute vis.mac in interactive mode
        G4String command = "/control/execute vis.mac";
        UImanager->ApplyCommand(command);
        
        // Start the interactive session
        ui->SessionStart();
        delete ui;
    } else {
        // Batch mode - execute the command provided as an argument
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        UImanager->ApplyCommand(command + fileName);
    }

    // Job termination
    delete visManager;
    delete runManager;

    return 0;
}


