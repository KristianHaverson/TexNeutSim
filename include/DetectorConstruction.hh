#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"
#include <vector>

class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;
class DetectorMessenger;

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    DetectorConstruction();
    virtual ~DetectorConstruction();

    // Setters
    void SetWorldMaterial(G4String materialName);
    void SetCrystalMaterial(G4String materialName);
    void SetGreaseMaterial(G4String materialName);
    void SetCoverMaterial(G4String materialName);

    void SetNumberOfBars(G4int nBars);
    void SetCrystalsPerBar(G4int nCrystals);
    void SetCrystalSize(G4double size);
    void SetGreaseThickness(G4double thickness);
    void SetBarSpacing(G4double spacing);
    void SetCoverThickness(G4double thickness);
    void SetWorldSize(G4double size);

    // Getters
    G4double GetWorldSize() const { return fWorldLength; }
    G4Material* GetCrystalMaterial() const { return fCrystalMaterial; }
    G4Material* GetWorldMaterial() const { return fWorldMaterial; }
    G4Material* GetGreaseMaterial() const { return fGreaseMaterial; }
    G4Material* GetCoverMaterial() const { return fCoverMaterial; }
    G4int GetNumberOfBars() const { return fNumberOfBars; }
    G4int GetCrystalsPerBar() const { return fCrystalsPerBar; }
    G4double GetCrystalSize() const{return fCrystalSize;}
    G4double GetGreaseThickness() const{return fGreaseThickness;}
    G4double GetBarSpacing() const {return fBarSpacing;}


    // Main construction method
    virtual G4VPhysicalVolume* Construct();

    void PrintParameters();

    G4int itt =0;
  private:
    // Methods
    void DefineMaterials();
    G4VPhysicalVolume* ConstructVolumes();
    void PlaceBars();
    void ConstructBar(G4ThreeVector position, G4int barIndex);
    G4LogicalVolume*  CreateCover(G4ThreeVector position, G4int barIndex);
    void CreateCrystal(G4LogicalVolume* parentVolume, G4ThreeVector position, G4int barIndex, G4int crystalIndex);
    void CreateGrease(G4LogicalVolume* parentVolume, G4ThreeVector position, G4int barIndex, G4int greaseIndex);
    void CreatePanel(G4ThreeVector position, G4double sizeX, G4double sizeY, G4double sizeZ, const std::string& name);
    
    void Clean();

    // World
    G4LogicalVolume* fLWorld = nullptr;
    G4VPhysicalVolume* fPWorld = nullptr;
    G4Material* fWorldMaterial = nullptr;
    G4double fWorldLength = 0.0;

    // Crystal
    G4Material* fCrystalMaterial = nullptr;
    G4double fCrystalSize = 0.0;

    // Bar
    G4int fCrystalsPerBar = 0;
    G4double fBarLength = 0.0;
    std::vector<G4LogicalVolume*> fLCrystals;

    // Grease
    G4Material* fGreaseMaterial = nullptr;
    G4double fGreaseThickness = 0.0;
    std::vector<G4LogicalVolume*> fLGrease;

    // Cover
    G4Material* fCoverMaterial = nullptr;
    G4double fCoverThickness = 0.0;
    std::vector<G4LogicalVolume*> fLCover;


    // Multiple Bars
    G4int fNumberOfBars = 0;
    G4double fBarSpacing = 0.0;

    // Detector Messenger
    DetectorMessenger* fDetectorMessenger = nullptr;

    void UpdateLogicalVolumes(std::vector<G4LogicalVolume*>& logicalVolumes, G4Material* newMaterial);

    void LogGeometryChange();

  public:
    std::vector<std::string> scoringHandles;
    //G4int getNumberScoringVolumes()const{return fNumberOfBars *fCrystalsPerBar }; 
    //std::vector<G4LogicalVolume*> GetScoringVolumes() const { return fLCrystals; }


    //void RebuildGeometry()
;};

#endif
