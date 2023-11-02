#pragma once
#include <SimulationChannelDescriptor.h>
#include <string>

class TimeDeltaAnalyzerSettings;

class TimeDeltaSimulationDataGenerator
   {
   public:
      TimeDeltaSimulationDataGenerator();
      ~TimeDeltaSimulationDataGenerator();

      void Initialize
         (U32 simulation_sample_rate, TimeDeltaAnalyzerSettings* settings);
      U32 GenerateSimulationData
         (
         U64 newest_sample_requested, 
         U32 sample_rate, 
         SimulationChannelDescriptor** simulation_channel
         );

   protected:
      TimeDeltaAnalyzerSettings* mSettings;
      U32 mSimulationSampleRateHz;

   protected:
      void CreateDeltaPair(SimulationChannelDescriptor &scd);

      SimulationChannelDescriptorGroup mChannels;
      SimulationChannelDescriptor *mStartData;
      SimulationChannelDescriptor *mStopData;
   };
