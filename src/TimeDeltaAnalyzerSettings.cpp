#include "TimeDeltaAnalyzerSettings.h"
#include <AnalyzerHelpers.h>

constexpr int kSettingsVer = 1;

/* Settings version history

1: Initial release

*/

TimeDeltaAnalyzerSettings::TimeDeltaAnalyzerSettings():
   mRefChannel(UNDEFINED_CHANNEL),
   mRefEdgeIsPos(true),
   mTagChannel(UNDEFINED_CHANNEL),
   mTagEdgeIsPos(true)
   {
   mRefChannelInterface.reset(new AnalyzerSettingInterfaceChannel());
   mRefChannelInterface->SetTitleAndTooltip
      ("Reference", "Measure time delta from this channel to target channel");
   mRefEdgeInterface.reset(new AnalyzerSettingInterfaceBool());
   mRefEdgeInterface->SetTitleAndTooltip
      ("Pos Edge", "Reference rising edge is active");
   mTagChannelInterface.reset(new AnalyzerSettingInterfaceChannel());
   mTagChannelInterface->SetTitleAndTooltip
      ("Target", "Measure time delta from this channel to target channel");
   mTagEdgeInterface.reset(new AnalyzerSettingInterfaceBool());
   mTagEdgeInterface->SetTitleAndTooltip
      ("Pos Edge", "Target rising edge is active");

   mRefChannelInterface->SetChannel(mRefChannel);
   mTagChannelInterface->SetChannel(mTagChannel);

   AddInterface(mRefChannelInterface.get());
   AddInterface(mRefEdgeInterface.get());
   AddInterface(mTagChannelInterface.get());
   AddInterface(mTagEdgeInterface.get());

   AddExportOption(0, "Export as text/csv file");
   AddExportExtension(0, "text", "txt");
   AddExportExtension(0, "csv", "csv");

   ClearChannels();
   AddChannel(mRefChannel, "Ref", false);
   AddChannel(mTagChannel, "Tag", false);
   }

TimeDeltaAnalyzerSettings::~TimeDeltaAnalyzerSettings()
   {
   }

bool TimeDeltaAnalyzerSettings::SetSettingsFromInterfaces()
   {
   mRefChannel = mRefChannelInterface->GetChannel();
   mRefEdgeIsPos = mRefEdgeInterface->GetValue();
   mTagChannel = mTagChannelInterface->GetChannel();
   mTagEdgeIsPos = mTagEdgeInterface->GetValue();

   ClearChannels();
   AddChannel(mRefChannel, "Time Delta", true);

   return true;
   }

void TimeDeltaAnalyzerSettings::UpdateInterfacesFromSettings()
   {
   mRefChannelInterface->SetChannel(mRefChannel);
   mRefEdgeInterface->SetValue(mRefEdgeIsPos);
   mTagChannelInterface->SetChannel(mTagChannel);
   mTagEdgeInterface->SetValue(mTagEdgeIsPos);
   }

void TimeDeltaAnalyzerSettings::LoadSettings(const char* settings)
   {
   int settingsVer;
   SimpleArchive text_archive;
   text_archive.SetString(settings);

   text_archive >> settingsVer;
   text_archive >> mRefChannel;
   text_archive >> mRefEdgeIsPos;
   text_archive >> mTagChannel;
   text_archive >> mTagEdgeIsPos;

   ClearChannels();
   AddChannel(mRefChannel, "Time Delta", true);
   AddChannel(mTagChannel, "Time Delta", true);

   UpdateInterfacesFromSettings();
   }

const char* TimeDeltaAnalyzerSettings::SaveSettings()
   {
   SimpleArchive text_archive;

   text_archive << kSettingsVer;
   text_archive << mRefChannel;
   text_archive << mRefEdgeIsPos;
   text_archive << mTagChannel;
   text_archive << mTagEdgeIsPos;

   return SetReturnString(text_archive.GetString());
   }
