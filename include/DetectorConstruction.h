/*
 * H_DetectorConstruction.h
 *
 *  Created on: Oct 2, 2018
 *      Author: vsevolod
 */

#pragma once

#include <G4VUserDetectorConstruction.hh>
#include "SensitiveDetector.h"

#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Trd.hh"
#include "G4Trap.hh"
#include "G4Sphere.hh"
#include "G4ExtrudedSolid.hh"
#include "G4VSolid.hh"
#include "G4UnionSolid.hh"
#include "G4MultiUnion.hh"
#include "G4SubtractionSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4Transform3D.hh"
#include "G4PVPlacement.hh"
#include "G4AssemblyVolume.hh"
#include "G4VisAttributes.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4Color.hh"
#include "G4TwoVector.hh"
#include "G4SDManager.hh"
#include "globals.hh"
#include "G4LogicalBorderSurface.hh"

#include "Const.hh"
#include "VolumeStructures.h"
#include "EventAction.h"

class DetectorConstruction: public G4VUserDetectorConstruction {
public:
    DetectorConstruction();
    virtual ~DetectorConstruction();

public:
    G4VPhysicalVolume* Construct();
    void ConstructSDandField();
    void DefineOpticalBorders();
    void SetVisAttributes();

    void DefineMateials();
    G4VPhysicalVolume* DefineVolumes();
    SensitiveDetector *LSD;



    G4LogicalVolume *worldLogical;

    inline void SetEventAction(EventAction* evAct){ _eventAction = evAct; };

private:
    G4Material *worldMaterial;
    G4Material *ScintMaterial;
    G4Material *Vacuum;
    G4Material *BPMaterial;
    G4Material *INOX;
    G4Material *SiO2;
    G4Material *Copper;
    G4Material *Beryllium;
    G4Material *Acrylic;
    G4Material *Water;


    G4Material *Air;

    EventAction *_eventAction;

    G4LogicalVolume *acrylicBoxLogical;
    G4LogicalVolume *acrylicCupWaterLogical1;
    G4LogicalVolume *acrylicCupWaterLogical2;

    G4LogicalVolume *pmtSphereLogical1;
    G4LogicalVolume *pmtSphereLogical2;

    G4LogicalVolume *acrylicWallsLogical[5];
    G4LogicalVolume *mirrorsLogical[5];

    G4RotationMatrix *Ra;
    G4ThreeVector Ta;



};
