##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=PokeFinder
ConfigurationName      :=Debug
WorkspacePath          :=D:/Programming/PokeFinder
ProjectPath            :=D:/Programming/PokeFinder
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=1
Date                   :=24/07/2017
CodeLitePath           :="D:/Program Files/CodeLite"
LinkerName             :="D:/Program Files/TDMGCC/bin/g++.exe"
SharedObjectLinkerName :="D:/Program Files/TDMGCC/bin/g++.exe" -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=$(IntermediateDirectory)/$(ProjectName)
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="PokeFinder.txt"
PCHCompileFlags        :=
MakeDirCommand         :=makedir
RcCmpOptions           := 
RcCompilerName         :="D:/Program Files/TDMGCC/bin/windres.exe"
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                := $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := "D:/Program Files/TDMGCC/bin/ar.exe" rcu
CXX      := "D:/Program Files/TDMGCC/bin/g++.exe"
CC       := "D:/Program Files/TDMGCC/bin/gcc.exe"
CXXFLAGS := -std=c++11 -g -O0 -Wall $(Preprocessors)
CFLAGS   :=  -g -O0 -Wall $(Preprocessors)
ASFLAGS  := 
AS       := "D:/Program Files/TDMGCC/bin/as.exe"


##
## User defined environment variables
##
CodeLiteDir:=D:\Program Files\CodeLite
WXWIN:=D:\Programming\wxWidgets-3.1.0
WXCFG:=gcc_dll\mswu
PATH:=D:\Programming\wxWidgets-3.1.0\lib\gcc_dll;$PATH
Objects0=$(IntermediateDirectory)/main.cpp$(ObjectSuffix) $(IntermediateDirectory)/Objects_LCRNG.cpp$(ObjectSuffix) $(IntermediateDirectory)/Objects_MTRNG.cpp$(ObjectSuffix) $(IntermediateDirectory)/Objects_NatureLock.cpp$(ObjectSuffix) $(IntermediateDirectory)/Objects_EncounterSlot.cpp$(ObjectSuffix) $(IntermediateDirectory)/Forms_MainFrame.cpp$(ObjectSuffix) $(IntermediateDirectory)/Forms_wxcrafter.cpp$(ObjectSuffix) $(IntermediateDirectory)/Forms_wxcrafter_bitmaps.cpp$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

MakeIntermediateDirs:
	@$(MakeDirCommand) "./Debug"


$(IntermediateDirectory)/.d:
	@$(MakeDirCommand) "./Debug"

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/main.cpp$(ObjectSuffix): main.cpp $(IntermediateDirectory)/main.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "D:/Programming/PokeFinder/main.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/main.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/main.cpp$(DependSuffix): main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/main.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/main.cpp$(DependSuffix) -MM main.cpp

$(IntermediateDirectory)/main.cpp$(PreprocessSuffix): main.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/main.cpp$(PreprocessSuffix) main.cpp

$(IntermediateDirectory)/Objects_LCRNG.cpp$(ObjectSuffix): Objects/LCRNG.cpp $(IntermediateDirectory)/Objects_LCRNG.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "D:/Programming/PokeFinder/Objects/LCRNG.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Objects_LCRNG.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Objects_LCRNG.cpp$(DependSuffix): Objects/LCRNG.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Objects_LCRNG.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Objects_LCRNG.cpp$(DependSuffix) -MM Objects/LCRNG.cpp

$(IntermediateDirectory)/Objects_LCRNG.cpp$(PreprocessSuffix): Objects/LCRNG.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Objects_LCRNG.cpp$(PreprocessSuffix) Objects/LCRNG.cpp

$(IntermediateDirectory)/Objects_MTRNG.cpp$(ObjectSuffix): Objects/MTRNG.cpp $(IntermediateDirectory)/Objects_MTRNG.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "D:/Programming/PokeFinder/Objects/MTRNG.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Objects_MTRNG.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Objects_MTRNG.cpp$(DependSuffix): Objects/MTRNG.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Objects_MTRNG.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Objects_MTRNG.cpp$(DependSuffix) -MM Objects/MTRNG.cpp

