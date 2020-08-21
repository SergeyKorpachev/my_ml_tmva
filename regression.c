#include <cstdlib>
#include <iostream>
#include <map>
#include <string>
#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TObjString.h"
#include "TSystem.h"
#include "TROOT.h"
#include "TMVA/Tools.h"
#include "TMVA/Factory.h"
#include "TMVA/DataLoader.h"
#include "TMVA/TMVARegGui.h"
using namespace TMVA;
void regression( TString myMethodList = "" )
{
   // The explicit loading of the shared libTMVA is done in TMVAlogon.C, defined in .rootrc
   // if you use your private .rootrc, or run from a different directory, please copy the
   // corresponding lines from .rootrc
   // methods to be processed can be given as an argument; use format:
   //
   //     mylinux~> root -l TMVARegression.CUndefined control sequence \"
   //
   //---------------------------------------------------------------
   // This loads the library
   TMVA::Tools::Instance();
   // Default MVA methods to be trained + tested
   std::map<std::string,int> Use;
   // Mutidimensional likelihood and Nearest-Neighbour methods
   Use["PDERS"]           = 0;
   Use["PDEFoam"]         = 0;
   Use["KNN"]             = 0;
   //
   // Linear Discriminant Analysis
   Use["LD"]              = 0;
   //
   // Function Discriminant analysis
   Use["FDA_GA"]          = 0;
   Use["FDA_MC"]          = 0;
   Use["FDA_MT"]          = 0;
   Use["FDA_GAMT"]        = 0;
   //
   // Neural Network
   Use["MLP"]             = 1;
   Use["DNN_CPU"] = 0;
   //
   // Support Vector Machine
   Use["SVM"]             = 0;
   //
   // Boosted Decision Trees
   Use["BDT"]             = 0;
   Use["BDTG"]            = 0;
   // ---------------------------------------------------------------
   std::cout << std::endl;
   std::cout << "==> Start TMVARegression" << std::endl;
   // Select methods (don't look at this code - not of interest)
   if (myMethodList != "") {
      for (std::map<std::string,int>::iterator it = Use.begin(); it != Use.end(); it++) it->second = 0;
      std::vector<TString> mlist = gTools().SplitString( myMethodList, ',' );
      for (UInt_t i=0; i<mlist.size(); i++) {
         std::string regMethod(mlist[i]);
         if (Use.find(regMethod) == Use.end()) {
            std::cout << "Method \"" << regMethod << "\" not known in TMVA under this name. Choose among the following:" << std::endl;
            for (std::map<std::string,int>::iterator it = Use.begin(); it != Use.end(); it++) std::cout << it->first << " ";
            std::cout << std::endl;
            return;
         }
         Use[regMethod] = 1;
      }
   }
   // --------------------------------------------------------------------------------------------------
   // Here the preparation phase begins
   // Create a new root output file
   TString outfileName( "regression.root" );
   TFile* outputFile = TFile::Open( outfileName, "RECREATE" );
   // Create the factory object. Later you can choose the methods
   // whose performance you'd like to investigate. The factory will
   // then run the performance analysis for you.
   //
   // The first argument is the base of the name of all the
   // weightfiles in the directory weight/
   //
   // The second argument is the output file for the training results
   // All TMVA output can be suppressed by removing the "!" (not) in
   // front of the "Silent" argument in the option string
   TMVA::Factory *factory = new TMVA::Factory( "TMVARegression", outputFile,
                                               "!V:!Silent:Color:DrawProgressBar:AnalysisType=Regression" );
   TMVA::DataLoader *dataloader=new TMVA::DataLoader("dataset");
   // If you wish to modify default settings
   // (please check "src/Config.h" to see all available global options)
   //
   //     (TMVA::gConfig().GetVariablePlotting()).fTimesRMS = 8.0;
   //     (TMVA::gConfig().GetIONames()).fWeightFileDir = "myWeightDirectory";
   // Define the input variables that shall be used for the MVA training
   // note that you may also use variable expressions, such as: "3*var1/var2*abs(var3)"
   // [all types of expressions that can also be parsed by TTree::Draw( "expression" )]
//   dataloader->AddVariable( "var1", "Variable 1", "units", 'F' );
//   dataloader->AddVariable( "var2", "Variable 2", "units", 'F' );

//   dataloader->AddVariable( "energy" );
   dataloader->AddVariable( "eecal" );
   dataloader->AddVariable( "ehcal" );

//   dataloader->AddVariable( "nhits_t" );
   dataloader->AddVariable( "nhits_ecal_t" );
   dataloader->AddVariable( "nhits_hcal_t" );

//   dataloader->AddVariable( "time" );
   dataloader->AddVariable( "etime" );
   dataloader->AddVariable( "htime" );
//   dataloader->AddVariable( "radius" );
   dataloader->AddVariable( "eradius" );
   dataloader->AddVariable( "hradius" );

   dataloader->AddVariable( "elayer" );
   dataloader->AddVariable( "hlayer" );

//   dataloader->AddVariable( "energy_hit" );
   dataloader->AddVariable( "energy_hit_ecal" );
   dataloader->AddVariable( "energy_hit_hcal" );

   dataloader->AddVariable( "energy_hit_ecal_max" );
   dataloader->AddVariable( "energy_hit_hcal_max" );

/*   dataloader->AddVariable( "n30_t" );
   dataloader->AddVariable( "n35_t" );
   dataloader->AddVariable( "n40_t" );
   dataloader->AddVariable( "n45_t" );
   dataloader->AddVariable( "n50_t" );
   dataloader->AddVariable( "n55_t" );
   dataloader->AddVariable( "n60_t" );
   dataloader->AddVariable( "n65_t" );
   dataloader->AddVariable( "n70_t" );
   dataloader->AddVariable( "n75_t" );
   dataloader->AddVariable( "n80_t" );
   dataloader->AddVariable( "nav_t" );

   dataloader->AddVariable( "c30" );
   dataloader->AddVariable( "c35" );
   dataloader->AddVariable( "c40" );
   dataloader->AddVariable( "c45" );
   dataloader->AddVariable( "c50" );
   dataloader->AddVariable( "c55" );
   dataloader->AddVariable( "c60" );
   dataloader->AddVariable( "c65" );
   dataloader->AddVariable( "c70" );
   dataloader->AddVariable( "c75" );
   dataloader->AddVariable( "c80" );
*/
//   dataloader->AddVariable( "muon_energy" );
//   dataloader->AddVariable( "muon_energy_hit" );
//   dataloader->AddVariable( "muon_nhits_t" );
//   dataloader->AddVariable( "muon_time" );
//   dataloader->AddVariable( "muon_layer" );

   // You can add so-called "Spectator variables", which are not used in the MVA training,
   // but will appear in the final "TestTree" produced by TMVA. This TestTree will contain the
   // input variables, the response values of all trained MVAs, and the spectator variables
//   dataloader->AddSpectator( "spec1:=var1*2",  "Spectator 1", "units", 'F' );
//   dataloader->AddSpectator( "spec2:=var1*3",  "Spectator 2", "units", 'F' );
   // Add the variable carrying the regression target
//   dataloader->AddTarget( "fvalue" );
   dataloader->AddTarget( "mc_energy" );
   // It is also possible to declare additional targets for multi-dimensional regression, ie:
   //     factory->AddTarget( "fvalue2" );
   // BUT: this is currently ONLY implemented for MLP
   // Read training and test data (see TMVAClassification for reading ASCII files)
   // load the signal and background event samples from ROOT trees
   TFile *input(0);
//   TString fname = "./tmva_reg_example.root";
   TString fname = "./gun_k0L_allgev_FTFP_BERT_160000evt_ILD_l5_v02steel.root";
   if (!gSystem->AccessPathName( fname )) {
      input = TFile::Open( fname ); // check if file in local directory exists
   }
   else {
      TFile::SetCacheFileDir(".");
      input = TFile::Open("http://root.cern.ch/files/tmva_reg_example.root", "CACHEREAD"); // if not: download from ROOT server
   }
   if (!input) {
      std::cout << "ERROR: could not open data file" << std::endl;
      exit(1);
   }
   std::cout << "--- TMVARegression           : Using input file: " << input->GetName() << std::endl;
   // Register the regression tree
//   TTree *regTree = (TTree*)input->Get("TreeR");
   TTree *regTree = (TTree*)input->Get("tree");
   // global event weights per tree (see below for setting event-wise weights)
   Double_t regWeight  = 1.0;
   // You can add an arbitrary number of regression trees
   dataloader->AddRegressionTree( regTree, regWeight );
   // This would set individual event weights (the variables defined in the
   // expression need to exist in the original TTree)
//   dataloader->SetWeightExpression( "var1", "Regression" );
//   dataloader->SetWeightExpression( "energy", "Regression" );
   // Apply additional cuts on the signal and background samples (can be different)
   TCut mycut = "(energy_hit_ecal_max > 0.0 ) && (energy_hit_hcal_max > 0.0 )"; // for example: TCut mycut = "abs(var1)<0.5 && abs(var2-0.5)<1"; 
   // tell the DataLoader to use all remaining events in the trees after training for testing:
   dataloader->PrepareTrainingAndTestTree( mycut,
                                         "nTrain_Regression=5000:nTest_Regression=5000:SplitMode=Random:NormMode=NumEvents:!V" );
   //
   //     dataloader->PrepareTrainingAndTestTree( mycut,
   //            "nTrain_Regression=0:nTest_Regression=0:SplitMode=Random:NormMode=NumEvents:!V" );
   // If no numbers of events are given, half of the events in the tree are used
   // for training, and the other half for testing:
   //
   //     dataloader->PrepareTrainingAndTestTree( mycut, "SplitMode=random:!V" );
   // Book MVA methods
   //
   // Please lookup the various method configuration options in the corresponding cxx files, eg:
   // src/MethoCuts.cxx, etc, or here: http://tmva.sourceforge.net/optionRef.html
   // it is possible to preset ranges in the option string in which the cut optimisation should be done:
   // "...:CutRangeMin[2]=-1:CutRangeMax[2]=1"...", where [2] is the third input variable
   // PDE - RS method
   if (Use["PDERS"])
      factory->BookMethod( dataloader,  TMVA::Types::kPDERS, "PDERS",
                           "!H:!V:NormTree=T:VolumeRangeMode=Adaptive:KernelEstimator=Gauss:GaussSigma=0.3:NEventsMin=40:NEventsMax=60:VarTransform=None" );
   // And the options strings for the MinMax and RMS methods, respectively:
   //
   //      "!H:!V:VolumeRangeMode=MinMax:DeltaFrac=0.2:KernelEstimator=Gauss:GaussSigma=0.3" );
   //      "!H:!V:VolumeRangeMode=RMS:DeltaFrac=3:KernelEstimator=Gauss:GaussSigma=0.3" );
   if (Use["PDEFoam"])
       factory->BookMethod( dataloader,  TMVA::Types::kPDEFoam, "PDEFoam",
             "!H:!V:MultiTargetRegression=F:TargetSelection=Mpv:TailCut=0.001:VolFrac=0.0666:nActiveCells=500:nSampl=2000:nBin=5:Compress=T:Kernel=None:Nmin=10:VarTransform=None" );
   // K-Nearest Neighbour classifier (KNN)
   if (Use["KNN"])
      factory->BookMethod( dataloader,  TMVA::Types::kKNN, "KNN",
                           "nkNN=20:ScaleFrac=0.8:SigmaFact=1.0:Kernel=Gaus:UseKernel=F:UseWeight=T:!Trim" );
   // Linear discriminant
   if (Use["LD"])
      factory->BookMethod( dataloader,  TMVA::Types::kLD, "LD",
                           "!H:!V:VarTransform=None" );
   // Function discrimination analysis (FDA) -- test of various fitters - the recommended one is Minuit (or GA or SA)
   if (Use["FDA_MC"])
      factory->BookMethod( dataloader,  TMVA::Types::kFDA, "FDA_MC",
                          "!H:!V:Formula=(0)+(1)*x0+(2)*x1:ParRanges=(-100,100);(-100,100);(-100,100):FitMethod=MC:SampleSize=100000:Sigma=0.1:VarTransform=D" );
   if (Use["FDA_GA"]) // can also use Simulated Annealing (SA) algorithm (see Cuts_SA options) .. the formula of this example is good for parabolas
      factory->BookMethod( dataloader,  TMVA::Types::kFDA, "FDA_GA",
                           "!H:!V:Formula=(0)+(1)*x0+(2)*x1:ParRanges=(-100,100);(-100,100);(-100,100):FitMethod=GA:PopSize=100:Cycles=3:Steps=30:Trim=True:SaveBestGen=1:VarTransform=Norm" );
   if (Use["FDA_MT"])
      factory->BookMethod( dataloader,  TMVA::Types::kFDA, "FDA_MT",
                           "!H:!V:Formula=(0)+(1)*x0+(2)*x1:ParRanges=(-100,100);(-100,100);(-100,100);(-10,10):FitMethod=MINUIT:ErrorLevel=1:PrintLevel=-1:FitStrategy=2:UseImprove:UseMinos:SetBatch" );
   if (Use["FDA_GAMT"])
      factory->BookMethod( dataloader,  TMVA::Types::kFDA, "FDA_GAMT",
                           "!H:!V:Formula=(0)+(1)*x0+(2)*x1:ParRanges=(-100,100);(-100,100);(-100,100):FitMethod=GA:Converger=MINUIT:ErrorLevel=1:PrintLevel=-1:FitStrategy=0:!UseImprove:!UseMinos:SetBatch:Cycles=1:PopSize=5:Steps=5:Trim" );
   // Neural network (MLP)
   if (Use["MLP"])

//    factory->BookMethod( dataloader, TMVA::Types::kMLP, "MLP", "!H:!V:VarTransform=Norm:NeuronType=sigmoid:NCycles=1:HiddenLayers=N,N-1,N-2:TrainingMethod=BP:ConvergenceImprove=1e-8:ConvergenceTests=15:EpochMonitoring:UseRegulator:LearningRate=0.005:DecayRate=0.05:TestRate=5" );

   //  factory->BookMethod( dataloader,  TMVA::Types::kMLP, "MLP", "!H:!V:VarTransform=Norm:NeuronType=sigmoid:NCycles=200:HiddenLayers=N-5,N-7:TestRate=2:TrainingMethod=BP:Sampling=0.3:SamplingEpoch=0.8:LearningRate=0.005:ConvergenceImprove=1e-6:ConvergenceTests=15:!UseRegulator" );    // TrainingMethod=BFGS
    
    factory->BookMethod( dataloader, TMVA::Types::kMLP, "MLP", "!H:!V:VarTransform=Norm:NeuronType=tanh:NCycles=5000:HiddenLayers=N+5,N,N-1:TestRate=6:LearningRate=0.1:TrainingMethod=BFGS:Sampling=0.3:SamplingEpoch=0.8:ConvergenceImprove=1e-6:ConvergenceTests=15:!UseRegulator" );
    
   if (Use["DNN_CPU"]) {
      /*
          TString layoutString ("Layout=TANH|(N+100)*2,LINEAR");
          TString layoutString ("Layout=SOFTSIGN|100,SOFTSIGN|50,SOFTSIGN|20,LINEAR");
          TString layoutString ("Layout=RELU|300,RELU|100,RELU|30,RELU|10,LINEAR");
          TString layoutString ("Layout=SOFTSIGN|50,SOFTSIGN|30,SOFTSIGN|20,SOFTSIGN|10,LINEAR");
          TString layoutString ("Layout=TANH|50,TANH|30,TANH|20,TANH|10,LINEAR");
          TString layoutString ("Layout=SOFTSIGN|50,SOFTSIGN|20,LINEAR");
          TString layoutString ("Layout=TANH|100,TANH|30,LINEAR");
       */
      TString layoutString("Layout=TANH|100,LINEAR");
      TString training0("LearningRate=1e-5,Momentum=0.5,Repetitions=1,ConvergenceSteps=500,BatchSize=50,"
                        "TestRepetitions=7,WeightDecay=0.01,Regularization=NONE,DropConfig=0.5+0.5+0.5+0.5,"
                        "DropRepetitions=2");
      TString training1("LearningRate=1e-5,Momentum=0.9,Repetitions=1,ConvergenceSteps=170,BatchSize=30,"
                        "TestRepetitions=7,WeightDecay=0.01,Regularization=L2,DropConfig=0.1+0.1+0.1,DropRepetitions="
                        "1");
      TString training2("LearningRate=1e-5,Momentum=0.3,Repetitions=1,ConvergenceSteps=150,BatchSize=40,"
                        "TestRepetitions=7,WeightDecay=0.01,Regularization=NONE");
      TString training3("LearningRate=1e-6,Momentum=0.1,Repetitions=1,ConvergenceSteps=500,BatchSize=100,"
                        "TestRepetitions=7,WeightDecay=0.0001,Regularization=NONE");
      TString trainingStrategyString("TrainingStrategy=");
      trainingStrategyString += training0 + "|" + training1 + "|" + training2 + "|" + training3;
      //       TString trainingStrategyString
      //       ("TrainingStrategy=LearningRate=1e-1,Momentum=0.3,Repetitions=3,ConvergenceSteps=20,BatchSize=30,TestRepetitions=7,WeightDecay=0.0,L1=false,DropFraction=0.0,DropRepetitions=5");
      TString nnOptions(
         "!H:V:ErrorStrategy=SUMOFSQUARES:VarTransform=G:WeightInitialization=XAVIERUNIFORM:Architecture=CPU");
      //       TString nnOptions ("!H:V:VarTransform=Normalize:ErrorStrategy=CHECKGRADIENTS");
      nnOptions.Append(":");
      nnOptions.Append(layoutString);
      nnOptions.Append(":");
      nnOptions.Append(trainingStrategyString);
      factory->BookMethod(dataloader, TMVA::Types::kDNN, "DNN_CPU", nnOptions); // NN
   }
   // Support Vector Machine
   if (Use["SVM"])
      factory->BookMethod( dataloader,  TMVA::Types::kSVM, "SVM", "Gamma=0.25:Tol=0.001:VarTransform=Norm" );
   // Boosted Decision Trees
   if (Use["BDT"])
     factory->BookMethod( dataloader,  TMVA::Types::kBDT, "BDT",
                           "!H:!V:NTrees=100:MinNodeSize=1.0%:BoostType=AdaBoostR2:SeparationType=RegressionVariance:nCuts=20:PruneMethod=CostComplexity:PruneStrength=30" );
   if (Use["BDTG"])
     factory->BookMethod( dataloader,  TMVA::Types::kBDT, "BDTG",
                           "!H:!V:NTrees=2000::BoostType=Grad:Shrinkage=0.1:UseBaggedBoost:BaggedSampleFraction=0.5:nCuts=20:MaxDepth=3:MaxDepth=4" );
   // --------------------------------------------------------------------------------------------------
   // Now you can tell the factory to train, test, and evaluate the MVAs
   // Train MVAs using the set of training events
   factory->TrainAllMethods();
   // Evaluate all MVAs using the set of test events
   factory->TestAllMethods();
   // Evaluate and compare performance of all configured MVAs
   factory->EvaluateAllMethods();
   // --------------------------------------------------------------
   // Save the output
   outputFile->Close();
   std::cout << "==> Wrote root file: " << outputFile->GetName() << std::endl;
   std::cout << "==> TMVARegression is done!" << std::endl;
   delete factory;
   delete dataloader;
   // Launch the GUI for the root macros
   if (!gROOT->IsBatch()) TMVA::TMVARegGui( outfileName );
}
int main( int argc, char** argv )
{
   // Select methods (don't look at this code - not of interest)
   TString methodList;
   for (int i=1; i<argc; i++) {
      TString regMethod(argv[i]);
      if(regMethod=="-b" || regMethod=="--batch") continue;
      if (!methodList.IsNull()) methodList += TString(",");
      methodList += regMethod;
   }
   regression(methodList);
   return 0;
}

