/*
 * LConst.hh
 *
 *  Created on: Oct 2, 2018
 *      Author: vsevolod
 */

#pragma once

#include "TMath.h"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "globals.hh"

#include "iostream"

namespace Const{

// World size
const G4double worldSizeX = 3*m;
const G4double worldSizeY = 3*m;
const G4double worldSizeZ = 3*m;

const G4double waterBoxSizeX = 420*mm;
const G4double waterBoxSizeY = 640*mm;
const G4double waterBoxSizeZ = 892*mm;


const G4double acrylicBoxSizeX = 420*mm;
const G4double acrylicBoxSizeY = 640*mm;
const G4double acrylicBoxSizeZ = 250*mm;


const G4double pmtRadius = 190*mm / 2.;

}

