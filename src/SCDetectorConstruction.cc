#include "SCDetectorConstruction.hh"

#include <cmath>
#include "globals.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4UnionSolid.hh"
#include "G4IntersectionSolid.hh"
#include "G4SubtractionSolid.hh"

#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4Isotope.hh"
#include "G4Element.hh"

#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4GenericMessenger.hh"

std::map<G4String, G4double> SCDetectorConstruction::m_hGeometryParameters;

SCDetectorConstruction::SCDetectorConstruction()
{
    ApplyMessengers();
}

SCDetectorConstruction::~SCDetectorConstruction()
{
}

void SCDetectorConstruction::ApplyMessengers()
{
    fMessengerCube = new G4GenericMessenger(
        this, 
        "/cube/", 
        "cube");

    fMessengerCube->DeclareProperty(
        "xCube", 
        xCube, 
        "xCube");

    fMessengerCube->DeclareProperty(
        "yCube", 
        yCube, 
        "yCube");

    fMessengerCube->DeclareProperty(
        "Cube_Width",
        Cube_Width, 
        "Cube_Width");

    fMessengerCube->DeclareProperty(
        "Cube_Length", 
        Cube_Length, 
        "Cube_Length");

    fMessengerCube->DeclareProperty(
        "Cube_Height", 
        Cube_Height, 
        "Cube_Height");

    fMessengerCube->DeclareProperty(
        "FiducialCube_Width", 
        FiducialCube_Width, 
        "FiducialCube_Width");

    fMessengerCube->DeclareProperty(
        "FiducialCube_Length", 
        FiducialCube_Length, 
        "FiducialCube_Length");

    fMessengerCube->DeclareProperty(
        "FiducialCube_Height", 
        FiducialCube_Height, 
        "FiducialCube_Height");

    fMessengerCube->DeclareProperty(
        "Li7_percent", 
        Li7_percent, 
        "Li7_percent");

    fMessengerCube->DeclareProperty(
        "Li6_percent", 
        Li6_percent, 
        "Li6_percent");

    // Default values for /cube/ messenger
    xCube = 70.71;
    yCube = 70.71;

    Cube_Width  = 10.;
    Cube_Length = 10.;
    Cube_Height = 10.;

    FiducialCube_Width  = 8.;
    FiducialCube_Length = 8.;
    FiducialCube_Height = 8.;

    // Natural abundance ~ Li7 (92.5%), Li6 (7.5%)
    Li7_percent = 92.5;
    Li6_percent = 7.5;

    fMessengerGeometry = new G4GenericMessenger(
        this, 
        "/geometry/", 
        "geometry");

    fMessengerGeometry->DeclareProperty(
        "build_moderators", 
        build_moderators, 
        "build_moderators");

    // Default values for /geometry/ messenger
    build_moderators = false;
}

G4double SCDetectorConstruction::GetGeometryParameter(const char *szParameter)
{
  if (m_hGeometryParameters.find(szParameter) != m_hGeometryParameters.end()){
    return m_hGeometryParameters[szParameter];
  }
  else {
    G4cout<< "Parameter: " << szParameter << " is not defined!!!!!" << G4endl;
    return 0;
  }
}

void SCDetectorConstruction::DefineGeometryParameters()
{
    //============================ Tolerances =================================
    m_hGeometryParameters["kTol"] = 0.1 *mm;

    //============================ Laboratory =================================
    m_hGeometryParameters["Lab_Width"]  = 4500. *mm;
    m_hGeometryParameters["Lab_Length"] = 4500. *mm;
    m_hGeometryParameters["Lab_Height"] = 4500. *mm;

    //=============================== Room ====================================
    m_hGeometryParameters["Room_Width"]     = 3960. *mm;
    m_hGeometryParameters["Room_Length"]    = 3960. *mm;
    m_hGeometryParameters["Room_Height"]    = 2470. *mm;
    m_hGeometryParameters["Room_Thickness"] =  400. *mm;

    //============================== Profile ==================================
    m_hGeometryParameters["Profile_Width"]     =  40. *mm;
    m_hGeometryParameters["Profile_Length"]    =  40. *mm;
    m_hGeometryParameters["Profile_Height"]    = 990. *mm;
    m_hGeometryParameters["Profile_Thickness"] =  10. *mm;

    //=============================== Wheel ===================================
    m_hGeometryParameters["Wheel_oD"]        = 300. *mm;
    m_hGeometryParameters["Wheel_iD"]        =   0. *mm;
    m_hGeometryParameters["Wheel_Thickness"] =  15. *mm;

    m_hGeometryParameters["Spoke_Length"]    =  40. *mm;

    //=============================== Paper ===================================
    m_hGeometryParameters["Paper_Thickness"] =   0.1*mm;

    //============================= Pb Cylinder ===============================
    m_hGeometryParameters["PbCylinder_oD"]            =  67. *mm;
    m_hGeometryParameters["PbCylinder_iD"]            =  57. *mm;
    m_hGeometryParameters["PbCylinder_Height"]        = 102. *mm;
    m_hGeometryParameters["PbCylinder_Bot_Thickness"] =  17. *mm;
    m_hGeometryParameters["PbCylinder_Cap_Thickness"] =  17. *mm;

    //============================== Moderator ================================
    m_hGeometryParameters["Moderator_Width"]  = 200. *mm;
    m_hGeometryParameters["Moderator_Length"] =  50. *mm;
    m_hGeometryParameters["Moderator_Height"] = 100. *mm;
}

