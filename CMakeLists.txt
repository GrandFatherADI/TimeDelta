cmake_minimum_required (VERSION 3.13)

project(TimeDeltaAnalyzer)

add_definitions( -DLOGIC2 )

# enable generation of compile_commands.json, helpful for IDEs to locate include files.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# custom CMake Modules are located in the cmake directory.
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

include(ExternalAnalyzerSDK)

set(SOURCES 
src/TimeDeltaAnalyzer.cpp
src/TimeDeltaAnalyzer.h
src/TimeDeltaAnalyzerResults.cpp
src/TimeDeltaAnalyzerResults.h
src/TimeDeltaAnalyzerSettings.cpp
src/TimeDeltaAnalyzerSettings.h
src/TimeDeltaSimulationDataGenerator.cpp
src/TimeDeltaSimulationDataGenerator.h
)

add_analyzer_plugin(${PROJECT_NAME} SOURCES ${SOURCES})
