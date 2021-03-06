/*
 * DetectorConstruction.cpp
 *
 *  Created on: Oct 2, 2018
 *      Author: vsevolod
 */

#include "DetectorConstruction.h"


DetectorConstruction::DetectorConstruction() {
    //    DefineMateials();

}

DetectorConstruction::~DetectorConstruction() {

}

G4VPhysicalVolume* DetectorConstruction::Construct() {
    LSD = new SensitiveDetector("LSD", _eventAction);
    G4SDManager::GetSDMpointer()->AddNewDetector(LSD);
    DefineMateials();

    return DefineVolumes();
}

void DetectorConstruction::ConstructSDandField() {


    //    G4cout << "_____________________________________________Detectors are made" << G4endl;
}
// Defining materials
void DetectorConstruction::DefineMateials() {
    G4String symbol;
    G4double a, z, density;
    G4int ncomponents, natoms;
    G4double fractionmass;

    G4NistManager* man = G4NistManager::Instance();

    G4Element* C =
            new G4Element("Carbon", symbol = "C", z = 6., a = 12.01*g/mole);
    G4Element* N =
            new G4Element("Nitrogen", symbol = "N", z = 7., a = 14.01*g/mole);
    G4Element* O =
            new G4Element("Oxygen", symbol = "O", z = 8., a = 16.00*g/mole);
    G4Element* Si =
            new G4Element("Silicon", symbol = "Si", z = 14., a = 28.09*g/mole);
    G4Element* H =
            new G4Element("Hydrogen", symbol = "H", z = 1., a = 1.008*g/mole);
    Air =
            new G4Material("Air", density = 0.000290*mg/cm3, ncomponents = 2);
    Air->AddElement(N, fractionmass = 0.7);
    Air->AddElement(O, fractionmass = 0.3);
    worldMaterial = Air;

    Acrylic =
            new G4Material("Acrylic", density = 1.18*g/cm3, ncomponents = 3);
    Acrylic->AddElement(C, natoms = 5);
    Acrylic->AddElement(O, natoms = 2);
    Acrylic->AddElement(H, natoms = 8);

    Water =
            new G4Material("Water", density = 0.9982*g/cm3, ncomponents = 2);
    Water->AddElement(H, natoms = 2);
    Water->AddElement(O, natoms = 1);


    G4Material* Scint = man->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
    ScintMaterial = Scint;

    INOX = man->FindOrBuildMaterial("G4_STAINLESS-STEEL");

    Beryllium = man->FindOrBuildMaterial("G4_Be");
    Copper = man->FindOrBuildMaterial("G4_Cu");

    BPMaterial = Copper;
    //    BPMaterial = Beryllium;


    Vacuum = new G4Material( "Galactic", z=1., a=1.0*g/mole, density= universe_mean_density,
                             kStateGas, 2.73*kelvin, 3.e-18*pascal );

    // Quartz Material (SiO2)
    SiO2 =
            new G4Material("quartz", density = 2.200*g/cm3, ncomponents = 2);
    SiO2->AddElement(Si, natoms = 1);
    SiO2->AddElement(O , natoms = 2);




    //
    // Generate and Add Material Properties Table
    //
    const G4int num = 36;
    //    const G4int num = 50;
    G4double WaveLength[num];
    G4double Absorption[num];      // Default value for absorption
    G4double AirAbsorption[num];
    G4double AirRefractiveIndex[num];
    G4double PhotonEnergy[num];

    //     Absorption of quartz per 1m
    G4double QuartzAbsorption[num] =
    {0.999572036,0.999544661,0.999515062,0.999483019,0.999448285,
     0.999410586,0.999369611,0.999325013,0.999276402,0.999223336,
     0.999165317,0.999101778,0.999032079,0.998955488,0.998871172,
     0.998778177,0.99867541 ,0.998561611,0.998435332,0.998294892,
     0.998138345,0.997963425,0.997767484,0.997547418,0.99729958 ,
     0.99701966 ,0.99670255 ,0.996342167,0.995931242,0.995461041,
     0.994921022,0.994298396,0.993577567,0.992739402,0.991760297,
     0.990610945};
//    G4double WaterAbsorption[num] =
//    {0.142087,0.106061,0.0834869,0.0683827,0.0552737,
//     0.0452757,0.0391016,0.0295795,0.0223762,0.0188202,
//     0.0169115,0.0160251,0.0149865,0.0142009,0.0143828,
//     0.0147581,0.0149400,0.0151273,0.0170301,0.0194281,
//     0.0253021,0.0342960,0.0458629,0.0502813,0.0536883 ,
//     0.0588657 ,0.0645651 ,0.0653863,0.0807698,0.0921587,
//     0.108020,0.146411,0.235745,0.287300,0.306713,
//     0.310492};

    for (int i=0; i<num; i++) {
        //        WaveLength[i] = (300 + i*10)*nanometer;
        WaveLength[i] = (300 + i*10)*nanometer;
        Absorption[i] = 100*m;      // Fake number for no absorption
        AirAbsorption[i] = 1.*cm;   // If photon hits air, kill it
        AirRefractiveIndex[i] = 1.; // Rough air refraction
        PhotonEnergy[num - (i+1)] = twopi*hbarc/WaveLength[i];
        /* Absorption is given per length and G4 needs mean free path
         length, calculate it here
         mean free path length - taken as probablility equal 1/e
         that the photon will be absorbed */

//        WaterAbsorption[i] = (-1)/log(WaterAbsorption[i])*100*cm;
        QuartzAbsorption[i] = (-1)/log(QuartzAbsorption[i])*100*cm;
        //        QuartzAbsorption[i] = 10.*cm;

        //EpotekAbsorption[i] = (-1)/log(EpotekAbsorption[i])*
        //epotekBarJoint.thickness;
    }

    G4double QuartzRefractiveIndex[num] =
    {1.456535,1.456812,1.4571  ,1.457399,1.457712,1.458038,
     1.458378,1.458735,1.459108,1.4595  ,1.459911,1.460344,
     1.460799,1.46128 ,1.461789,1.462326,1.462897,1.463502,
     1.464146,1.464833,1.465566,1.46635 ,1.46719 ,1.468094,
     1.469066,1.470116,1.471252,1.472485,1.473826,1.475289,
     1.476891,1.478651,1.480592,1.482739,1.485127,1.487793};

    G4double WaterRefractiveIndex[num] =
    {1.331124, 1.331390, 1.331665, 1.331950, 1.332247, 1.332555,
     1.332875, 1.333210, 1.333559, 1.333923, 1.334305, 1.334705,
     1.335126, 1.335568, 1.336033, 1.336525, 1.337044, 1.337593,
     1.338176, 1.338795, 1.339453, 1.340156, 1.340907, 1.341711,
     1.342574, 1.343502, 1.344503, 1.345585, 1.346758, 1.348033,
     1.349423, 1.350943, 1.352610, 1.354446, 1.356476, 1.358730};

    G4double AcrylicRefractiveIndex[num] =
    {1.489915, 1.490240, 1.490578, 1.490933, 1.491305, 1.491694,
     1.492103, 1.492533, 1.492985, 1.493461, 1.493963, 1.494492,
     1.495052, 1.495644, 1.496272, 1.496938, 1.497645, 1.498398,
     1.499201, 1.500057, 1.500973, 1.501954, 1.503007, 1.504139,
     1.505359, 1.506676, 1.508102, 1.509649, 1.511331, 1.513167,
     1.515175, 1.517379, 1.519807, 1.522490, 1.525467, 1.528786};


    // Assign absorption and refraction to materials

    // Quartz
    G4MaterialPropertiesTable* QuartzMPT = new G4MaterialPropertiesTable();
    QuartzMPT->AddProperty("RINDEX", PhotonEnergy, QuartzRefractiveIndex, num);
    QuartzMPT->AddProperty("ABSLENGTH", PhotonEnergy, QuartzAbsorption, num);

    SiO2->SetMaterialPropertiesTable(QuartzMPT);

    G4MaterialPropertiesTable* WaterMPT = new G4MaterialPropertiesTable();
    WaterMPT->AddProperty("RINDEX", PhotonEnergy, WaterRefractiveIndex, num);
    WaterMPT->AddProperty("ABSLENGTH", PhotonEnergy, Absorption, num);
    Water->SetMaterialPropertiesTable(WaterMPT);

    G4MaterialPropertiesTable* AcrylicMPT = new G4MaterialPropertiesTable();
    AcrylicMPT->AddProperty("RINDEX", PhotonEnergy, AcrylicRefractiveIndex, num);
    AcrylicMPT->AddProperty("ABSLENGTH", PhotonEnergy, Absorption, num);
    Acrylic->SetMaterialPropertiesTable(AcrylicMPT);

    // Air
    G4MaterialPropertiesTable* AirMPT = new G4MaterialPropertiesTable();
    AirMPT->AddProperty("RINDEX", PhotonEnergy, AirRefractiveIndex, num);
    AirMPT->AddProperty("ABSLENGTH", PhotonEnergy, AirAbsorption, num);

    // Assign these properties to the world volume
    Air->SetMaterialPropertiesTable(AirMPT);
    // In our rough assumption
    Vacuum->SetMaterialPropertiesTable(AirMPT);
}