void SCDetectorConstruction::DefineMaterials()
{
    G4NistManager *nist = G4NistManager::Instance();

    // --- Air ----------------------------------------------------------------
    Air = nist->FindOrBuildMaterial("G4_AIR");

    // --- Lithium Fluoride ---------------------------------------------------

    // Lithium Isotopes
    Li7 = new G4Isotope(
        "Li7",
        3,
        7,
        7.016 * g/mole
    );

    Li6 = new G4Isotope(
        "Li6",
        3,
        6,
        6.015 * g/mole
    );

    enLi = new G4Element(
        "enriched Li", 
        "enLi", 
        2
    );
    enLi->AddIsotope(Li7, Li7_percent * perCent);
    enLi->AddIsotope(Li6, Li6_percent * perCent);

    // Fluorine
    F19 = nist->FindOrBuildElement("F");

    // https://physics.nist.gov/cgi-bin/Star/compos.pl?matno=185
    G4double LiF_density = 2.635 * g/cm3;
    enLiF = new G4Material("enriched LiF", LiF_density, 2);

    enLiF->AddElement(enLi, 1);  // 1 Li atom
    enLiF->AddElement(F19,  1);  // 1 F atom

    // --- Concrete -----------------------------------------------------------
    Concrete = nist->FindOrBuildMaterial("G4_CONCRETE");

    // --- Al -----------------------------------------------------------------
    Al = nist->FindOrBuildMaterial("G4_Al");

    // --- Lead ---------------------------------------------------------------
    Pb = nist->FindOrBuildMaterial("G4_Pb");

    // --- Paper --------------------------------------------------------------
    G4Element* H = nist->FindOrBuildElement("H");
    G4Element* C = nist->FindOrBuildElement("C");
    G4Element* O = nist->FindOrBuildElement("O");

    G4double paper_density = 0.8 * g/cm3;

    Paper = new G4Material(
        "Paper",
        paper_density,
        3
    );

    // Cellulose formula: C6 H10 O5
    Paper->AddElement(C, 6);
    Paper->AddElement(H, 10);
    Paper->AddElement(O, 5);

    // --- Polyethelene -------------------------------------------------------
    Polyethelene = nist->FindOrBuildMaterial("G4_POLYETHYLENE");
}


// **************************************************************************//
// *** Start Construction ***************************************************//
// **************************************************************************//

G4VPhysicalVolume* SCDetectorConstruction::Construct()
{
    DefineMaterials();
    DefineGeometryParameters();
    ConstructLab();
    ConstructRoom();
    ConstructProfile();
    ConstructWheel();
    ConstructPbCylinder();
    ConstructCube();

    if (build_moderators==true)
    {
    ConstructModerators();
    }

    return phys_Lab;
}

void SCDetectorConstruction::ConstructLab()
{
    G4double Lab_Width  = GetGeometryParameter("Lab_Width");
    G4double Lab_Length = GetGeometryParameter("Lab_Length");
    G4double Lab_Height = GetGeometryParameter("Lab_Height");

    G4Box* solid_Lab = new G4Box(
        "solid_Lab", 
        Lab_Width/2, 
        Lab_Length/2, 
        Lab_Height/2
    );

    logic_Lab = new G4LogicalVolume(
        solid_Lab, 
        Air, 
        "logic_Lab"
    );

    auto vector_Lab = G4ThreeVector(
        0., 
        0., 
        0.);

    phys_Lab = new G4PVPlacement(
        0, 
        vector_Lab, 
        logic_Lab, 
        "phys_Lab", 
        nullptr, 
        false, 
        0, 
        checkOverlaps
    );
}

