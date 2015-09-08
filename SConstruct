#################################################################
# SConstruct template file.
#
# Use this file and the asociated SConscript files to compile and
# create the executable(s) of a C++ project. This file expects the 
# following folder hierarchy:
# 
# root       - SConstruct file
#   |_ h     - header files 
##  |_ src   - SConscript and source files, main.cpp also lives in here 
#   |_ build - object and executable files 
#
# Variables that need to be updted:
# TARGET = target file name
# INSTALL_PATH = Full path of a folder where the binaries will be installed
#
# Extra libraries should be included in the variable EXTRA_LIBS. The path
# for such libraries must be defined in EXTRA_LIBS_PATH
#
# By default it is compiled in debug mode. To compile in release mode
# execute as follows:
# $scons release=1
#
# To install the resultant binary files, execute as follows:
# $scons install
# 
################################################################## 
import os
# Get some environment variables
HOME = os.environ[ 'HOME' ]
INC = os.environ[ 'CPATH' ]

# Set the target name
TARGET = 'thirdEye'

# Set the installation directory
INSTALL_PATH = HOME + '/bin/'

###################################################################
# Set extra libs
###################################################################
MY_LIBS_PATH = HOME + '/lib/'

MY_EXTRA_LIBS_DBG = Split( """""" ) 

MY_EXTRA_LIBS     = Split( """""" ) 

OPENCV_LIBS       = Split( """opencv_core
                              opencv_highgui
                              opencv_imgproc""" )

EXTRA_LIBS_PATH = MY_LIBS_PATH 
EXTRA_LIBS_DBG  = MY_EXTRA_LIBS_DBG + OPENCV_LIBS
EXTRA_LIBS      = MY_EXTRA_LIBS     + OPENCV_LIBS

###################################################################
# Set extra include folders
###################################################################
MY_INCLUDES_PATH = INC

EXTRA_INC_PATH = MY_INCLUDES_PATH

###################################################################
# Compiler config
###################################################################
# Set the compiler
CXX = 'g++'

# Compile using C++11
STD_VER = '-std=c++11'

# Set position independed code, required for linking libraries
THIS_THING = "-fPIC"

# Set debug level. 
# Produce information for use by GDB. The '3' provides extra info
DEBUG_LEVEL = ' -ggdb3'

# Set warning level. Get lots of warnings
WARNING_LEVEL = ' -Wextra -Wall -pedantic '

# Set optimization properites
OPT_LEVEL = ' -O2'

###################################################################
# Define compilation environments
#################################################################
env = Environment( CC = CXX, CPPPATH = EXTRA_INC_PATH, LIBPATH = EXTRA_LIBS_PATH )
release = ARGUMENTS.get( 'release', 0 )

if int(release):
   env.Append( CPPFLAGS = THIS_THING + OPT_LEVEL + WARNING_LEVEL + STD_VER )
   env.Append( LIBS = EXTRA_LIBS )
else:
   env.Append( CPPFLAGS = THIS_THING + DEBUG_LEVEL + WARNING_LEVEL + STD_VER )
   env.Append( LIBS = EXTRA_LIBS_DBG )
   TARGET = TARGET + 'd'

# Print used flags ans libraries (is this redundant?)
print "flags:", env.subst( '$CPPFLAGS' )
print "libs:", env[ 'LIBS' ]

##################################################################
# Compile using lots of CPU cores
##################################################################
num_cpu = int( os.environ.get( 'NUM_CPU', 8 ) )
env.SetOption( 'num_jobs', num_cpu )
#print "running with -j", env.GetOption( 'num_jobs' )


###################################################################
# Auxiliar SConscript files
###################################################################
sources_path =  'src/SConscript'
build_path = 'build'

SConscript( sources_path, exports='env TARGET INSTALL_PATH', variant_dir = build_path, duplicate=0 )

