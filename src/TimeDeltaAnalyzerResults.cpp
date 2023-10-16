#include "TimeDeltaAnalyzerResults.h"
#include <AnalyzerHelpers.h>
#include "TimeDeltaAnalyzer.h"
#include "TimeDeltaAnalyzerSettings.h"
#include <iostream>
#include <fstream>
#include <sstream>

TimeDeltaAnalyzerResults::TimeDeltaAnalyzerResults
   (TimeDeltaAnalyzer* analyzer, TimeDeltaAnalyzerSettings* settings): 
   AnalyzerResults(),
   mSettings(settings),
   mAnalyzer(analyzer)
   {
   }

TimeDeltaAnalyzerResults::~TimeDeltaAnalyzerResults()
   {
   }

void TimeDeltaAnalyzerResults::GenerateBubbleText
   (U64 frame_index, Channel& channel, DisplayBase display_base)
   {
   ClearResultStrings();
   Frame frame = GetFrame(frame_index);
   std::stringstream numStrm;

   numStrm << (float(frame.mData1) / frame.mData2);
   AddResultString(numStrm.str().c_str());
   }

void TimeDeltaAnalyzerResults::GenerateExportFile
   (const char* file, DisplayBase display_base, U32 export_type_user_id)
   {
   std::ofstream file_stream(file, std::ios::out);

   U64 trigger_sample = mAnalyzer->GetTriggerSample();
   U32 sample_rate = mAnalyzer->GetSampleRate();

   file_stream << "Time [s],Value" << std::endl;

   U64 num_frames = GetNumFrames();
   for (U32 i = 0; i < num_frames; i++)
      {
      Frame frame = GetFrame(i);

      char time_str[128];
      AnalyzerHelpers::GetTimeString(frame.mStartingSampleInclusive, trigger_sample, sample_rate, time_str, 128);

      char number_str[128];
      AnalyzerHelpers::GetNumberString(frame.mData1, display_base, 8, number_str, 128);

      file_stream << time_str << "," << number_str << std::endl;

      if (UpdateExportProgressAndCheckForCancel(i, num_frames) == true)
         {
         file_stream.close();
         return;
         }
      }

   file_stream.close();
   }

void TimeDeltaAnalyzerResults::GenerateFrameTabularText(U64 frame_index, DisplayBase display_base)
   {
   #ifdef SUPPORTS_PROTOCOL_SEARCH
   Frame frame = GetFrame(frame_index);
   ClearTabularText();

   char number_str[128];
   AnalyzerHelpers::GetNumberString(frame.mData1, display_base, 8, number_str, 128);
   AddTabularText(number_str);
   #endif
   }

void TimeDeltaAnalyzerResults::GeneratePacketTabularText(U64 packet_id, DisplayBase display_base)
   {
   //not supported

   }

void TimeDeltaAnalyzerResults::GenerateTransactionTabularText(U64 transaction_id, DisplayBase display_base)
   {
   //not supported
   }