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
/// \file PrimaryGeneratorAction.cc
/// \brief Implementation of the PrimaryGeneratorAction class
//
//
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "PrimaryGeneratorAction.hh"
#include "DetectorConstruction.hh"
#include "ParticleMessenger.hh"

#include "G4RandomDirection.hh"
#include "G4Event.hh"
#include "G4ParticleTable.hh"
#include "G4IonTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4Geantino.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::PrimaryGeneratorAction(DetectorConstruction* det)
    : G4VUserPrimaryGeneratorAction(), 
      fDetector(det),
      fParticleGun(new G4ParticleGun(1)) // Initialize particle gun for one particle
{   

    // initialise everything //
    fEnergy = 1.0 * MeV; 
    fUniformE = false;
    fMinEnergy = 0.0;     
    fMaxEnergy = 10.0 * MeV; 
    fPosition = G4ThreeVector(0., 0., 0.);
    fRandomPosition = false; 
    fIsotropic = false;      
    fMinTheta = -180.0 * deg;    
    fMaxTheta = 180.0 * deg; 
    fMinPhi = 0.0;         
    fMaxPhi = 360.0 * deg; 

    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    fParticleDef= particleTable->FindParticle("neutron");
    fParticleGun->SetParticleDefinition(fParticleDef);

    fParticleGun->SetParticleDefinition(fParticleDef);
    fParticleGun->SetParticleEnergy(fEnergy); 
    fParticleGun->SetParticlePosition(fPosition); 

    // read in actual settings //
    fParticleMessenger = new ParticleMessenger(this);
    


}
void PrimaryGeneratorAction::SetParticleName(const G4String& name)
{
    fParticleDef = G4ParticleTable::GetParticleTable()->FindParticle(name);
    if (fParticleDef) {
        fParticleGun->SetParticleDefinition(fParticleDef);
    }
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fParticleGun;
}

void PrimaryGeneratorAction::PrintSettings() 
{     
    G4cout<< " ******************* SETTINGS  ******************* "<<G4endl;
    G4cout << "Particle Name: " << fParticleDef->GetParticleName() << G4endl;

    if (!fUniformE) {
        G4cout << "Energy: " << fParticleGun->GetParticleEnergy() / MeV << " MeV" << G4endl;
    } else {
        G4cout << "Energy Range: [" << fMinEnergy / MeV << " MeV, " << fMaxEnergy / MeV << " MeV]" << G4endl;
    }

    if (!fRandomPosition) {
        G4cout << "Position: " << fParticleGun->GetParticlePosition() << G4endl;
    } else {
        G4cout << "Random Position: Yes" << G4endl;
    }

    if (fIsotropic) {
        G4cout << "Isotropic Direction: Yes" << G4endl;
    } else {
        G4cout << "Theta Range: [" << fMinTheta / deg << " deg, " << fMaxTheta / deg << " deg]" << G4endl;
        G4cout << "Phi Range: [" << fMinPhi / deg << " deg, " << fMaxPhi / deg << " deg]" << G4endl;
    }
    G4cout<< " ******************* SETTINGS  ******************* "<<G4endl;

}
void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent){

    ////////////////////////////////////////////////////////////////
    //// define Cf252
    //G4int Z = 98, A = 252;
    //G4double ionCharge   = 0.*eplus;
    //G4double excitEnergy = 0.*keV;
    //fParticleDef = G4IonTable::GetIonTable()->GetIon(Z,A,excitEnergy);
    //if (!fParticleDef) {
    //     G4cerr << "Error: Particle definition not found for Z=" << Z << ", A=" << A << G4endl;
    //     return; 
    //}
    //fParticleGun->SetParticleDefinition(fParticleDef);
    //fParticleGun->SetParticleCharge(ionCharge);

    //////////////////////////////////////////////////////////////
    // define energy
    if (fUniformE) {
        fParticleGun->SetParticleEnergy(fMinEnergy + G4UniformRand() * (fMaxEnergy - fMinEnergy));
    } else {
        fParticleGun->SetParticleEnergy(fEnergy); 
    }

    //////////////////////////////////////////////////////////////
    // Set position based on random position settings
    if (fRandomPosition) {

      G4double world_size = fDetector->GetWorldSize();
      G4double posX = (G4UniformRand() - 0.5) * world_size/2.0;
      G4double posY = (G4UniformRand() - 0.5) * world_size/2.0;
      G4double posZ = (G4UniformRand() - 0.5) * world_size/2.0;
      fParticleGun->SetParticlePosition(G4ThreeVector(posX, posY, posZ));
    } else {
        fParticleGun->SetParticlePosition(fPosition);
    }
    //////////////////////////////////////////////////////////////
    // Set momentum direction based on isotropic settings
    if (fIsotropic) {
        fParticleGun->SetParticleMomentumDirection(G4RandomDirection()); 
    } else {
        G4double theta = fMinTheta + G4UniformRand() * (fMaxTheta - fMinTheta);
        G4double phi = fMinPhi + G4UniformRand() * (fMaxPhi - fMinPhi);
        G4ThreeVector direction;
        direction.setRThetaPhi(1.0, theta , phi); 
        fParticleGun->SetParticleMomentumDirection(direction);
    }

   //////////////////////////////////////////////////////////////
   // Print particle settings for this generation

    G4bool debug = false;
    if(debug){
      PrintSettings();
      G4cout << " ******************* PARTICLE GENERATED ******************* " << G4endl;
      G4cout << "Particle Definition: " << fParticleDef->GetParticleName() << G4endl;
      G4cout << "Position: " << fParticleGun->GetParticlePosition() << G4endl;
      G4cout << "Momentum Direction: " << fParticleGun->GetParticleMomentumDirection() << G4endl;
      G4ThreeVector momentumDirection = fParticleGun->GetParticleMomentumDirection();
      G4double theta = momentumDirection.theta(); // Get theta
      G4double phi = momentumDirection.phi(); // Get phi
      G4cout << "Theta: " << theta / deg << " degrees" << G4endl; // Convert from radians to degrees
      G4cout << "Phi: " << phi / deg << " degrees" << G4endl; // Convert from radians to degrees
      G4double totalEnergy = fParticleGun->GetParticleEnergy();
      G4cout << "Total Energy: " << totalEnergy / MeV << " MeV" << G4endl; // Convert to MeV
      G4cout << " ******************************************************* " << G4endl;
    }
    // Generate the primary vertex for the event
    fParticleGun->GeneratePrimaryVertex(anEvent);
}

