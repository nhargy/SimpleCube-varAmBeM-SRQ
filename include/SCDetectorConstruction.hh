#ifndef SCDETECTORCONSTRUCTION_HH   
#define SCDETECTORCONSTRUCTION_HH 1

// Includes
#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"
#include "SCSensitiveDetector.hh"
#include <map>

// Forward declarations
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Element;
class G4Isotope;
class G4Material;
class G4GenericMessenger;

class SCDetectorConstruction : public G4VUserDetectorConstruction
{
public:
    SCDetectorConstruction();
    virtual ~SCDetectorConstruction();

    virtual G4VPhysicalVolume* Construct();

    G4double GetGeometryParameter(const char *szParameter);

    // position of source inside PbCylinder
    G4double GetPbCylinderInsideFloor() const { return zpos_PbCylinder_Inside_Floor; }

private:
    void DefineMaterials();
    void ConstructLab();
    void ConstructRoom();
    void ConstructCube();
    void ConstructProfile();
    void ConstructWheel();
    void ConstructPbCylinder();
    void ConstructModerators();

    virtual void ConstructSDandField();

    void ApplyMessengers();
    void DefineGeometryParameters();

    static std::map<G4String, G4double> m_hGeometryParameters;

    G4bool checkOverlaps = true;

    /* Lab */
    G4LogicalVolume*   logic_Lab = nullptr;
    G4VPhysicalVolume* phys_Lab  = nullptr;

    /* Cube */
    G4LogicalVolume*   logic_Cube = nullptr;
    G4VPhysicalVolume* phys_Cube  = nullptr;

    /* Fiducial Cube */
    G4LogicalVolume*   logic_FiducialCube = nullptr;
    G4VPhysicalVolume* phys_FiducialCube  = nullptr;

    /* Wheel */
    G4LogicalVolume*   logic_Wheel = nullptr;
    G4VPhysicalVolume* phys_Wheel  = nullptr;

    /* Paper */
    G4LogicalVolume*   logic_Paper = nullptr;
    G4VPhysicalVolume* phys_Paper  = nullptr;

    /* Profile */
    G4LogicalVolume*   logic_Profile = nullptr;
    G4VPhysicalVolume* phys_Profile  = nullptr;

    /* PbCylinder */
    G4LogicalVolume*   logic_PbCylinder = nullptr;
    G4VPhysicalVolume* phys_PbCylinder  = nullptr;

    /* Room */
    G4LogicalVolume*   logic_Room = nullptr;
    G4VPhysicalVolume* phys_Room  = nullptr;

    /* Moderators */
    G4LogicalVolume*   logic_Moderator = nullptr;
    G4VPhysicalVolume* phys_Moderator1  = nullptr;
    G4VPhysicalVolume* phys_Moderator2  = nullptr;

    /* == Special Coordinates  ==*/
    G4double zpos_RoomFloor  = 0.;
    G4double zpos_ProfileTop = 0.;
    G4double zpos_WheelTop   = 0.;
    G4double zpos_PaperTop   = 0.;
    G4double zpos_PbCylinder_Inside_Floor = 0.;

    // Material definitions
    G4Material* Air      = nullptr;
    G4Material* LiF      = nullptr;
    G4Material* enLiF    = nullptr;
    G4Material* PLA      = nullptr;
    G4Material* Concrete = nullptr;
    G4Material* Al       = nullptr;
    G4Material* Pb       = nullptr;
    G4Material* Paper    = nullptr;
    G4Material* Polyethelene = nullptr;

    // Isotop definitions
    G4Isotope* Li7  = nullptr;
    G4Isotope* Li6  = nullptr;
    G4Element* enLi = nullptr;
    G4Element* F19  = nullptr;

    // Messengers
    G4GenericMessenger* fMessengerCube     = nullptr;
    G4GenericMessenger* fMessengerGeometry = nullptr;

    G4double xCube = 0.;
    G4double yCube = 0.;

    G4double Cube_Width  = 0.;
    G4double Cube_Length = 0.;
    G4double Cube_Height = 0.;

    G4double FiducialCube_Width  = 0.;
    G4double FiducialCube_Length = 0.;
    G4double FiducialCube_Height = 0.;

    G4double Li7_percent = 0.;
    G4double Li6_percent = 0.;

    G4bool build_moderators = false;
};

#endif
