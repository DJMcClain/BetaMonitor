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
/// \file electromagnetic/TestEm3/src/StepMax.cc
/// \brief Implementation of the StepMax class
//
// $Id: StepMax.cc 98762 2016-08-09 14:08:07Z gcosmo $
//

#include "StepMax.hh"
#include "StepMaxMessenger.hh"

StepMax::StepMax(const G4String& processName)
 : G4VDiscreteProcess(processName),fMess(0)
{
  for (G4int k=0; k<kMaxAbsor; k++) fStepMax[k] = DBL_MAX;
  fMess = new StepMaxMessenger(this);
}

StepMax::~StepMax() { delete fMess; }

G4bool StepMax::IsApplicable(const G4ParticleDefinition& particle)
{
  return (particle.GetPDGCharge() != 0. && !particle.IsShortLived());
}

void StepMax::SetStepMax(G4int k,G4double step) {fStepMax[k] = step;}
