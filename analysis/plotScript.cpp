#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TCanvas.h>
#include <iostream>
#include <vector>
#include <string>
#include <set>

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

struct HistogramData {
    std::vector<std::vector<TH1D*>> histograms;
    std::vector<TCanvas*> canvases;
};


struct PrimaryConditionData {
    std::vector<TVector3> sourcePositions;
    std::vector<TVector3> beamDirections;
    std::vector<double> beamEnergies;
    std::vector<std::string> beamNames;
};
// Define a structure to hold the detector condition data (single entry)
struct DetectorConditionData {
    std::vector<std::string> scoringNames;
    std::vector<TVector3> scoringLocations;
    std::vector<std::string> scoringMaterials;
    std::vector<TVector3> scoringSizes;
};
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

TTree* openInputFile(const std::string& filename, const std::string& key);


std::pair<int, int> getSimConfig(TTree* tree);
HistogramData createHists(int numberBars, int numberCubes);
void populateHists(TTree* tree, HistogramData& data);
void drawHists(HistogramData& data, int numberBars, int numberCubes);
void saveHists(const HistogramData& data, const std::string& outputFilename);

PrimaryConditionData readPrimaryConditions(TTree* tree);
void printPrimaryConditions(const PrimaryConditionData& data);

DetectorConditionData readDetectorConditions(TTree* tree);
void printDetectorConditions(const DetectorConditionData& data);
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

void plotScript() {

    std::string dataIN = "../build/simTree_20240926_150257.root";
    // get beam conditions //
    TTree* primaryTree = openInputFile(dataIN.c_str(), "primaryConditions");
    if (primaryTree) {
        PrimaryConditionData primaryData = readPrimaryConditions(primaryTree);
        printPrimaryConditions(primaryData);
    }


    // get detector conditions //
    TTree* detectorTree = openInputFile(dataIN.c_str(), "detectorConditions");
    if (!detectorTree) return;
    DetectorConditionData detectorConditions = readDetectorConditions(detectorTree);
    printDetectorConditions(detectorConditions);


    // get simulated data //
    TTree* tree = openInputFile(dataIN.c_str(), "simEvents");
    if (!tree) return;
    auto [numberBars, numberCubes] = getSimConfig(tree);
    std::cout << "numberBars = " << numberBars << std::endl;
    std::cout << "numberCubes = " << numberCubes << std::endl;
    HistogramData histData = createHists(numberBars, numberCubes);
    populateHists(tree, histData);
    drawHists(histData, numberBars, numberCubes);
    saveHists(histData, "outputHists.root");

}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