$(IntermediateDirectory)/Objects_MTRNG.cpp$(PreprocessSuffix): Objects/MTRNG.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Objects_MTRNG.cpp$(PreprocessSuffix) Objects/MTRNG.cpp

$(IntermediateDirectory)/Objects_NatureLock.cpp$(ObjectSuffix): Objects/NatureLock.cpp $(IntermediateDirectory)/Objects_NatureLock.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "D:/Programming/PokeFinder/Objects/NatureLock.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Objects_NatureLock.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Objects_NatureLock.cpp$(DependSuffix): Objects/NatureLock.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Objects_NatureLock.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Objects_NatureLock.cpp$(DependSuffix) -MM Objects/NatureLock.cpp

$(IntermediateDirectory)/Objects_NatureLock.cpp$(PreprocessSuffix): Objects/NatureLock.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Objects_NatureLock.cpp$(PreprocessSuffix) Objects/NatureLock.cpp

$(IntermediateDirectory)/Objects_EncounterSlot.cpp$(ObjectSuffix): Objects/EncounterSlot.cpp $(IntermediateDirectory)/Objects_EncounterSlot.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "D:/Programming/PokeFinder/Objects/EncounterSlot.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Objects_EncounterSlot.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Objects_EncounterSlot.cpp$(DependSuffix): Objects/EncounterSlot.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Objects_EncounterSlot.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Objects_EncounterSlot.cpp$(DependSuffix) -MM Objects/EncounterSlot.cpp

$(IntermediateDirectory)/Objects_EncounterSlot.cpp$(PreprocessSuffix): Objects/EncounterSlot.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Objects_EncounterSlot.cpp$(PreprocessSuffix) Objects/EncounterSlot.cpp

$(IntermediateDirectory)/Forms_MainFrame.cpp$(ObjectSuffix): Forms/MainFrame.cpp $(IntermediateDirectory)/Forms_MainFrame.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "D:/Programming/PokeFinder/Forms/MainFrame.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Forms_MainFrame.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Forms_MainFrame.cpp$(DependSuffix): Forms/MainFrame.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Forms_MainFrame.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Forms_MainFrame.cpp$(DependSuffix) -MM Forms/MainFrame.cpp

$(IntermediateDirectory)/Forms_MainFrame.cpp$(PreprocessSuffix): Forms/MainFrame.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Forms_MainFrame.cpp$(PreprocessSuffix) Forms/MainFrame.cpp

$(IntermediateDirectory)/Forms_wxcrafter.cpp$(ObjectSuffix): Forms/wxcrafter.cpp $(IntermediateDirectory)/Forms_wxcrafter.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "D:/Programming/PokeFinder/Forms/wxcrafter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Forms_wxcrafter.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Forms_wxcrafter.cpp$(DependSuffix): Forms/wxcrafter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Forms_wxcrafter.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Forms_wxcrafter.cpp$(DependSuffix) -MM Forms/wxcrafter.cpp

$(IntermediateDirectory)/Forms_wxcrafter.cpp$(PreprocessSuffix): Forms/wxcrafter.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Forms_wxcrafter.cpp$(PreprocessSuffix) Forms/wxcrafter.cpp

$(IntermediateDirectory)/Forms_wxcrafter_bitmaps.cpp$(ObjectSuffix): Forms/wxcrafter_bitmaps.cpp $(IntermediateDirectory)/Forms_wxcrafter_bitmaps.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "D:/Programming/PokeFinder/Forms/wxcrafter_bitmaps.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Forms_wxcrafter_bitmaps.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Forms_wxcrafter_bitmaps.cpp$(DependSuffix): Forms/wxcrafter_bitmaps.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Forms_wxcrafter_bitmaps.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Forms_wxcrafter_bitmaps.cpp$(DependSuffix) -MM Forms/wxcrafter_bitmaps.cpp

$(IntermediateDirectory)/Forms_wxcrafter_bitmaps.cpp$(PreprocessSuffix): Forms/wxcrafter_bitmaps.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Forms_wxcrafter_bitmaps.cpp$(PreprocessSuffix) Forms/wxcrafter_bitmaps.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


