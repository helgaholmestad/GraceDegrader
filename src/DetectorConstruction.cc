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
////
// $Id: DetectorConstruction.cc,v 1.1 2010/10/18 15:56:17 maire Exp $
// GEANT4 tag $Name: geant4-09-04 $
//
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "DetectorConstruction.hh"
#include "AntiPSD.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4CutTubs.hh"
#include "G4LogicalVolume.hh"
#include "G4VPVParameterisation.hh"
#include "G4PVPlacement.hh"
#include "G4PVParameterised.hh"
#include "G4UniformMagField.hh"
#include "G4UniformElectricField.hh"
#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"
#include "G4Element.hh"
#include "G4GeometryManager.hh"
#include "G4PhysicalConstants.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4SDManager.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "globals.hh"
#include "G4UnionSolid.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"
#include "G4EqMagElectricField.hh"
#include "G4MagIntegratorStepper.hh"
#include "G4ClassicalRK4.hh"
#include "G4MagIntegratorDriver.hh"
#include "G4ChordFinder.hh"
#include "AntiPTestCellParameterisation.hh"
#include "G4SubtractionSolid.hh"
#include "G4UserLimits.hh"
#include <iostream>
#include <stdlib.h>     /* strtod */
//------------------------------------------------------------------------------

DetectorConstruction::DetectorConstruction(char * input)
{
  //std::string str(degraderThickness)
  degraderThickness=atof(input);  
  degraderThickness=degraderThickness*1.0*um;
}

//------------------------------------------------------------------------------

DetectorConstruction::~DetectorConstruction()
{}

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------



//------------------------------------------------------------------------------

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  G4RotationMatrix* RotChamber = new G4RotationMatrix;
  RotChamber->rotateY(M_PI/2.0);
  DefineMaterials();

  //Defining the world. Make it a small size such that particles are not tracked loonger than necassary
  G4double WorldSize=18.0*cm;
  G4Box* solidWorld = new G4Box("World",				//its name
			 WorldSize/2.0,WorldSize/2.0,WorldSize/2.0);	//its size, divide by two to get the right size
  
  G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld,		//its solid
						    defaultMaterial,	//its material
				   "World");		//its name
  
  G4VPhysicalVolume* physiWorld = new G4PVPlacement(0,			//no rotation
						    G4ThreeVector(0,0.0,0.0),	//at (0,0,0)
				 logicWorld,		//its logical volume				 
				 "World",		//its name
				 0,			//its mother  volume
				 true,			//no boolean operation
				 0);			//copy number




  //Defining the AD foil. This is defined to be to be 50 um of stainless steel. See chapter   of thesis. 
  G4Box * solidADFoil = new G4Box("ADFoil",		//its name
  				  (50*um)/2.0, 5*cm,5*cm);
  
  G4LogicalVolume*  logicADFoil = new G4LogicalVolume(solidADFoil,    //its solid
  				    StainlessSteel, //its material
  				    "ADFoil"); //name 
  
  G4PVPlacement* physiADFoil = new G4PVPlacement(0,		   //no rotation
  						 G4ThreeVector(-3.0*cm,0.0,0.0),  //its position
  						 logicADFoil,     //its logical volume		    
  						 "ADFoil", //its name
  						 logicWorld,        //its mother
  						 false,             //no boulean operat
  						 0);                //copy number  



  //This is the varialbe degrader 
  G4Box * solidDegrader = new G4Box("degrader",		//its name
  				    (degraderThickness/2.0), 5*cm,5*cm);
  
  G4LogicalVolume*  logicDegrader = new G4LogicalVolume(solidDegrader,    //its solid
  				    AlMaterial, //its material
  				    "degrader"); //name 
  
  G4PVPlacement* physiDegrader = new G4PVPlacement(0,		   //no rotation
  						 G4ThreeVector(0.0*cm,0.0,0.0),  //its position
  						 logicDegrader,     //its logical volume		    
  						 "degrader", //its name
  						 logicWorld,        //its mother
  						 false,             //no boulean operat
  						 0);                //copy number  
  


  //This is the chamber foil, that is the foil on the 
  G4Box * solidChamberFoil = new G4Box("ChamberFoil",		//its name
  				  25.0/2*um, 5*cm,5*cm); 
  G4LogicalVolume*  logicChamberFoil = new G4LogicalVolume(solidChamberFoil,    //its solid
  				    TiMaterial, //its material
  				    "ChamberFoil"); //name 
  
  G4PVPlacement* physiChamberFoil = new G4PVPlacement(0,		   //no rotation
						      G4ThreeVector(3*cm-50.0/2*um,0.0,0.0),  //its position
  				  logicChamberFoil,     //its logical volume		    
  				  "ChamberFoil", //its name
  				  logicWorld,        //its mother
  				  false,             //no boulean operat
  				  0);                //copy number  



  
  
  G4Tubs* solidSilicon = new G4Tubs("Silicon",0,2.0*cm,0.5*cm,0,2*M_PI);
  G4LogicalVolume* logicSilicon = new G4LogicalVolume(solidSilicon,    //its solid
  						      vac, //its material
  						      "siliconDetector"); //name

  G4PVPlacement* physiSilicon1 = new G4PVPlacement(RotChamber,		   //no rotation
  						   G4ThreeVector(3.5*cm,0,0.0*cm),//*cm*std::ta
						   //(9*M_PI/2)+10*cm),  its position
						   //G4ThreeVector(-1.5*cm,0,0.0*cm),//*cm*std::tan(9*M_PI/2)+10*cm),  //its position
  						   logicSilicon,   //  its logical volume		    
  						   "siliconDetector",// its name
  						   logicWorld,       // its mother
  						   true,            // no boulean operat
  						   7);              //  copy number



  G4cout<<"kommer vi hit"<<G4endl;
  
  //Here we make the detector sensitive such that the hits are recorded in this detector
    G4VSensitiveDetector* detector = new AntiPSD("Silicon");
  // Get pointer to detector manager                                                     
  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  // Register detector with manager                                                      
  SDman->AddNewDetector(detector);
  logicSilicon->SetSensitiveDetector(detector);

  //Visualization attributes
  logicWorld->SetVisAttributes (G4VisAttributes::Invisible);
  G4cout<<"noe galt med visualisering"<<G4endl;
  return physiWorld;
  
}

//------------------------------------------------------------------------------


void DetectorConstruction::DefineMaterials()
{ 
  
  G4NistManager* man = G4NistManager::Instance();
  man->SetVerbose(1);
  // define Elements  
  G4Material* Si= man->FindOrBuildMaterial("G4_Si");
  G4Material* Al = man->FindOrBuildMaterial("G4_Al");
  G4Material* Ti =man->FindOrBuildMaterial("G4_Ti");  
  G4Material* Ss= man->FindOrBuildMaterial("G4_STAINLESS-STEEL");
  G4Material* Air=man->FindOrBuildMaterial("G4_AIR");
  
  
  G4Material* GoodVac    = man->FindOrBuildMaterial("G4_Galactic");  
  //  G4Material* Vacuum = new G4Material("TurboPumpVacuum", z=7., a=14.0*g/mole,density= vacuumDensity,kStateGas, 290,pressure);  
  defaultMaterial  =Air;
  
 //ThermalOMaterial = SiO2;
  AlMaterial = Al;
  TiMaterial =Ti;
  SiliconMaterial=Si;
  StainlessSteel=Ss;
  vac=GoodVac;
 
}


