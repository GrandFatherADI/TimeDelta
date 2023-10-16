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
   mResults->AddChannelBubblesWillAppearOn(mSettings->mRefChannel);
   }

void TimeDeltaAnalyzer::WorkerThread()
   {
   AnalyzerChannelData *refData = GetAnalyzerChannelData(mSettings->mRefChannel);
   AnalyzerChannelData *tagData = GetAnalyzerChannelData(mSettings->mTagChannel);
   uint32_t sampleRate = GetSampleRate();

   // Start on the first edge
   refData->AdvanceToNextEdge();
   tagData->AdvanceToNextEdge();

   // Move to the next edge if this one is boring
   if (bool(refData->GetBitState()) ^ mSettings->mRefEdgeIsPos)
      refData->AdvanceToNextEdge();

   if (bool(tagData->GetBitState()) ^ mSettings->mTagEdgeIsPos)
      tagData->AdvanceToNextEdge();

   while (true)
      {
      // Both reference and target channels are at interesting edges.
      // We are looking for the nearest reference edge before the current
      // target edge.

      uint64_t refEdge = refData->GetSampleNumber();
      uint64_t tagEdge = tagData->GetSampleNumber();

      if (refEdge > tagEdge)
         {
         // Current reference edge is after current target edge so find the
         // next interesting target edge
         tagData->AdvanceToNextEdge();
         tagData->AdvanceToNextEdge();
         continue;
         }

      uint64_t pendingRefEdge = refEdge;

      // Force a small read ahead on both channels (we hope)
      tagData->WouldAdvancingToAbsPositionCauseTransition(tagEdge + 10);
      refData->WouldAdvancingToAbsPositionCauseTransition(tagEdge + 10);

      while (refEdge < tagEdge)
         {
         pendingRefEdge = refEdge;

         if (!refData->WouldAdvancingToAbsPositionCauseTransition(tagEdge + 1))
            break;

         refData->AdvanceToNextEdge();
         refData->AdvanceToNextEdge();
         refEdge = refData->GetSampleNumber();
         }

      // Pending values are the samples we want for the delta
      mResults->AddMarker
         (pendingRefEdge, AnalyzerResults::Start, mSettings->mRefChannel);
      mResults->AddMarker
         (tagEdge, AnalyzerResults::Stop, mSettings->mTagChannel);

      uint64_t end = tagEdge;

      if (pendingRefEdge == end)
         ++end;

      //we have a delta to save
      Frame frame;
      frame.mData1 = tagEdge - pendingRefEdge;
      frame.mData2 = sampleRate;
      frame.mFlags = 0;
      frame.mStartingSampleInclusive = pendingRefEdge;
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

      tagData->AdvanceToNextEdge();
      tagData->AdvanceToNextEdge();

      if (pendingRefEdge == refEdge)
         {
         refData->AdvanceToNextEdge();
         refData->AdvanceToNextEdge();
         }

      pendingRefEdge = 0;
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