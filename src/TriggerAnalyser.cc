/**\class TriggerAnalyser TriggerAnalyser.cc Analysis/Tools/src/TriggerAnalyser.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Roberval Walsh Bastos Rangel
//         Created:  Mon, 20 Oct 2014 14:24:08 GMT
//
//

// system include files
#include "boost/program_options.hpp"
#include "boost/algorithm/string.hpp"
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
// 
// user include files
#include "TString.h" 
#include "Analysis/Tools/interface/TriggerAnalyser.h"

//
// class declaration
//

using namespace analysis;
using namespace analysis::tools;

TriggerAnalyser::TriggerAnalyser()
{
}


TriggerAnalyser::TriggerAnalyser(int argc, char * argv[]) : BaseAnalyser(argc,argv)
{
   triggeranalysis_  = analysis_->triggerResults(config_->triggerResults());
   
   
   if ( config_->triggerObjectsDir() != "" )
   {
      // online jets
      for ( auto & obj : config_->triggerObjectsJets() )
         analysis_->addTree<TriggerObject> (obj,Form("%s/%s", config_->triggerObjectsDir().c_str(),obj.c_str()));
      // online b jets
      for ( auto & obj : config_->triggerObjectsBJets_ )
         analysis_->addTree<TriggerObject> (obj,Form("%s/%s", config_->triggerObjectsDir().c_str(),obj.c_str()));
      // online muons
      analysis_->addTree<TriggerObject> (config_->triggerObjectsL1Muons(),Form("%s/%s",config_->triggerObjectsDir().c_str(),config_->triggerObjectsL1Muons().c_str()));
      analysis_->addTree<TriggerObject> (config_->triggerObjectsL3Muons(),Form("%s/%s",config_->triggerObjectsDir().c_str(),config_->triggerObjectsL3Muons().c_str()));
   }
   
}

TriggerAnalyser::~TriggerAnalyser()
{
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
}


//
// member functions
//
// ------------ method called for each event  ------------


bool TriggerAnalyser::selectionTrigger() // Maybe not use this, use selectionHLT and selectionL1
{
   bool hlt = selectionHLT();
   bool l1  = selectionL1();
   
   return (hlt && l1);
   
}

bool TriggerAnalyser::selectionHLT()
{
   if ( config_->hltPath_ == "" ) return true;
   
   ++cutflow_;
   if ( ! analysis_->triggerResult(config_->hltPath_) ) return false;
   
   if ( std::string(h1_["cutflow"] -> GetXaxis()-> GetBinLabel(cutflow_+1)) == "" ) 
      h1_["cutflow"] -> GetXaxis()-> SetBinLabel(cutflow_+1,(config_->hltPath_).c_str());
   
   h1_["cutflow"] -> Fill(cutflow_,weight_);

   return true;
}

bool TriggerAnalyser::selectionL1()
{
   if ( config_->l1Seed_ == "" ) return true;
   
   ++cutflow_;
   if ( ! analysis_->triggerResult(config_->l1Seed_)  ) return false;
   
   if ( std::string(h1_["cutflow"] -> GetXaxis()-> GetBinLabel(cutflow_+1)) == "" ) 
      h1_["cutflow"] -> GetXaxis()-> SetBinLabel(cutflow_+1,(config_->l1Seed_).c_str());
   
   h1_["cutflow"] -> Fill(cutflow_,weight_);

   return true;
}


bool TriggerAnalyser::analysisWithTrigger()
{
   return triggeranalysis_;
}

