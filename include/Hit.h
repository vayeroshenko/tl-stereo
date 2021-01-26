/*
 * Hit.h
 *
 *  Created on: Oct 3, 2018
 *      Author: vsevolod
 */

#pragma once

#include <G4VHit.hh>
//G4
#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"

//my
#include "HitDataStructure.hh"

class myHit: public G4VHit {
public:
    myHit();
    virtual ~myHit();

    myHit(const myHit&);
    const myHit& operator=(const myHit&);
    G4int operator==(const myHit&) const;

	inline void* operator new(size_t);
	inline void  operator delete(void*);

	void Draw();
	void Print();

public:

	HitData myData;

private:

};

typedef G4THitsCollection<myHit> HitsCollection;

extern G4Allocator<myHit> HitAllocator;

inline void* myHit::operator new(size_t)
{
	void *aHit;
    aHit = (void *) HitAllocator.MallocSingle();
	return aHit;
}

inline void myHit::operator delete(void *aHit)
{
    HitAllocator.FreeSingle((myHit*) aHit);
}
