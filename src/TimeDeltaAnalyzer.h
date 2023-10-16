#pragma once

#include <Analyzer.h>
#include "TimeDeltaAnalyzerResults.h"
#include "TimeDeltaSimulationDataGenerator.h"

class TimeDeltaAnalyzerSettings;

class ANALYZER_EXPORT TimeDeltaAnalyzer : public Analyzer2
   {
   public:
      TimeDeltaAnalyzer();
      virtual ~TimeDeltaAnalyzer();

      virtual void SetupResults();
      virtual void WorkerThread();

      virtual U32 GenerateSimulationData(U64 newest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channels);
      virtual U32 GetMinimumSampleRateHz();

      virtual const char* GetAnalyzerName() const;
      virtual bool NeedsRerun();

   protected: //vars
      std::auto_ptr< TimeDeltaAnalyzerSettings > mSettings;
      std::auto_ptr< TimeDeltaAnalyzerResults > mResults;

      TimeDeltaSimulationDataGenerator mSimulationDataGenerator;
      bool mSimulationInitilized;
   };

extern "C" ANALYZER_EXPORT const char* __cdecl GetAnalyzerName();
extern "C" ANALYZER_EXPORT Analyzer* __cdecl CreateAnalyzer();
extern "C" ANALYZER_EXPORT void __cdecl DestroyAnalyzer(Analyzer* analyzer);
