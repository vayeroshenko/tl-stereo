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

class Hit: public G4VHit {
public:
    Hit();
    virtual ~Hit();

    Hit(const Hit&);
    const Hit& operator=(const Hit&);
    G4int operator==(const Hit&) const;

	inline void* operator new(size_t);
	inline void  operator delete(void*);

	void Draw();
	void Print();

public:

	HitData myData;

private:

};

typedef G4THitsCollection<Hit> HitsCollection;

extern G4Allocator<Hit> HitAllocator;

inline void* Hit::operator new(size_t)
{
	void *aHit;
    aHit = (void *) HitAllocator.MallocSingle();
	return aHit;
}

inline void Hit::operator delete(void *aHit)
{
    HitAllocator.FreeSingle((Hit*) aHit);
}
