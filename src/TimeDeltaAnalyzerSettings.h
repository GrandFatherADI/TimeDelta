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

      Channel mRefChannel;
      bool mRefEdgeIsPos;
      Channel mTagChannel;
      bool mTagEdgeIsPos;

   protected:

      std::auto_ptr<AnalyzerSettingInterfaceChannel> mRefChannelInterface;
      std::auto_ptr<AnalyzerSettingInterfaceBool> mRefEdgeInterface;
      std::auto_ptr<AnalyzerSettingInterfaceChannel> mTagChannelInterface;
      std::auto_ptr<AnalyzerSettingInterfaceBool> mTagEdgeInterface;
   };
