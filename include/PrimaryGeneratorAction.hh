
#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1
#include "G4GeneralParticleSource.hh"
#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"

class G4ParticleGun;
class G4Event;
class DetectorConstruction;

#include <fstream>

using namespace std;

// -----------------------------------------------------------------------------------------

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
  PrimaryGeneratorAction();    
  virtual ~PrimaryGeneratorAction();
  void GeneratePrimaries(G4Event*);
private:
  G4ParticleGun*           particleGun;
};

// -----------------------------------------------------------------------------------------

#endif