void SCDetectorConstruction::ConstructRoom()
{
    G4double Room_Width     = GetGeometryParameter("Room_Width");
    G4double Room_Length    = GetGeometryParameter("Room_Length");
    G4double Room_Thickness = GetGeometryParameter("Room_Thickness");

    G4double Lab_Height = GetGeometryParameter("Lab_Height");

    G4Box* solid_Room = new G4Box(
        "solid_Room1", 
        (Room_Width/2), 
        (Room_Length/2), 
        (Room_Thickness/2)
    );

    logic_Room = new G4LogicalVolume(
        solid_Room, 
        Concrete, 
        "logic_Room"
    );

    auto vector_Room = G4ThreeVector(
        0., 
        0., 
        -(Lab_Height/2) + (Room_Thickness/2));

    phys_Room = new G4PVPlacement(
        0, 
        vector_Room, 
        logic_Room, 
        "phys_Room", 
        logic_Lab, 
        false, 
        0, 
        checkOverlaps
    );

    /* Define special coordinate */
    zpos_RoomFloor = -(Lab_Height/2) + (Room_Thickness);

    /* -- VisAttributes ---------------------------------------------------- */
    auto colour_Room = G4Colour(0.5, 0.5, 0.5, 0.9);
    G4VisAttributes *vis_Room = new G4VisAttributes(colour_Room);
    vis_Room   ->SetVisibility(true);
    vis_Room   ->SetForceSolid(true);
    vis_Room   ->SetForceAuxEdgeVisible(true);
    logic_Room ->SetVisAttributes(vis_Room);
}

void SCDetectorConstruction::ConstructProfile()
{
    G4double Profile_Width     = GetGeometryParameter("Profile_Width");
    G4double Profile_Length    = GetGeometryParameter("Profile_Length");
    G4double Profile_Height    = GetGeometryParameter("Profile_Height");
    G4double Profile_Thickness = GetGeometryParameter("Profile_Thickness");

    G4Box* solid_Profile1 = new G4Box(
        "solid_Profile1", 
        Profile_Width/2, 
        Profile_Length/2, 
        Profile_Height/2
    );

    G4Box* solid_Profile2 = new G4Box(
        "solid_Profile2", 
        (Profile_Width/2)  - Profile_Thickness, 
        (Profile_Length/2) - Profile_Thickness, 
        Profile_Height/2
    );

    auto sub_vector_Profile1_Profile2 = G4ThreeVector(
        0., 
        0., 
        0.);

    G4SubtractionSolid* solid_Profile = new G4SubtractionSolid(
        "solid_Profile", 
        solid_Profile1, 
        solid_Profile2,
        0,
        sub_vector_Profile1_Profile2
    );

    logic_Profile = new G4LogicalVolume(
        solid_Profile, 
        Al, 
        "logic_Profile"
    );

    G4double zpos_Profile_centre = zpos_RoomFloor + Profile_Height/2;
    auto vector_Profile = G4ThreeVector(
        0., 
        0., 
        zpos_Profile_centre);

    phys_Profile = new G4PVPlacement(
        0, 
        vector_Profile, 
        logic_Profile, 
        "phys_Profile", 
        logic_Lab, 
        false, 
        0, 
        checkOverlaps
    );

    /* Define special coordinate */
    zpos_ProfileTop = zpos_RoomFloor + Profile_Height;

    /* -- VisAttributes ---------------------------------------------------- */
    auto colour_Profile = G4Colour(0.9, 0.9, 0.9, 1);
    G4VisAttributes *vis_Profile = new G4VisAttributes(colour_Profile);
    vis_Profile   ->SetVisibility(true);
    vis_Profile   ->SetForceSolid(true);
    vis_Profile   ->SetForceAuxEdgeVisible(true);
    logic_Profile ->SetVisAttributes(vis_Profile);

}

