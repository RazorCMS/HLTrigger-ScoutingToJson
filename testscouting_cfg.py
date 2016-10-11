import FWCore.ParameterSet.Config as cms

process = cms.Process("UNPACKING")
process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1000000) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        "/store/data/Run2015D/ScoutingPFHT/RAW/v1/000/260/627/00000/E8BE8730-CE81-E511-A7D5-02163E011E02.root"
    )
)

process.testScouting = cms.EDProducer('TestScouting',
        muons = cms.InputTag("hltScoutingMuonPacker"),
        pfjets = cms.InputTag("hltScoutingPFPacker"),
        particles = cms.InputTag("hltScoutingPFPacker"),
        vertices = cms.InputTag("hltScoutingPFPacker")
)

process.out = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string('scoutingtest.root'),
    outputCommands = cms.untracked.vstring('drop *'),
)
  
process.p = cms.Path(process.testScouting)

process.e = cms.EndPath(process.out)
