#include "TimeDeltaSimulationDataGenerator.h"
#include "TimeDeltaAnalyzerSettings.h"

#include <AnalyzerHelpers.h>

TimeDeltaSimulationDataGenerator::TimeDeltaSimulationDataGenerator()
   {
   }

TimeDeltaSimulationDataGenerator::~TimeDeltaSimulationDataGenerator()
   {
   }

void TimeDeltaSimulationDataGenerator::Initialize
   (U32 simulation_sample_rate, TimeDeltaAnalyzerSettings* settings)
   {
   mSimulationSampleRateHz = simulation_sample_rate;
   mSettings = settings;

   mStartData = mChannels.Add(settings->mStartChannel, simulation_sample_rate, BIT_LOW);
   mStopData = mChannels.Add(settings->mStopChannel, simulation_sample_rate, BIT_LOW);
   }

U32 TimeDeltaSimulationDataGenerator::GenerateSimulationData
   (
   U64 largest_sample_requested, 
   U32 sample_rate, 
   SimulationChannelDescriptor** simulation_channel
   )
   {
   U64 largestSample = 
      AnalyzerHelpers::AdjustSimulationTargetSample
         (largest_sample_requested, sample_rate, mSimulationSampleRateHz);

   while (mStartData->GetCurrentSampleNumber() < largestSample)
      CreateDeltaPair(*mStartData);

   while (mStopData->GetCurrentSampleNumber() < largestSample)
      CreateDeltaPair(*mStopData);

   *simulation_channel = mChannels.GetArray();
   return mChannels.GetCount();
   }

void TimeDeltaSimulationDataGenerator::CreateDeltaPair
   (SimulationChannelDescriptor &scd)
   {
   scd.TransitionIfNeeded(BIT_LOW);
   scd.Advance(10);
   scd.Transition();
   scd.Advance(2000);
   scd.Transition();
   }
