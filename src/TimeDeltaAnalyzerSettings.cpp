#include "TimeDeltaAnalyzerSettings.h"
#include <AnalyzerHelpers.h>

constexpr int kSettingsVer = 1;

/* Settings version history

1: Initial release

*/

TimeDeltaAnalyzerSettings::TimeDeltaAnalyzerSettings():
   mStartChannel(UNDEFINED_CHANNEL),
   mStartEdgeIsPos(true),
   mStopChannel(UNDEFINED_CHANNEL),
   mStopEdgeIsPos(true)
   {
   mStartChannelInterface.reset(new AnalyzerSettingInterfaceChannel());
   mStartChannelInterface->SetTitleAndTooltip
      ("Start", "Measure time delta from this channel to stop channel");
   mStartEdgeInterface.reset(new AnalyzerSettingInterfaceBool());
   mStartEdgeInterface->SetTitleAndTooltip
      ("Start Pos Edge", "Start rising edge is active");
   mStopChannelInterface.reset(new AnalyzerSettingInterfaceChannel());
   mStopChannelInterface->SetTitleAndTooltip
      ("Stop", "Measure time delta from start channel to this channel");
   mStopEdgeInterface.reset(new AnalyzerSettingInterfaceBool());
   mStopEdgeInterface->SetTitleAndTooltip
      ("Stop Pos Edge", "Stop rising edge is active");

   mStartChannelInterface->SetChannel(mStartChannel);
   mStopChannelInterface->SetChannel(mStopChannel);

   AddInterface(mStartChannelInterface.get());
   AddInterface(mStartEdgeInterface.get());
   AddInterface(mStopChannelInterface.get());
   AddInterface(mStopEdgeInterface.get());

   AddExportOption(0, "Export as text/csv file");
   AddExportExtension(0, "text", "txt");
   AddExportExtension(0, "csv", "csv");

   ClearChannels();
   AddChannel(mStartChannel, "Start", false);
   AddChannel(mStopChannel, "Stop", false);
   }

TimeDeltaAnalyzerSettings::~TimeDeltaAnalyzerSettings()
   {
   }

bool TimeDeltaAnalyzerSettings::SetSettingsFromInterfaces()
   {
   mStartChannel = mStartChannelInterface->GetChannel();
   mStartEdgeIsPos = mStartEdgeInterface->GetValue();
   mStopChannel = mStopChannelInterface->GetChannel();
   mStopEdgeIsPos = mStopEdgeInterface->GetValue();

   ClearChannels();
   AddChannel(mStartChannel, "Time Delta", true);

   return true;
   }

void TimeDeltaAnalyzerSettings::UpdateInterfacesFromSettings()
   {
   mStartChannelInterface->SetChannel(mStartChannel);
   mStartEdgeInterface->SetValue(mStartEdgeIsPos);
   mStopChannelInterface->SetChannel(mStopChannel);
   mStopEdgeInterface->SetValue(mStopEdgeIsPos);
   }

void TimeDeltaAnalyzerSettings::LoadSettings(const char* settings)
   {
   int settingsVer;
   SimpleArchive text_archive;
   text_archive.SetString(settings);

   text_archive >> settingsVer;
   text_archive >> mStartChannel;
   text_archive >> mStartEdgeIsPos;
   text_archive >> mStopChannel;
   text_archive >> mStopEdgeIsPos;

   ClearChannels();
   AddChannel(mStartChannel, "Time Delta", true);
   AddChannel(mStopChannel, "Time Delta", true);

   UpdateInterfacesFromSettings();
   }

const char* TimeDeltaAnalyzerSettings::SaveSettings()
   {
   SimpleArchive text_archive;

   text_archive << kSettingsVer;
   text_archive << mStartChannel;
   text_archive << mStartEdgeIsPos;
   text_archive << mStopChannel;
   text_archive << mStopEdgeIsPos;

   return SetReturnString(text_archive.GetString());
   }