TTree* openInputFile(const std::string& filename, const std::string& key) {
    TFile* file = TFile::Open(filename.c_str());
    if (!file || file->IsZombie()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return nullptr;
    }
    TTree* tree = dynamic_cast<TTree*>(file->Get(key.c_str()));
    if (!tree) {
        std::cerr << "Error retrieving 'events' tree from file: " << filename << std::endl;
        return nullptr;
    }
    return tree;
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

std::pair<int, int> getSimConfig(TTree* tree) {
    std::vector<std::string>* volumeNames = nullptr;
    tree->SetBranchAddress("ScoringName", &volumeNames);
    tree->GetEntry(0);

    std::set<std::string> uniqueVolumeNames;
    int maxBarIndex = -1, maxCubeIndex = -1;

    for (const auto& name : *volumeNames) {
        uniqueVolumeNames.insert(name);
        int barIndex = -1, cubeIndex = -1;
        if (sscanf(name.c_str(), "LogicalCrystal_%d_%d", &barIndex, &cubeIndex) == 2) {
            maxBarIndex = std::max(maxBarIndex, barIndex);
            maxCubeIndex = std::max(maxCubeIndex, cubeIndex);
        }
    }

    return {maxBarIndex + 1, maxCubeIndex + 1};
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

HistogramData createHists(int numberBars, int numberCubes) {
    HistogramData data;
    data.histograms.resize(numberBars, std::vector<TH1D*>(numberCubes, nullptr));

    for (int barIndex = 0; barIndex < numberBars; barIndex++) {
        for (int cubeIndex = 0; cubeIndex < numberCubes; cubeIndex++) {
            std::string histName = "hist_" + std::to_string(barIndex) + "_" + std::to_string(cubeIndex);
            std::string histTitle = "Bar " + std::to_string(barIndex) + " Cube " + std::to_string(cubeIndex) + " Energy Deposition";

            data.histograms[barIndex][cubeIndex] = new TH1D(histName.c_str(), histTitle.c_str(), 100, 0, 1);
            data.histograms[barIndex][cubeIndex]->GetXaxis()->SetTitle("Energy (MeV)");
            data.histograms[barIndex][cubeIndex]->GetYaxis()->SetTitle("Counts");
        }
    }

    return data;
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

void populateHists(TTree* tree, HistogramData& data) {
    std::vector<std::string>* volumeNames = nullptr;
    std::vector<double>* edepValues = nullptr;
    tree->SetBranchAddress("ScoringName", &volumeNames);
    tree->SetBranchAddress("Energy", &edepValues);

    Long64_t nEntries = tree->GetEntries();
    for (Long64_t i = 0; i < nEntries; ++i) {
        tree->GetEntry(i);

        for (size_t j = 0; j < volumeNames->size(); ++j) {
            int barIndex = -1, cubeIndex = -1;

            if (sscanf(volumeNames->at(j).c_str(), "LogicalCrystal_%d_%d", &barIndex, &cubeIndex) == 2) {
                double edep = edepValues->at(j);
                if (edep > 0.0) {
                    data.histograms[barIndex][cubeIndex]->Fill(edep);
                }
            } else {
                std::cerr << "Error parsing volume name: " << volumeNames->at(j) << std::endl;
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

void drawHists(HistogramData& data, int numberBars, int numberCubes) {
    data.canvases.resize(numberBars);
    for (int barIndex = 0; barIndex < numberBars; barIndex++) {
        data.canvases[barIndex] = new TCanvas(("Canvas_" + std::to_string(barIndex)).c_str(), ("Bar " + std::to_string(barIndex)).c_str(), 800, 600);
        data.canvases[barIndex]->Divide(2, (numberCubes + 1) / 2);
        for (int cubeIndex = 0; cubeIndex < numberCubes; cubeIndex++) {
            data.canvases[barIndex]->cd(cubeIndex + 1);
            data.histograms[barIndex][cubeIndex]->Draw("HIST");
        }
    }
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

void saveHists(const HistogramData& data, const std::string& outputFilename) {
    TFile outputFile(outputFilename.c_str(), "RECREATE");
    for (const auto& histRow : data.histograms) {
        for (const auto& hist : histRow) {
            hist->Write();
        }
    }
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////


PrimaryConditionData readPrimaryConditions(TTree* tree) {
    PrimaryConditionData data;
    
    double beamEnergies;
    std::string* beamName = new std::string(); 
    std::tuple<double, double, double>* beamDirection = new std::tuple<double, double, double>(); // Initialize tuple pointers
    std::tuple<double, double, double>* sourcePosition = new std::tuple<double, double, double>(); // Initialize tuple pointers

    tree->SetBranchAddress("SourcePosition", &sourcePosition);
    tree->SetBranchAddress("BeamDirection", &beamDirection);
    tree->SetBranchAddress("BeamEnergy", &beamEnergies);
    tree->SetBranchAddress("BeamName", &beamName); // std::string

    Long64_t nEntries = tree->GetEntries(); // Get the number of entries in the tree
    for (Long64_t i = 0; i < nEntries; ++i) {
        tree->GetEntry(i); // Get the entry data for the current index
        
        data.beamEnergies.push_back(beamEnergies);
        data.beamNames.push_back(*beamName); // Dereference the string pointer
        
        data.sourcePositions.push_back(TVector3(std::get<0>(*sourcePosition), std::get<1>(*sourcePosition), std::get<2>(*sourcePosition)));
        data.beamDirections.push_back(TVector3(std::get<0>(*beamDirection), std::get<1>(*beamDirection), std::get<2>(*beamDirection)));
    }

    delete beamName;
    delete beamDirection;
    delete sourcePosition;

    return data; // Return the filled data structure
}

void printPrimaryConditions(const PrimaryConditionData& data) {
    std::cout << "Primary Conditions:\n";
    
    TH1D* energyHistogram = new TH1D("BeamEnergyHist", "Beam Energy Histogram;Energy (MeV);Counts", 100, 0, 10); // Adjust the range as needed
    TH1D* thetaHistogram = new TH1D("BeamThetaHist", "Beam Theta Histogram;Theta (degrees);Counts", 100, 0, 180);
    TH1D* phiHistogram = new TH1D("BeamPhiHist", "Beam Phi Histogram;Phi (degrees);Counts", 100, 0, 360);
    
    TH1D* sourceXHistogram = new TH1D("SourceXHist", "Source X Position Histogram;X (units);Counts", 100, -10, 10); // Adjust range as needed
    TH1D* sourceYHistogram = new TH1D("SourceYHist", "Source Y Position Histogram;Y (units);Counts", 100, -10, 10); // Adjust range as needed
    TH1D* sourceZHistogram = new TH1D("SourceZHist", "Source Z Position Histogram;Z (units);Counts", 100, -10, 10); // Adjust range as needed

    for (size_t i = 0; i < data.beamEnergies.size(); ++i) {
        // Fill energy histogram
        energyHistogram->Fill(data.beamEnergies[i]);
        
        // Calculate theta and phi from the beam direction
        const TVector3& beamDirection = data.beamDirections[i];
        double theta = beamDirection.Theta() * 180.0 / TMath::Pi(); // Convert to degrees
        double phi = beamDirection.Phi() * 180.0 / TMath::Pi();   // Convert to degrees
        
        // Fill theta and phi histograms
        thetaHistogram->Fill(theta);
        phiHistogram->Fill(phi);

        // Fill source position histograms
        const TVector3& sourcePosition = data.sourcePositions[i];
        sourceXHistogram->Fill(sourcePosition.X());
        sourceYHistogram->Fill(sourcePosition.Y());
        sourceZHistogram->Fill(sourcePosition.Z());
    }

    // Create a canvas and draw all histograms
    TCanvas* canvas = new TCanvas("cb", "Beam Energy Histogram", 800, 600);
    canvas->Divide(2, 3); // Divide the canvas into 2x3 grid to fit all histograms
    canvas->cd(1);
    energyHistogram->Draw();
    canvas->cd(2);
    thetaHistogram->Draw();
    canvas->cd(3);
    phiHistogram->Draw();
    canvas->cd(4);
    sourceXHistogram->Draw();
    canvas->cd(5);
    sourceYHistogram->Draw();
    canvas->cd(6);
    sourceZHistogram->Draw();
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

DetectorConditionData readDetectorConditions(TTree* tree) {
    DetectorConditionData data;

    std::tuple<double, double, double>* scoringLocationPtr = new std::tuple<double, double, double>(); // Initialize tuple pointers
    std::tuple<double, double, double>* scoringSizePtr = new std::tuple<double, double, double>(); // Initialize tuple pointers
    std::string* scoringNamePtr = new std::string(); 
    std::string* scoringMaterialPtr = new std::string(); 


    tree->SetBranchAddress("ScoringName", &scoringNamePtr);
    tree->SetBranchAddress("ScoringLocation", &scoringLocationPtr);
    tree->SetBranchAddress("ScoringMaterial", &scoringMaterialPtr);
    tree->SetBranchAddress("ScoringSize", &scoringSizePtr);

    Long64_t nEntries = tree->GetEntries(); // Get the number of entries in the tree
    
    for (Long64_t i = 0; i < nEntries; ++i) {
        tree->GetEntry(i); // Only need the first entry (index 0)

        data.scoringNames.push_back(   *scoringNamePtr );
        data.scoringMaterials.push_back(  *scoringMaterialPtr   );
        data.scoringLocations.push_back(TVector3(std::get<0>(*scoringLocationPtr), std::get<1>(*scoringLocationPtr), std::get<2>(*scoringLocationPtr)));
        data.scoringSizes.push_back(TVector3(std::get<0>(*scoringSizePtr), std::get<1>(*scoringSizePtr), std::get<2>(*scoringSizePtr)));

    }


    tree->GetEntry(0); // Only need the first entry (index 0)

    delete scoringNamePtr;
    delete scoringLocationPtr;
    delete scoringMaterialPtr;
    delete scoringSizePtr;

    return data;
}


void printDetectorConditions(const DetectorConditionData& data) {
    std::cout << "Detector Conditions:\n";

        for (size_t i = 0; i < data.scoringNames.size(); ++i) {
        std::cout << "Entry " << i + 1 << ":\n";
        std::cout << "  Scoring Name: " << data.scoringNames[i] << "\n";
        std::cout << "  Scoring Location: (" 
                  << data.scoringLocations[i].X() << ", " 
                  << data.scoringLocations[i].Y() << ", " 
                  << data.scoringLocations[i].Z() << ")\n";
        std::cout << "  Scoring Material: " << data.scoringMaterials[i] << "\n";
        std::cout << "  Scoring Size: (" 
                  << data.scoringSizes[i].X() << ", " 
                  << data.scoringSizes[i].Y() << ", " 
                  << data.scoringSizes[i].Z() << ")\n";
    }
}