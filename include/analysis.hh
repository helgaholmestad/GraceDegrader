#ifndef ANALYSIS_HH_
#define ANALYSIS_HH_
#include "G4Event.hh"
#include "G4Run.hh"
#include "DetectorConstruction.hh"
#include <iostream>
#include <vector>
#include <fstream>
using namespace std;
class analysis {
public:
	//! Singleton pattern
  static analysis* GetInstance() {
    if ( analysis::singleton == NULL ) analysis::singleton = new analysis();
    return analysis::singleton;
  }
  void makeFile();
  void writeFile();
  void writePerEvent(const G4Event* event);
  ofstream outfile;
private: 
  analysis();
  //! Singleton static instance
  static analysis* singleton;
};

  
#endif
