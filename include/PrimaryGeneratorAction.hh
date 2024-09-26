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
/// \file PrimaryGeneratorAction.hh
/// \brief Definition of the PrimaryGeneratorAction class
//
//
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "globals.hh"

class G4Event;
class DetectorConstruction;
class ParticleMessenger;
class RunAction;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    PrimaryGeneratorAction(DetectorConstruction*, RunAction* run);    
   ~PrimaryGeneratorAction();

  public:
    virtual void GeneratePrimaries(G4Event*);
    G4ParticleGun* GetParticleGun() {return fParticleGun;};

  private:

    RunAction* fRun;
    G4ParticleGun* fParticleGun;
    G4ParticleDefinition* fParticleDef;

    DetectorConstruction*  fDetector;
    ParticleMessenger* fParticleMessenger = nullptr;

    void PrintSettings();

    // Particle properties
    G4double fEnergy       ;// = 1.0 * MeV; 
    G4bool fUniformE       ;// = false;
    G4double fMinEnergy    ;// = 0.0;     
    G4double fMaxEnergy    ;// = 10.0 * MeV; 
    
    G4ThreeVector fPosition;// = G4ThreeVector(0., 0., 0.);
    G4bool fRandomPosition  ;//= false; 
    
    G4bool fIsotropic       ;//= false;      
    G4double fMinTheta ;//= -180.0 * deg;    
    G4double fMaxTheta ;//= 180.0 * deg; 
    G4double fMinPhi   ;//= 0.0;         
    G4double fMaxPhi   ;//= 360.0 * deg; 



  public:
    void SetParticleName(const G4String&);

    void SetParticleEnergy(G4double energy) { fEnergy = energy; }
    void SetUniformEnergy(G4bool flag) { fUniformE = flag; }
    void SetMinEnergy(G4double energy) { fMinEnergy = energy; }
    void SetMaxEnergy(G4double energy) { fMaxEnergy = energy; }
    
    void SetSourcePosition(const G4ThreeVector& pos) { fPosition = pos; }
    void SetRandomPosition(G4bool flag) { fRandomPosition = flag; }
    
    void SetIsotropic(G4bool flag) { fIsotropic = flag; }
    void SetMinTheta(G4double theta) { fMinTheta = theta; }
    void SetMaxTheta(G4double theta) { fMaxTheta = theta; }
    void SetMinPhi(G4double phi) { fMinPhi = phi; }
    void SetMaxPhi(G4double phi) { fMaxPhi = phi; }



};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

