#!/bin/bash

while [[ $# -gt 0 ]]; do
  case $1 in
    -d|--debug)
      DebugFlags="-g -D __DEBUG__"
      shift
      ;;
    -p|--profiler)
      ProfilerFlags="-pg"
      shift
      ;;
    -o|--optimize)
      OptimizeFlags="-O2"
      shift
      ;;
    -a|--assembly)
      AssemblyFlags="-g -Wa,-ahl"
      shift
      ;;
    -*|--*)
      echo "Unknown option $1"
      exit 1
      ;;
  esac
done

# Output path (relative to build sh file)
OutputPath="./build/linux64"

# Assets path (relative to output folder)
AssetsPath="../../assets"

# Include path (relative to output folder)
IncludePath="../../deps/include"

# Lib path (relative to output folder)
LibPath="../../deps/lib/linux64"

# Allow the war1 binary to run via clicking in file browser="-no-pie" https://bugzilla.gnome.org/show_bug.cgi?id=737849
CommonCompilerFlags="-std=c99 -Wall -x c $ProfilerFlags $OptimizeFlags $AssemblyFlags $DebugFlags -I $IncludePath"
CommonLinkerFlags="-L $LibPath -lglfw -lpthread -lm -ldl -no-pie"


# Create output path if doesn't exists
mkdir -p $OutputPath
cd $OutputPath

# Empty the build folder
rm -f *

# Compile the project
AllGCCArgs="$CommonCompilerFlags ../../src/war1.c ../../deps/include/glad/glad.c -o war1 $CommonLinkerFlags"
echo "gcc $AllGCCArgs"
if gcc $AllGCCArgs; then

# Copy assets

cp $AssetsPath/* ./

# Generate ctags
cd ../../
echo "Generating ctags - Start";
ctags --tag-relative=yes --extras=f --language-force=C --recurse=yes
echo "Generating ctags - Done";
fi