G4VPhysicalVolume* DetectorConstruction::DefineVolumes(){

    //////////////////////// World /////////////////////////////
    G4VSolid *worldSolid = new G4Box("World",
                                     Const::worldSizeX/2.,
                                     Const::worldSizeY/2.,
                                     Const::worldSizeZ/2.);

    worldLogical = new G4LogicalVolume(worldSolid,
                                       Vacuum /*Air*/, // worldMaterial,
                                       "World");

    G4VPhysicalVolume *worldPhysical = new G4PVPlacement(0,
                                                         G4ThreeVector(),
                                                         worldLogical,
                                                         "World",
                                                         0,
                                                         false,
                                                         0);
    ///////////////////////////////////////////////////////////


    G4VSolid *waterBox = new G4Box("waterBox",
                                   Const::waterBoxSizeX /2.,
                                   Const::waterBoxSizeY /2.,
                                   Const::waterBoxSizeZ /2.);
    G4LogicalVolume *waterBoxLogical = new G4LogicalVolume(waterBox,
                                                           Water,
                                                           "waterBox");
    new G4PVPlacement(0,
                      G4ThreeVector(0,0,1*um),
                      waterBoxLogical,
                      "waterBox",
                      worldLogical,
                      false,
                      0);



    G4VSolid *acrylicBox = new G4Box("acrylicBox",
                                     Const::acrylicBoxSizeX /2.,
                                     Const::acrylicBoxSizeY /2.,
                                     Const::acrylicBoxSizeZ /2.);
    acrylicBoxLogical = new G4LogicalVolume(acrylicBox,
                                            Acrylic,
                                            "acrylicBox");
    new G4PVPlacement(0,
                      G4ThreeVector(0,0, Const::waterBoxSizeZ /2. + Const::acrylicBoxSizeZ /2.),
                      acrylicBoxLogical,
                      "acrylicBox",
                      worldLogical,
                      false,
                      0);


    G4VSolid *acrylicCup = new G4Tubs("acrylicCup1",
                                      Const::pmtRadius,
                                      Const::pmtRadius + 1*mm,
                                      (Const::pmtRadius + 1*mm)/2,
                                      0,
                                      twopi);

    G4LogicalVolume *acrylicCupLogical1 = new G4LogicalVolume(acrylicCup,
                                                              Acrylic,
                                                              "acrylicCup1");

    G4LogicalVolume *acrylicCupLogical2 = new G4LogicalVolume(acrylicCup,
                                                              Acrylic,
                                                              "acrylicCup2");



    new G4PVPlacement(0,
                      G4ThreeVector(0, - Const::pmtRadius*1.5,
                                    Const::waterBoxSizeZ /2. + Const::acrylicBoxSizeZ + (Const::pmtRadius + 1*mm)/2.),
                      acrylicCupLogical1,
                      "acrylicCup",
                      worldLogical,
                      false,
                      0);

    new G4PVPlacement(0,
                      G4ThreeVector(0, + Const::pmtRadius*1.5,
                                    Const::waterBoxSizeZ /2. + Const::acrylicBoxSizeZ + (Const::pmtRadius + 1*mm)/2.),
                      acrylicCupLogical2,
                      "acrylicCup",
                      worldLogical,
                      false,
                      0);

    G4VSolid *pmtSphereSolid = new G4Sphere("pmtSphere",
                                            0.,
                                            Const::pmtRadius + 1.*mm,
                                            0,
                                            twopi,
                                            pi/2,
                                            pi/2);

    G4VSolid *acrylicCupWater_raw = new G4Tubs("acrylicCupWater_raw",
                                               0,
                                               Const::pmtRadius,
                                               (Const::pmtRadius + 1*mm)/2,
                                               0,
                                               twopi);

    Ra = new G4RotationMatrix();
    Ta = G4ThreeVector(0,0, (Const::pmtRadius + 1*mm)/2.);

    G4VSolid *acrylicCupWater = new G4SubtractionSolid("acrylicCupWater",
                                                       acrylicCupWater_raw,
                                                       pmtSphereSolid,
                                                       Ra,
                                                       Ta);



    acrylicCupWaterLogical1 = new G4LogicalVolume(acrylicCupWater,
                                                  Water,
                                                  "acrylicCupWater1");

    acrylicCupWaterLogical2 = new G4LogicalVolume(acrylicCupWater,
                                                  Water,
                                                  "acrylicCupWater2");

    acrylicCupWaterLogical1->SetSensitiveDetector(LSD);
    acrylicCupWaterLogical2->SetSensitiveDetector(LSD);


    new G4PVPlacement(0,
                      G4ThreeVector(0, - Const::pmtRadius*1.5,
                                    Const::waterBoxSizeZ /2. + Const::acrylicBoxSizeZ + (Const::pmtRadius + 1*mm)/2.),
                      acrylicCupWaterLogical1,
                      "acrylicCup",
                      worldLogical,
                      false,
                      0);

    new G4PVPlacement(0,
                      G4ThreeVector(0, + Const::pmtRadius*1.5,
                                    Const::waterBoxSizeZ /2. + Const::acrylicBoxSizeZ + (Const::pmtRadius + 1*mm)/2.),
                      acrylicCupWaterLogical2,
                      "acrylicCup",
                      worldLogical,
                      false,
                      0);


    G4VSolid *pmtSphere = new G4Sphere("pmtSphere",
                                       Const::pmtRadius,
                                       Const::pmtRadius + 1.*mm,
                                       0,
                                       twopi,
                                       pi/2,
                                       pi/2);


    pmtSphereLogical1 = new G4LogicalVolume(pmtSphere,
                                            SiO2,
                                            "pmtSphere1");
    pmtSphereLogical2 = new G4LogicalVolume(pmtSphere,
                                            SiO2,
                                            "pmtSphere2");


    new G4PVPlacement(0,
                      G4ThreeVector(0, - Const::pmtRadius*1.5,
                                    Const::waterBoxSizeZ /2. + Const::acrylicBoxSizeZ + (Const::pmtRadius + 1*mm)),
                      pmtSphereLogical1,
                      "acrylicCup",
                      worldLogical,
                      false,
                      0);

    new G4PVPlacement(0,
                      G4ThreeVector(0, + Const::pmtRadius*1.5,
                                    Const::waterBoxSizeZ /2. + Const::acrylicBoxSizeZ + (Const::pmtRadius + 1*mm)),
                      pmtSphereLogical2,
                      "acrylicCup",
                      worldLogical,
                      false,
                      0);

    G4VSolid *acrylicWallX = new G4Box("acrylicWallX",
                                       Const::acrylicWallThickness/2.,
                                       Const::waterBoxSizeY/2. + Const::acrylicWallThickness,
                                       Const::waterBoxSizeZ/2. + Const::acrylicWallThickness);
    G4VSolid *acrylicWallY = new G4Box("acrylicWallY",
                                       Const::waterBoxSizeX/2. + Const::acrylicWallThickness,
                                       Const::acrylicWallThickness/2.,
                                       Const::waterBoxSizeZ/2. + Const::acrylicWallThickness);
    G4VSolid *acrylicWallZ = new G4Box("acrylicWallZ",
                                       Const::waterBoxSizeX/2. + Const::acrylicWallThickness,
                                       Const::waterBoxSizeY/2. + Const::acrylicWallThickness,
                                       Const::acrylicWallThickness/2.);

    G4VSolid *mirrorX = new G4Box("mirrorX",
                                  Const::mirrorThickness/2.,
                                  Const::waterBoxSizeY/2. + Const::acrylicWallThickness + Const::wallGap + Const::mirrorThickness,
                                  Const::waterBoxSizeZ/2. + Const::acrylicWallThickness + Const::wallGap + Const::mirrorThickness);
    G4VSolid *mirrorY = new G4Box("mirrorY",
                                  Const::waterBoxSizeX/2. + Const::acrylicWallThickness + Const::wallGap + Const::mirrorThickness,
                                  Const::mirrorThickness/2.,
                                  Const::waterBoxSizeZ/2. + Const::acrylicWallThickness + Const::wallGap + Const::mirrorThickness);
    G4VSolid *mirrorZ = new G4Box("mirrorZ",
                                  Const::waterBoxSizeX/2. + Const::acrylicWallThickness + Const::wallGap + Const::mirrorThickness,
                                  Const::waterBoxSizeY/2. + Const::acrylicWallThickness + Const::wallGap + Const::mirrorThickness,
                                  Const::mirrorThickness/2.);


    acrylicWallsLogical[0] = new G4LogicalVolume(acrylicWallX,
                                                 Acrylic,
                                                 "acrylicWallX1");

    acrylicWallsLogical[1] = new G4LogicalVolume(acrylicWallX,
                                                 Acrylic,
                                                 "acrylicWallX2");

    acrylicWallsLogical[2] = new G4LogicalVolume(acrylicWallY,
                                                 Acrylic,
                                                 "acrylicWallY1");

    acrylicWallsLogical[3] = new G4LogicalVolume(acrylicWallY,
                                                 Acrylic,
                                                 "acrylicWallY2");

    acrylicWallsLogical[4] = new G4LogicalVolume(acrylicWallZ,
                                                 Acrylic,
                                                 "acrylicWallZ1");

    mirrorsLogical[0] = new G4LogicalVolume(mirrorX,
                                            INOX,
                                            "mirrorX1");
    mirrorsLogical[1] = new G4LogicalVolume(mirrorX,
                                            INOX,
                                            "mirrorX2");
    mirrorsLogical[2] = new G4LogicalVolume(mirrorY,
                                            INOX,
                                            "mirrorY1");
    mirrorsLogical[3] = new G4LogicalVolume(mirrorY,
                                            INOX,
                                            "mirrorY2");
    mirrorsLogical[4] = new G4LogicalVolume(mirrorZ,
                                            INOX,
                                            "mirrorZ1");

    new G4PVPlacement(0,
                      G4ThreeVector(- Const::waterBoxSizeX/2. - Const::acrylicWallThickness/2.,
                                    0,
                                    0),
                      acrylicWallsLogical[0],
            acrylicWallsLogical[0]->GetName(),
            worldLogical,
            false,
            0);
    new G4PVPlacement(0,
                      G4ThreeVector(Const::waterBoxSizeX/2. + Const::acrylicWallThickness/2.,
                                    0,
                                    0),
                      acrylicWallsLogical[1],
            acrylicWallsLogical[1]->GetName(),
            worldLogical,
            false,
            0);
    new G4PVPlacement(0,
                      G4ThreeVector(0,
                                    -Const::waterBoxSizeY/2. - Const::acrylicWallThickness/2.,
                                    0),
                      acrylicWallsLogical[2],
            acrylicWallsLogical[2]->GetName(),
            worldLogical,
            false,
            0);
    new G4PVPlacement(0,
                      G4ThreeVector(0,
                                    Const::waterBoxSizeY/2. + Const::acrylicWallThickness/2.,
                                    0),
                      acrylicWallsLogical[3],
            acrylicWallsLogical[3]->GetName(),
            worldLogical,
            false,
            0);
    new G4PVPlacement(0,
                      G4ThreeVector(0,
                                    0,
                                    -Const::waterBoxSizeZ/2. - Const::acrylicWallThickness/2.),
                      acrylicWallsLogical[4],
            acrylicWallsLogical[4]->GetName(),
            worldLogical,
            false,
            0);



    new G4PVPlacement(0,
                      G4ThreeVector(-Const::waterBoxSizeX/2. - Const::acrylicWallThickness
                                    - Const::wallGap - Const::mirrorThickness/2.,
                                    0,
                                    0),
                      mirrorsLogical[0],
            mirrorsLogical[0]->GetName(),
            worldLogical,
            false,
            0);
    new G4PVPlacement(0,
                      G4ThreeVector(Const::waterBoxSizeX/2. + Const::acrylicWallThickness
                                    + Const::wallGap + Const::mirrorThickness/2.,
                                    0,
                                    0),
                      mirrorsLogical[1],
            mirrorsLogical[1]->GetName(),
            worldLogical,
            false,
            0);
    new G4PVPlacement(0,
                      G4ThreeVector(0,
                                    -Const::waterBoxSizeY/2. - Const::acrylicWallThickness
                                    - Const::wallGap - Const::mirrorThickness/2.,
                                    0),
                      mirrorsLogical[2],
            mirrorsLogical[2]->GetName(),
            worldLogical,
            false,
            0);
    new G4PVPlacement(0,
                      G4ThreeVector(0,
                                    Const::waterBoxSizeY/2. + Const::acrylicWallThickness
                                    + Const::wallGap + Const::mirrorThickness/2.,
                                    0),
                      mirrorsLogical[3],
            mirrorsLogical[3]->GetName(),
            worldLogical,
            false,
            0);
    new G4PVPlacement(0,
                      G4ThreeVector(0,
                                    0,
                                    -Const::waterBoxSizeZ/2. - Const::acrylicWallThickness
                                    - Const::wallGap - Const::mirrorThickness/2.),
                      mirrorsLogical[4],
            mirrorsLogical[4]->GetName(),
            worldLogical,
            false,
            0);




    DefineOpticalBorders();
    SetVisAttributes();


    G4cout << "_____________________________________________Volumes are made" << G4endl;
    return worldPhysical;
}


