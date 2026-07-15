#ifndef SCACTIONINITIALIZATION_HH
#define SCACTIONINITIALIZATION_HH

#include "G4VUserActionInitialization.hh"
#include "globals.hh"

#include "SCPrimaryGenerator.hh"
#include "SCRunAction.hh"

class SCDetectorConstruction;

class SCActionInitialization : public G4VUserActionInitialization
{
public:
    SCActionInitialization(
        const SCDetectorConstruction* detector,
        const G4String& outputFilePath = "output.root"
    );

    ~SCActionInitialization();

    void Build() const override;

private:
    const SCDetectorConstruction* fDetector = nullptr;
    G4String fOutputFilePath;
};

#endif