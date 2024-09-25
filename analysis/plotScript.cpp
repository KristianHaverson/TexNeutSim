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

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

TTree* openInputFile(const std::string& filename, const std::string& key);
std::pair<int, int> getSimConfig(TTree* tree);
HistogramData createHists(int numberBars, int numberCubes);
void populateHists(TTree* tree, HistogramData& data);
void drawHists(HistogramData& data, int numberBars, int numberCubes);
void saveHists(const HistogramData& data, const std::string& outputFilename);


///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

void plotScript() {
    TTree* tree = openInputFile("build/output.root", "event");
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
    tree->SetBranchAddress("VolumeNames", &volumeNames);
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
    tree->SetBranchAddress("VolumeNames", &volumeNames);
    tree->SetBranchAddress("EdepValues", &edepValues);

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
