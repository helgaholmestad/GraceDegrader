#include "analysis.hh"
#include "AntiPHit.hh"
#include "DetectorConstruction.hh"
#include "G4SDManager.hh"
#include "G4RunManager.hh"
#include "G4DigiManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4Track.hh"
#include <iostream>
#include <sstream>
using namespace std;
analysis* analysis::singleton = 0;

//Constructor: it initializes parameters
analysis::analysis(){;
}
void analysis::makeFile(){
  string filename="Degrader.txt";
  outfile.open(filename.c_str(),std::ios::trunc);
}

void analysis::writePerEvent(const G4Event* event){
  //outfile<<"new"<<"\n";
  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  G4int AntiPCollectionID = SDman->GetCollectionID("AntiPCollection");
  G4HCofThisEvent * HCE=event->GetHCofThisEvent();
  AntiPHitsCollection* hc=0;
  hc=(AntiPHitsCollection*)(HCE->GetHC(AntiPCollectionID));
  
 
  G4int nEntries = hc->entries();  
  for(G4int itr  = 0 ; itr < nEntries ; itr++) {
    if((*hc)[itr]->GetParticleName()=="anti_proton"){
      if((*hc)[itr]->GetMyParticleEnergy()/keV>10.0){
	continue;
      }
      if(((*hc)[itr]->GetParticleName()!="anti_proton")||((*hc)[itr]->GetHitPosition().x()/cm>3.0))
	{
	  continue;
	}
      outfile<<(*hc)[itr]->GetMyParticleEnergy()/keV<<"  "<<(*hc)[itr]->GetHitPosition().x()/cm<<"  "<<(*hc)[itr]->GetHitPosition().y()/cm<<"  "<<(*hc)[itr]->GetHitPosition().z()/cm<<"  "<<(*hc)[itr]->GetMyMomentumDirection().x()<<"  "<<(*hc)[itr]->GetMyMomentumDirection().y()<<"  "<<(*hc)[itr]->GetMyMomentumDirection().z()<<"  "<<event->GetEventID()<<"\n";
    }
  }
}  
void analysis::writeFile(){
  outfile.close();
}

