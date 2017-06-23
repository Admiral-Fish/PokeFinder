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
Date                   :=23/06/2017
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
Objects0=$(IntermediateDirectory)/main.cpp$(ObjectSuffix) $(IntermediateDirectory)/Objects_LCRNG.cpp$(ObjectSuffix) $(IntermediateDirectory)/Objects_MTRNG.cpp$(ObjectSuffix) 



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


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


