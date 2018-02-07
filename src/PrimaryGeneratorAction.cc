#include "PrimaryGeneratorAction.hh"
#include "DetectorConstruction.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "Randomize.hh"
#include "globals.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include <iostream>
#include <cmath>
#include "G4GeneralParticleSource.hh"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
//#include <random>



//#include "CLHEP/Random/RandGauss.h"
//#include "/Applications/geant4.9.6.p02/source/externals/clhep/include/CLHEP/Random/RandLandau.h" //(include this before method definition)

// -----------------------------------------------------------------------------------------

PrimaryGeneratorAction::PrimaryGeneratorAction()
{
  G4String particleName = "anti_proton";
  particleGun  = new G4ParticleGun();
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* particle = particleTable->FindParticle(particleName);  	
  particleGun->SetParticleDefinition(particle);
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{ 
  delete particleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent){
  //We simulate the particle as a point beam with energy 5.3MeV
  particleGun->SetParticleEnergy(5.3*MeV);
  particleGun->SetParticlePosition(G4ThreeVector(-3.0*cm-(25.0*um),0,0));
  particleGun->SetParticleMomentumDirection(G4ThreeVector(1,0,0));
  particleGun->GeneratePrimaryVertex(anEvent);
  
}