void SCDetectorConstruction::ConstructWheel()
{
    G4double Wheel_oD        = GetGeometryParameter("Wheel_oD");
    G4double Wheel_iD        = GetGeometryParameter("Wheel_iD");
    G4double Wheel_Thickness = GetGeometryParameter("Wheel_Thickness");

    G4Tubs* solid_Wheel = new G4Tubs(
        "solid_Rim",
        Wheel_iD/2,
        Wheel_oD/2,
        Wheel_Thickness/2,
        0.  *deg,
        360.*deg
    );

    logic_Wheel = new G4LogicalVolume(
        solid_Wheel, 
        Polyethelene, 
        "logic_Wheel"
    );

    G4double zpos_Wheel_centre = zpos_ProfileTop + Wheel_Thickness/2;
    auto vector_Wheel = G4ThreeVector(
        0., 
        0., 
        zpos_Wheel_centre);

    phys_Wheel = new G4PVPlacement(
        0, 
        vector_Wheel, 
        logic_Wheel, 
        "phys_Wheel", 
        logic_Lab, 
        false, 
        0, 
        checkOverlaps
    );

    /* Define special coordinate */
    zpos_WheelTop = zpos_Wheel_centre + Wheel_Thickness/2;

    /* -- VisAttributes ---------------------------------------------------- */
    auto colour_Wheel = G4Colour(1., 1., 1., 1);
    G4VisAttributes *vis_Wheel = new G4VisAttributes(colour_Wheel);
    vis_Wheel   ->SetVisibility(true);
    vis_Wheel   ->SetForceSolid(true);
    vis_Wheel   ->SetForceAuxEdgeVisible(true);
    logic_Wheel ->SetVisAttributes(vis_Wheel);
}

void SCDetectorConstruction::ConstructPbCylinder()
{
    G4double PbCylinder_oD     = GetGeometryParameter("PbCylinder_oD");
    G4double PbCylinder_iD     = GetGeometryParameter("PbCylinder_iD");
    G4double PbCylinder_Height = GetGeometryParameter("PbCylinder_Height");
    G4double PbCylinder_Bot_Thickness = GetGeometryParameter("PbCylinder_Bot_Thickness");
    G4double PbCylinder_Cap_Thickness = GetGeometryParameter("PbCylinder_Cap_Thickness");

    G4double solid_Cylinder1_Height = PbCylinder_Height + PbCylinder_Cap_Thickness;
    G4double solid_Cylinder2_Height = solid_Cylinder1_Height - (PbCylinder_Bot_Thickness + PbCylinder_Cap_Thickness);
    G4double zoffset_Cylinder2 = -(PbCylinder_Bot_Thickness + PbCylinder_Cap_Thickness)/2 + PbCylinder_Bot_Thickness;

    G4Tubs* solid_Cylinder1 = new G4Tubs(
        "solid_Cylinder1",
        0.,
        PbCylinder_oD/2,
        solid_Cylinder1_Height/2,
        0.  *deg,
        360.*deg
    );

    G4Tubs* solid_Cylinder2 = new G4Tubs(
        "solid_Cylinder2",
        0.,
        PbCylinder_iD/2,
        solid_Cylinder2_Height/2,
        0.  *deg,
        360.*deg
    );

    auto sub_vector_PbCylinder = G4ThreeVector(
        0., 
        0., 
        zoffset_Cylinder2);

    G4SubtractionSolid* solid_PbCylinder = new G4SubtractionSolid(
        "solid_PbCylinder", 
        solid_Cylinder1, 
        solid_Cylinder2,
        0,
        sub_vector_PbCylinder
    );

    logic_PbCylinder = new G4LogicalVolume(
        solid_PbCylinder, 
        Pb, 
        "logic_PbCylinder"
    );

    auto vector_PbCylinder = G4ThreeVector(
        0., 
        0., 
        zpos_WheelTop + solid_Cylinder1_Height/2);

    phys_PbCylinder = new G4PVPlacement(
        0, 
        vector_PbCylinder, 
        logic_PbCylinder, 
        "phys_PbCylinder", 
        logic_Lab, 
        false, 
        0, 
        checkOverlaps
    );

    /* Define special coordinate */
    zpos_PbCylinder_Inside_Floor = zpos_WheelTop + PbCylinder_Bot_Thickness;

    /* -- VisAttributes ---------------------------------------------------- */
    auto colour_PbCylinder = G4Colour(1., 0., 0., 0.7);
    G4VisAttributes *vis_PbCylinder = new G4VisAttributes(colour_PbCylinder);
    vis_PbCylinder   ->SetVisibility(true);
    vis_PbCylinder   ->SetForceSolid(true);
    vis_PbCylinder   ->SetForceAuxEdgeVisible(true);
    logic_PbCylinder ->SetVisAttributes(vis_PbCylinder);
}