// Definition of absorbtion surfaces
void DetectorConstruction::DefineOpticalBorders()
{
    const G4int num1 = 2;
    G4double Ephoton[num1] = {1.7*eV, 6.2*eV};

    G4OpticalSurface* OpVolumeKillSurface =
            new G4OpticalSurface("VolumeKillSurface");
    OpVolumeKillSurface->SetType(dielectric_metal);
    OpVolumeKillSurface->SetFinish(polished);
    OpVolumeKillSurface->SetModel(glisur);

    G4double ReflectivityKill[num1] = {0., 0.};
    G4double EfficiencyKill[num1] = {1., 1.};
    G4MaterialPropertiesTable* VolumeKill = new G4MaterialPropertiesTable();
    VolumeKill->AddProperty("REFLECTIVITY", Ephoton, ReflectivityKill, num1);
    VolumeKill->AddProperty("EFFICIENCY",   Ephoton, EfficiencyKill,   num1);
    OpVolumeKillSurface->SetMaterialPropertiesTable(VolumeKill);

    G4OpticalSurface* OpVolumeMirrorSurface =
            new G4OpticalSurface("VolumeMirrorSurface");
    OpVolumeMirrorSurface->SetType(dielectric_metal);
    OpVolumeMirrorSurface->SetFinish(polished);
    OpVolumeMirrorSurface->SetModel(glisur);
    G4double ReflectivityMirror[num1] = {0.98, 0.98};
    G4double EfficiencyMirror[num1] = {0., 0};
    G4MaterialPropertiesTable* VolumeMirror = new G4MaterialPropertiesTable();
    VolumeMirror->AddProperty("REFLECTIVITY", Ephoton, ReflectivityMirror, num1);
    VolumeMirror->AddProperty("EFFICIENCY", Ephoton, EfficiencyMirror, num1);
    OpVolumeMirrorSurface->SetMaterialPropertiesTable(VolumeMirror);

    G4OpticalSurface* quartzSurface = new G4OpticalSurface("quartzBorder");
    quartzSurface->SetType(dielectric_dielectric);

    new G4LogicalSkinSurface("DetectorAbsSurface",
                             pmtSphereLogical1, OpVolumeKillSurface);
    new G4LogicalSkinSurface("DetectorAbsSurface",
                             pmtSphereLogical2, OpVolumeKillSurface);

    for (int j = 0; j < 5; ++j)
        new G4LogicalSkinSurface("MirrorSurface",
                                 mirrorsLogical[j],
                                 OpVolumeMirrorSurface);
    //    for (int j = 0; j < LConst::pmt_n_channels*2; ++j) {
    //        new G4LogicalSkinSurface("DetectorAbsSurface",
    //                                 LDetectorOut[j], OpVolumeKillSurface);
    //        new G4LogicalSkinSurface("sectorSurface",
    //                                 LWindowOut[j], quartzSurface);
    //        new G4LogicalSkinSurface("sectorSurface",
    //                                 LBarOut[j], quartzSurface);
    //    }



}

// Visual attributes
void DetectorConstruction::SetVisAttributes()
{
    G4Color blue        = G4Color(0., 0., 1.);
    G4Color green       = G4Color(0., 1., 0.);
    G4Color red         = G4Color(1., 0., 0.);
    G4Color white       = G4Color(1., 1., 1.);
    G4Color cyan        = G4Color(0., 1., 1.);
    G4Color DircColor   = G4Color(0.0, 0.0, 1.0, 0.2);
    G4Color SensColor   = G4Color(0.0, 1.0, 1.0, 0.1);

    G4VisAttributes *sectorVisAtt = new G4VisAttributes;
    sectorVisAtt->SetColor(green);
    sectorVisAtt->SetVisibility(true);
    //    for (int j = 0; j < LConst::pmt_n_channels*2; ++j) {
    //        LWindowOut[j]->SetVisAttributes(sectorVisAtt);
    //    }
}
