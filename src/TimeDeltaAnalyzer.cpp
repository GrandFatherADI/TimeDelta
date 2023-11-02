#include "TimeDeltaAnalyzer.h"
#include "TimeDeltaAnalyzerSettings.h"
#include <AnalyzerChannelData.h>

TimeDeltaAnalyzer::TimeDeltaAnalyzer()
   : Analyzer2(),
   mSettings(new TimeDeltaAnalyzerSettings()),
   mSimulationInitilized(false)
   {
   SetAnalyzerSettings(mSettings.get());
   UseFrameV2();
   }

TimeDeltaAnalyzer::~TimeDeltaAnalyzer()
   {
   KillThread();
   }

void TimeDeltaAnalyzer::SetupResults()
   {
   mResults.reset(new TimeDeltaAnalyzerResults(this, mSettings.get()));
   SetAnalyzerResults(mResults.get());
   mResults->AddChannelBubblesWillAppearOn(mSettings->mStartChannel);
   }

void TimeDeltaAnalyzer::WorkerThread()
   {
   AnalyzerChannelData *startData = GetAnalyzerChannelData(mSettings->mStartChannel);
   AnalyzerChannelData *stopData = GetAnalyzerChannelData(mSettings->mStopChannel);
   uint32_t sampleRate = GetSampleRate();

   // Start on the first edge
   startData->AdvanceToNextEdge();
   stopData->AdvanceToNextEdge();

   // Move to the next edge if this one is boring
   if (bool(startData->GetBitState()) ^ mSettings->mStartEdgeIsPos)
      startData->AdvanceToNextEdge();

   if (bool(stopData->GetBitState()) ^ mSettings->mStopEdgeIsPos)
      stopData->AdvanceToNextEdge();

   while (true)
      {
      // Both start and stop channels are at interesting edges.
      // We are looking for the nearest start edge before the current
      // stop edge.

      uint64_t startEdge = startData->GetSampleNumber();
      uint64_t stopEdge = stopData->GetSampleNumber();

      if (startEdge > stopEdge)
         {
         // Current start edge is after current stop edge so find the
         // next interesting stop edge
         stopData->AdvanceToNextEdge();
         stopData->AdvanceToNextEdge();
         continue;
         }

      uint64_t pendingStartEdge = startEdge;

      // Force a small read ahead on both channels (we hope)
      stopData->WouldAdvancingToAbsPositionCauseTransition(stopEdge + 10);
      startData->WouldAdvancingToAbsPositionCauseTransition(stopEdge + 10);

      while (startEdge < stopEdge)
         {
         pendingStartEdge = startEdge;

         if (!startData->WouldAdvancingToAbsPositionCauseTransition(stopEdge + 1))
            break;

         startData->AdvanceToNextEdge();
         startData->AdvanceToNextEdge();
         startEdge = startData->GetSampleNumber();
         }

      // Pending values are the samples we want for the delta
      mResults->AddMarker
         (pendingStartEdge, AnalyzerResults::Start, mSettings->mStartChannel);
      mResults->AddMarker
         (stopEdge, AnalyzerResults::Stop, mSettings->mStopChannel);

      uint64_t end = stopEdge;

      if (pendingStartEdge == end)
         ++end;

      //we have a delta to save
      Frame frame;
      frame.mData1 = stopEdge - pendingStartEdge;
      frame.mData2 = sampleRate;
      frame.mFlags = 0;
      frame.mStartingSampleInclusive = pendingStartEdge;
      frame.mEndingSampleInclusive = end;

      mResults->AddFrame(frame);

      // Now do it for Frame V2
      FrameV2 frameV2;

      mResults->AddFrameV2
         (
         frameV2, "Delta", 
         frame.mStartingSampleInclusive, frame.mEndingSampleInclusive
         );

      mResults->CommitResults();
      ReportProgress(frame.mEndingSampleInclusive);

      stopData->AdvanceToNextEdge();
      stopData->AdvanceToNextEdge();

      if (pendingStartEdge == startEdge)
         {
         startData->AdvanceToNextEdge();
         startData->AdvanceToNextEdge();
         }

      pendingStartEdge = 0;
      }
   }

bool TimeDeltaAnalyzer::NeedsRerun()
   {
   return false;
   }

U32 TimeDeltaAnalyzer::GenerateSimulationData
   (
   U64 minimum_sample_index, 
   U32 device_sample_rate, 
   SimulationChannelDescriptor** simulation_channels
   )
   {
   if (mSimulationInitilized == false)
      {
      mSimulationDataGenerator.Initialize
         (GetSimulationSampleRate(), mSettings.get());
      mSimulationInitilized = true;
      }

   return mSimulationDataGenerator.GenerateSimulationData
      (minimum_sample_index, device_sample_rate, simulation_channels);
   }

U32 TimeDeltaAnalyzer::GetMinimumSampleRateHz()
   {
   // No minimum really, but this is best practise.
   return 25000;
   }

const char* TimeDeltaAnalyzer::GetAnalyzerName() const
   {
   return ::GetAnalyzerName();
   }

const char* GetAnalyzerName()
   {
   return "Time Delta";
   }

Analyzer* CreateAnalyzer()
   {
   return new TimeDeltaAnalyzer();
   }

void DestroyAnalyzer(Analyzer* analyzer)
   {
   delete analyzer;
   }