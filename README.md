# HLTrigger-ScoutingToJson
This CMSSW plugin writes scouting event data in JSON format.  Use as follows:

```
cmsrel CMSSW_8_0_1
cd CMSSW_8_0_1/src/
cmsenv
git clone https://github.com/RazorCMS/HLTrigger-ScoutingToJson.git HLTrigger/ScoutingToJson
cd HLTrigger/ScoutingToJson/
scram b
cmsRun testscouting_cfg.py
```