/*
 #include "TFile.h"
 #include "TTree.h"
 #include "TString.h"
 #include "TROOT.h"

 #include "TMVA/Factory.h"
 #include "TMVA/DataLoader.h"
 #include "TMVA/Tools.h"
 #include "TMVA/TMVARegGui.h"

 void regression1() {
     TFile *input;
     TString fname = "gun_k0L_allgev_FTFP_BERT_5000evt_ILD_l5_v02steel.root";
     input = TFile::Open(fname);
     
     TTree *regTree = (TTree*)input->Get("tree");
     
     TString outfileName("tmva_regression.root");
     TFile* outputFile = TFile::Open( outfileName, "RECREATE" );
     
     TMVA::Factory *factory = new TMVA::Factory( "TMVARegression", outputFile, "!V:!Silent:Color:DrawProgressBar:AnalysisType=Regression" );
     
     TMVA::DataLoader *dataloader = new TMVA::DataLoader("data");
     
     dataloader->AddVariable( "energy" );
     dataloader->AddVariable( "eecal" );
     dataloader->AddVariable( "ehcal" );
 //    dataloader->AddVariable( "nhits_ecal" );
 //    dataloader->AddVariable( "nhits_hcal" );
 //    dataloader->AddVariable( "edist" );
 //    dataloader->AddVariable( "hdist" );
 //    dataloader->AddVariable( "elayer" );
 //    dataloader->AddVariable( "hlayer" );
 //    dataloader->AddVariable( "muon_energy" );
 //    dataloader->AddVariable( "muon_energy_hit" );
 //    dataloader->AddVariable( "muon_nhits" );
 //    dataloader->AddVariable( "muon_time" );

     dataloader->AddTarget( "mc_energy" );

     Double_t regWeight     = 1.0;
     
     dataloader->AddRegressionTree( regTree, regWeight );
     
 //    dataloader->PrepareTrainingAndTestTree("", "nTrain_Regression=1500:nTest_Regression=1500:SplitMode=random:!V" );
     dataloader->PrepareTrainingAndTestTree("", "SplitMode=random:!V" );
     
     factory->BookMethod( dataloader, TMVA::Types::kMLP, "MLP", "!H:!V:VarTransform=Norm:NeuronType=sigmoid:NCycles=1:HiddenLayers=N,N-1,N-2:TrainingMethod=BP:ConvergenceImprove=1e-8:ConvergenceTests=15:EpochMonitoring:UseRegulator:LearningRate=0.005:DecayRate=0.05:TestRate=5" );

     factory->TrainAllMethods();
     
     factory->TestAllMethods();
     
     factory->EvaluateAllMethods();
     
     outputFile->Close();
     
     delete factory;
     delete dataloader;
     
     TMVA::TMVARegGui( outfileName );
 }
*/
