//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
// $Id: exampleN03.cc,v 1.39 2010/12/01 05:56:17 allison Exp $
// GEANT4 tag $Name: geant4-09-04 $
//
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4RunManager.hh"
#include "G4UImanager.hh"

#include "Randomize.hh"
#include "G4PhysListFactory.hh"
#include "G4VModularPhysicsList.hh"
#include "G4EmStandardPhysicsSS.hh"
#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "QGSP_FTFP_BERT.hh"
#include "QGSP_BIC.hh"
#include "QBBC.hh"
#include "QGSP_BERT.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4EmUserPhysics.hh"
#include "G4SystemOfUnits.hh"
#include "SteppingAction.hh"
#include "time.h"
#include "G4StepLimiterPhysics.hh"
#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

//#include "TH1.h"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//int main(int argc,char** argv)
int main(int argc,char ** argv)
{
	
  //
  G4cout<<"er vi i main"<<G4endl;
  clock_t tStart = clock();
  G4RunManager * runManager = new G4RunManager;
  
  // Set mandatory initialization classes
  DetectorConstruction* detector = new DetectorConstruction(argv[1]);
  G4cout<<"detector construction done"<<G4endl;
  runManager->SetUserInitialization(detector);
  G4PhysListFactory factory;
  G4VModularPhysicsList* phys = factory.GetReferencePhysList("FTFP_BERT");
  phys->ReplacePhysics(new G4EmStandardPhysicsSS());
  phys->RegisterPhysics(new G4EmUserPhysics());  
  runManager->SetUserInitialization(phys);
  
  // Set user action classes
  PrimaryGeneratorAction* gen_action = new PrimaryGeneratorAction();
  runManager->SetUserAction(gen_action);
  //
  RunAction* run_action = new RunAction();  
  runManager->SetUserAction(run_action);
  //
  EventAction* event_action = new EventAction();
  runManager->SetUserAction(event_action);
  SteppingAction *stepping=new SteppingAction(event_action);
  runManager->SetUserAction(stepping);
  runManager->Initialize();
   
// Begin comment
#ifdef G4VIS_USE
  // Initialize visualization
  G4VisManager* visManager = new G4VisExecutive;
  // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
  // G4VisManager* visManager = new G4VisExecutive("Quiet");
  visManager->Initialize();
#endif

  // Get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  if (argc!=2)   // batch mode
    {
      G4String command = "/control/execute ";
      G4String fileName = argv[2];
      UImanager->ApplyCommand(command+fileName);
    }
  else
    {  // interactive mode : define UI session
#ifdef G4UI_USE
      G4UIExecutive* ui = new G4UIExecutive(argc, argv);
#ifdef G4VIS_USE
      UImanager->ApplyCommand("/control/execute vis.mac"); 
#endif
      if (ui->IsGUI())
	ui->SessionStart();
      delete ui;
#endif
    }
#ifdef G4VIS_USE
  delete visManager;
#endif
//End comment	
  // Delete runManager
  delete runManager;
  G4cout << "runManager deleted" << G4endl;
  printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
  return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....
