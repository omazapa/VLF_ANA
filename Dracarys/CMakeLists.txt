#requires the same version of cmake that is using ROOT
#Author Omar.Zapata@cern.ch 2017 CMS-UdeA

cmake_minimum_required(VERSION 3.7 FATAL_ERROR)
project(HarryPlotter)

list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})

find_package(ROOT REQUIRED COMPONENTS RIO Gui Core Imt RIO Net Hist Graf Graf3d Gpad Tree TreePlayer Rint Postscript Matrix Physics MathCore Thread MultiProc )

#---Define useful ROOT functions and macros (e.g. ROOT_GENERATE_DICTIONARY)
include(${ROOT_USE_FILE})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/interface)
ROOT_GENERATE_DICTIONARY(G__HarryPlotter Plotter.h LINKDEF ${CMAKE_CURRENT_SOURCE_DIR}/interface/LinkDef.h)

#---Create a shared library with geneated dictionary
add_library(HarryPlotter SHARED src/Plotter.cxx G__HarryPlotter.cxx)
target_link_libraries(HarryPlotter ${ROOT_LIBRARIES})

