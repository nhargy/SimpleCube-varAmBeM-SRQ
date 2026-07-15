#include "SCActionInitialization.hh"

SCActionInitialization::SCActionInitialization(
    const SCDetectorConstruction* detector,
    const G4String& outputFilePath
)
: fDetector(detector),
  fOutputFilePath(outputFilePath)
{}

SCActionInitialization::~SCActionInitialization()
{}

void SCActionInitialization::Build() const
{
    SCPrimaryGenerator *generator = new SCPrimaryGenerator(fDetector);
    SetUserAction(generator);

    SCRunAction *runAction = new SCRunAction();
    runAction->SetOutputDirectory(fOutputFilePath);
    SetUserAction(runAction);
}