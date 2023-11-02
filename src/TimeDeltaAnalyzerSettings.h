#pragma once

#include <AnalyzerSettings.h>
#include <AnalyzerTypes.h>

class TimeDeltaAnalyzerSettings : public AnalyzerSettings
   {
   public:

      TimeDeltaAnalyzerSettings();
      virtual ~TimeDeltaAnalyzerSettings();

      virtual bool SetSettingsFromInterfaces();
      void UpdateInterfacesFromSettings();
      virtual void LoadSettings(const char* settings);
      virtual const char* SaveSettings();

      Channel mStartChannel;
      bool mStartEdgeIsPos;
      Channel mStopChannel;
      bool mStopEdgeIsPos;

   protected:

      std::auto_ptr<AnalyzerSettingInterfaceChannel> mStartChannelInterface;
      std::auto_ptr<AnalyzerSettingInterfaceBool> mStartEdgeInterface;
      std::auto_ptr<AnalyzerSettingInterfaceChannel> mStopChannelInterface;
      std::auto_ptr<AnalyzerSettingInterfaceBool> mStopEdgeInterface;
   };
