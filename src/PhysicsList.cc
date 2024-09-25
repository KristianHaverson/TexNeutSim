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
/// \file PhysicsList.cc
/// \brief Implementation of the PhysicsList class
//

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "PhysicsList.hh"

#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

#include "G4NuclideTable.hh"

//#include "HadronElasticPhysicsHP.hh"

#include "G4HadronElasticPhysics.hh"
#include "G4HadronPhysicsFTFP_BERT_HP.hh"
#include "G4HadronPhysicsQGSP_BIC_HP.hh"
#include "G4HadronPhysicsQGSP_BIC_AllHP.hh"
#include "G4HadronInelasticQBBC.hh"
#include "G4HadronPhysicsINCLXX.hh"

#include "G4IonElasticPhysics.hh"
#include "G4IonPhysicsXS.hh"
#include "G4IonPhysicsPHP.hh"
#include "G4IonINCLXXPhysics.hh"

#include "G4StoppingPhysics.hh"
//#include "GammaNuclearPhysics.hh"

//#include "ElectromagneticPhysics.hh"
#include "G4EmStandardPhysics.hh"

#include "G4DecayPhysics.hh"

//include "RadioactiveDecayPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"

#include "G4Neutron.hh"
#include "G4ProcessManager.hh"
#include "G4HadronicInteraction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysicsList::PhysicsList()
:G4VModularPhysicsList(),
 fHadronElastic(nullptr), fHadronInelastic(nullptr),
 fIonElastic(nullptr), fIonInelastic(nullptr),
 fGammaNuclear(nullptr), fElectromagnetic(nullptr),
 fDecay(nullptr), fRadioactiveDecay(nullptr)
{

  G4int verb = 0;
  SetVerboseLevel(verb);
  
  new G4UnitDefinition( "mm2/g",  "mm2/g", "Surface/Mass", mm2/g);
  new G4UnitDefinition( "um2/mg", "um2/mg","Surface/Mass", um*um/mg);
  
  //const G4double meanLife =0;// 1*nanosecond, halfLife = meanLife*std::log(2);
  //G4NuclideTable::GetInstance()->SetThresholdOfHalfLife(0);



 // Hadron Elastic scattering
  fHadronElastic = new G4HadronElasticPhysics(verb);
  RegisterPhysics(fHadronElastic);

  // Hadron Inelastic Physics
  fHadronInelastic = new G4HadronPhysicsQGSP_BIC_HP(verb);
  RegisterPhysics(fHadronInelastic);

  // Ion Elastic Physics
  fIonElastic = new G4IonElasticPhysics(verb);
  RegisterPhysics(fIonElastic);

  // Ion Inelastic Physics
  fIonInelastic = new G4IonPhysicsXS(verb);
  RegisterPhysics(fIonInelastic);

  // stopping Particles
  RegisterPhysics( new G4StoppingPhysics(verb));

  // Gamma-Nuclear Physics
  //fGammaNuclear = new GammaNuclearPhysics("gamma");
  //RegisterPhysics(fGammaNuclear);

  // EM physics
  fElectromagnetic = new G4EmStandardPhysics(verb);
  RegisterPhysics(fElectromagnetic);

  // Decay
  fDecay = new G4DecayPhysics(verb);
  RegisterPhysics(fDecay);

  // Radioactive decay
  fRadioactiveDecay = new G4RadioactiveDecayPhysics(verb);
  RegisterPhysics(fRadioactiveDecay);
  


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysicsList::~PhysicsList()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::ConstructProcess()
{
  // Transportation first (mandatory)
  AddTransportation();

  // Physics constructors
  fHadronElastic->ConstructProcess();
  //fHadronInelastic->ConstructProcess();
  //fIonElastic->ConstructProcess();
  //fIonInelastic->ConstructProcess();
  //fGammaNuclear->ConstructProcess();
  fElectromagnetic->ConstructProcess();
  //fDecay->ConstructProcess();
  //fRadioactiveDecay->ConstructProcess();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::SetCuts()
{
  //SetCutValue(0*mm, "proton");
  //SetCutValue(10*km, "e-");
  //SetCutValue(10*km, "e+");
  //SetCutValue(10*km, "gamma");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