void SCDetectorConstruction::ConstructCube()
{
    G4Box* solid_Cube = new G4Box(
        "solid_Cube", 
        Cube_Width/2, 
        Cube_Length/2, 
        Cube_Height/2
    );

    logic_Cube = new G4LogicalVolume(
        solid_Cube, 
        enLiF, 
        "logic_Cube"
    );

    auto rotZ45 = new G4RotationMatrix();
    rotZ45->rotateZ(45.*deg);

    auto vector_Cube = G4ThreeVector(
        xCube, 
        yCube, 
        zpos_WheelTop+Cube_Height/2);

    phys_Cube = new G4PVPlacement(
        rotZ45, 
        vector_Cube, 
        logic_Cube, 
        "phys_Cube", 
        logic_Lab, 
        false, 
        0, 
        checkOverlaps
    );

    /* VisAttributes */
    auto colour_Cube = G4Colour(0, 1., 1., 0.25);
    G4VisAttributes *vis_Cube = new G4VisAttributes(colour_Cube);
    vis_Cube   ->SetVisibility(true);
    vis_Cube   ->SetForceSolid(true);
    vis_Cube   ->SetForceAuxEdgeVisible(true);
    logic_Cube ->SetVisAttributes(vis_Cube);

    G4Box* solid_FiducialCube = new G4Box(
        "solid_FiducialCube", 
        FiducialCube_Width/2, 
        FiducialCube_Length/2, 
        FiducialCube_Height/2
    );

    logic_FiducialCube = new G4LogicalVolume(
        solid_FiducialCube, 
        enLiF, 
        "logic_FiducialCube"
    );

    auto vector_FiducialCube = G4ThreeVector(
        0., 
        0., 
        0.);

    phys_FiducialCube = new G4PVPlacement(
        0, 
        vector_FiducialCube, 
        logic_FiducialCube, 
        "phys_FiducialCube", 
        logic_Cube, 
        false, 
        0, 
        checkOverlaps
    );

    /* VisAttributes */
    auto colour_FiducialCube = G4Colour(Li7_percent/100, 0., Li6_percent/100);
    G4VisAttributes *vis_FiducialCube = new G4VisAttributes(colour_FiducialCube);
    vis_FiducialCube   ->SetVisibility(true);
    vis_FiducialCube   ->SetForceSolid(true);
    vis_FiducialCube   ->SetForceAuxEdgeVisible(true);
    logic_FiducialCube ->SetVisAttributes(vis_FiducialCube);
}

void SCDetectorConstruction::ConstructModerators()
{
    G4double Moderator_Width  = GetGeometryParameter("Moderator_Width");
    G4double Moderator_Length = GetGeometryParameter("Moderator_Length");
    G4double Moderator_Height = GetGeometryParameter("Moderator_Height");

    G4double PbCylinder_oD = GetGeometryParameter("PbCylinder_oD");

    G4Box* solid_Moderator = new G4Box(
        "solid_Moderator", 
        Moderator_Width/2, 
        Moderator_Length/2, 
        Moderator_Height/2
    );

    logic_Moderator = new G4LogicalVolume(
        solid_Moderator, 
        Polyethelene, 
        "logic_Moderator"
    );

    auto rotZ45 = new G4RotationMatrix();
    rotZ45->rotateZ(45.*deg);

    auto vector_Moderator1 = G4ThreeVector(
        ((PbCylinder_oD/2) + (Moderator_Length/2) + 1)/std::sqrt(2.0), 
        ((PbCylinder_oD/2) + (Moderator_Length/2) + 1)/std::sqrt(2.0),  
        zpos_WheelTop+Moderator_Height/2);

    phys_Moderator1 = new G4PVPlacement(
        rotZ45, 
        vector_Moderator1, 
        logic_Moderator, 
        "phys_Moderator1", 
        logic_Lab, 
        false, 
        0, 
        checkOverlaps
    );

    auto vector_Moderator2 = G4ThreeVector(
        -((PbCylinder_oD/2) + (Moderator_Length/2) + 1)/std::sqrt(2.0), 
        -((PbCylinder_oD/2) + (Moderator_Length/2) + 1)/std::sqrt(2.0),  
        zpos_WheelTop+Moderator_Height/2);

    phys_Moderator2 = new G4PVPlacement(
        rotZ45, 
        vector_Moderator2, 
        logic_Moderator, 
        "phys_Moderator2", 
        logic_Lab, 
        false, 
        0, 
        checkOverlaps
    );

    /* VisAttributes */
    auto colour_Moderator = G4Colour(1., 1., 1., 1.);
    G4VisAttributes *vis_Moderator = new G4VisAttributes(colour_Moderator);
    vis_Moderator   ->SetVisibility(true);
    vis_Moderator   ->SetForceSolid(true);
    vis_Moderator   ->SetForceAuxEdgeVisible(true);
    logic_Moderator ->SetVisAttributes(vis_Moderator);
}

void SCDetectorConstruction::ConstructSDandField()
{
    SCSensitiveDetector *sensDet = new SCSensitiveDetector("SensitveDetector");
    logic_FiducialCube->SetSensitiveDetector(sensDet);
}