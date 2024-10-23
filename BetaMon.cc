#define SEG_DBG
#include <fstream>
#include <string>
#include "g4root.hh"

#ifdef SEG_DBG
  #include <stdio.h>
  #include <execinfo.h>
  #include <signal.h>
  #include <stdlib.h>
  #include <unistd.h>
#endif

//#ifdef G4VIS_USE
   #include "G4VisExecutive.hh"
//#endif
//#ifdef G4UI_USE
   #include "G4UIExecutive.hh"
//#endif
#include "G4Types.hh"
#include "G4UImanager.hh"
#ifdef G4MULTITHREADED
   #include "G4MTRunManager.hh"
#else
  #include "G4RunManager.hh"
#endif
#include "BM_RunAction.hh"
#include "BM_EventAction.hh"
#include "BM_SteppingAction.hh"
//#include "BM_TrackingAction.hh"
#include "ActionInitialization.hh"
//#include "BM_PhysicsList.hh"
#include "QBBC.hh"

#include "BM_Detector.hh"
#include "BM_PrimaryGenerator.hh"
#include "BM_Output.hh"
#include <TFile.h>

class TFile;
#ifdef SEG_DBG
void handler(int sig) // for segfault
{
   void* array[10];
   size_t size;
   size = backtrace(array, 10);
   fprintf(stderr, "Error: signal-%d:\n", sig);
   backtrace_symbols_fd(array, size, STDERR_FILENO);
   exit(1);
}
#endif

int main(int argc, char** argv)
{ //G4cout << "main 1" << G4endl;
#ifdef SEG_DBG
  std::cout << "Using our segfault debugger...\n";
  signal(SIGSEGV, handler); // using the segfault handler
#endif

  // unsigned int count;
  // std::ifstream countIn;
  // std::ofstream countOut;
  //countOut.open("counter.txt", std::ios::out);
  // countOut << count;
  // countOut.close();
  // countIn.open("counter.txt", std::ios::in);
  // //G4cout << "main 2" << G4endl;
  // if(countIn.is_open())
  // {
  //   countIn >> count;
  //   count++;
  // }
  // else
  // {
  //   count = 1;
  // }
  // countIn.close();
  // countOut.open("counter.txt", std::ios::out);
  // countOut << count;
  // countOut.close();

  //Change to G4MTRunManager if we want multithreading
  #ifdef G4MULTITHREADED
  G4MTRunManager* runManager = new G4MTRunManager;
  runManager->SetNumberOfThreads((G4Threading::G4GetNumberOfCores())-2);
  G4cout << "Multithreaded" << G4endl;
  #else
  G4RunManager* runManager = new G4RunManager;
  G4cout << "Single threaded" << G4endl;
  #endif
  runManager->SetVerboseLevel(1);
  //G4cout << "main 3" << G4endl;
  runManager->SetUserInitialization(new BM_Detector());
  //runManager->SetUserInitialization(new BM_PhysicsList());
  //physlist->AddPhysicsList("local");
  G4VModularPhysicsList* physicsList = new QBBC;
  physicsList->SetVerboseLevel(1);
  runManager->SetUserInitialization(physicsList);
  runManager->SetUserInitialization(new ActionInitialization());
  //G4WorkerRunManager* WorkManager = new G4WorkerRunManager;
  //WorkManager->SetUserAction(new BM_PrimaryGenerator());
  //runManager->SetUserAction(new BM_SteppingAction());
  //runManager->SetUserAction(new BM_EventAction());
  //runManager->SetUserAction(new BM_RunAction());
  //runManager->SetUserAction(new BM_TrackingAction());
  //G4cout << "main 4" << G4endl;
  //runManager->Initialize();

  //#ifdef G4VIS_USE
  // Initialize visualization
  G4VisManager* visManager = new G4VisExecutive;
  // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
   //G4VisManager* visManager = new G4VisExecutive("Quiet");
  visManager->Initialize();
  //#endif

  // Get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();
  //G4cout << "main 5" << G4endl;


  //G4cout << "The output for this run will be stored in " << outname << G4endl;
  //BM_Output::Instance()->SetFilename(outname);
  BM_Output::Instance()->SetFilename();
  if (argc != 1)
  {
    // batch mode
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UImanager->ApplyCommand(command + fileName);
  }
  else
  {
  // interactive mode : define UI session
  //#ifdef G4UI_USE
     G4UIExecutive* ui = 0;
     ui = new G4UIExecutive(argc, argv);
      //G4cout << "main 6" << G4endl;
    //#ifdef G4VIS_USE
      UImanager->ApplyCommand("/control/execute ../init_vis.mac");
    //#else
    //  UImanager->ApplyCommand("/control/execute init.mac");
    //#endif
    ui->SessionStart();
    delete ui;
  //#endif
  }


  // Job termination
  // Free the store: user actions, physics_list and detector_description are
  // owned and deleted by the run manager, so they should not be deleted
  // in the main() program !
  #ifdef G4VIS_USE
    delete visManager;
  #endif
  delete runManager;

  //G4cout << "Writing file to " << outname << "; goodbye" << G4endl;

  return 0;
}
