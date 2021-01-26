/*
 * Hit.cpp
 *
 *  Created on: Oct 3, 2018
 *      Author: vsevolod
 */

#include "Hit.h"

#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Square.hh"
#include "G4Color.hh"
#include "G4VisAttributes.hh"
#include "G4UnitsTable.hh"

G4Allocator<myHit> HitAllocator;

myHit::myHit() :
    G4VHit()
{}

myHit::~myHit() {}

myHit::myHit(const myHit& right) :
    G4VHit()
{
    myData = right.myData;
}

const myHit& myHit::operator=(const myHit& right)
{
    myData = right.myData;
    return *this;
}

G4int myHit::operator==(const myHit& right) const
{
    return (this == &right) ? 1 : 0;
}

void myHit::Draw()
{
    G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
    if (pVVisManager) {
        G4ThreeVector pos =
                G4ThreeVector(myData.X, myData.Y, myData.Z);
        G4Circle circle(pos);
        circle.SetScreenSize(4.);
        circle.SetFillStyle(G4Circle::filled);
        G4Color color(1., 0., 0.);
        G4VisAttributes att(color);
        circle.SetVisAttributes(att);
        pVVisManager->Draw(circle);
    }
}

void myHit::Print()
{
    /*
  G4cout << "Hit Print: "
     << " trackID: " << trackID
     << " energy: " << G4BestUnit(energy, "Energy")
     << " position: " << G4BestUnit(pos, "Length")
     << " time: " << G4BestUnit(time, "Time")
     << G4endl;
  */
}
