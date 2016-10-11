// system include files
#include <iostream>
#include <memory>
#include <fstream>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/global/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/ConfigurationDescriptions.h"
#include "FWCore/ParameterSet/interface/ParameterSetDescription.h"

#include "DataFormats/Scouting/interface/ScoutingVertex.h"
#include "DataFormats/Scouting/interface/ScoutingPFJet.h"
#include "DataFormats/Scouting/interface/ScoutingCaloJet.h"
#include "DataFormats/Scouting/interface/ScoutingParticle.h"
#include "DataFormats/Scouting/interface/ScoutingMuon.h"
#include "DataFormats/Math/interface/deltaR.h"
     
class TestScouting : public edm::global::EDProducer <>{
   public:
      TestScouting(const edm::ParameterSet&);
      virtual ~TestScouting();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual void produce(edm::StreamID sid, edm::Event&, const edm::EventSetup&) const override;

     edm::EDGetTokenT<ScoutingPFJetCollection> pfjets_;
     edm::EDGetTokenT<ScoutingMuonCollection> muons_;
     edm::EDGetTokenT<ScoutingParticleCollection> particles_;
     edm::EDGetTokenT<ScoutingVertexCollection> vertices_;

     std::ofstream *os;
};

TestScouting::TestScouting(const edm::ParameterSet& iConfig)
{
    pfjets_ = consumes<ScoutingPFJetCollection>(edm::InputTag("hltScoutingPFPacker", "", "HLT"));
    muons_ = consumes<ScoutingMuonCollection>(edm::InputTag("hltScoutingMuonPacker", "", "HLT"));
    particles_ = consumes<ScoutingParticleCollection>(edm::InputTag("hltScoutingPFPacker", "", "HLT"));
    vertices_ = consumes<ScoutingVertexCollection>(edm::InputTag("hltScoutingPFPacker", "", "HLT"));

    os = new std::ofstream("scouting.json");

    //begin JSON
}

TestScouting::~TestScouting() {
    os->close();
}

void TestScouting::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  edm::ParameterSetDescription desc;
  desc.add<edm::InputTag> ("pfjets",edm::InputTag("hltScoutingPFPacker","","HLT"));
  desc.add<edm::InputTag> ("muons",edm::InputTag("hltScoutingMuonPacker","","HLT"));
  desc.add<edm::InputTag> ("particles",edm::InputTag("hltScoutingPFPacker","","HLT"));
  desc.add<edm::InputTag> ("vertices",edm::InputTag("hltScoutingPFPacker","","HLT"));
  descriptions.add("testScouting",desc);
}

void TestScouting::produce(edm::StreamID sid, edm::Event& iEvent, const edm::EventSetup& iSetup) const
{
  using namespace edm;
  
  //get jets
  Handle<ScoutingPFJetCollection> pfjets;
  iEvent.getByToken(pfjets_, pfjets);
  
  //get particles
  Handle<ScoutingParticleCollection> particles;
  iEvent.getByToken(particles_, particles);
  
  //get vertices
  Handle<ScoutingVertexCollection> vertices;
  iEvent.getByToken(vertices_, vertices);

  //get muons
  Handle<ScoutingMuonCollection> muons;
  iEvent.getByToken(muons_, muons);

  *os << "{ \"event\" : { \"eventNum\" : " << iEvent.id().event() << ", ";

  *os << "\"muons\" : [";
  for(unsigned int i = 0; i < muons->size(); i++){
      ScoutingMuon mu = (*muons)[i];
      if (i) *os << ",";
      *os << " { \"pt\" : " << mu.pt() << ", \"eta\" : " << mu.eta() << ", \"phi\" : " << mu.phi() << ", \"m\" : " << mu.m() << " }";
  }
  *os << " ], ";

  *os << "\"jets\" : [";
  for(unsigned int i = 0; i < pfjets->size(); i++){
      ScoutingPFJet jet = (*pfjets)[i];
      if (i) *os << ",";
      *os << " { \"pt\" : " << jet.pt() << ", \"eta\" : " << jet.eta() << ", \"phi\" : " << jet.phi() << ", \"m\" : " << jet.m() << ", \"constituents\" : [";
      for(unsigned int j = 0; j < jet.constituents().size(); j++){
            ScoutingParticle c = (*particles)[jet.constituents()[j]];
            if (j) *os << ",";
            *os << " { \"pt\" : " << c.pt() << ", \"eta\" : " << c.eta() << ", \"phi\" : " << c.phi() << ", \"m\" : " << c.m() << " }";

      }
      *os << " ] }";
  }
  *os << " ] } }\n";
}

//define this as a plug-in
DEFINE_FWK_MODULE(TestScouting);
