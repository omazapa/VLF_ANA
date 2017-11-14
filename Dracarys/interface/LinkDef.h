// @(#)root/mpi:$Id: LinkDef.h  -- :: $
// Author: Omar Zapata   3/11/2017

#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ nestedclass;
#pragma link C++ nestedtypedef;

//classes
#pragma link C++ namespace Harry;
#pragma link C++ class Harry::Plotter+;

#endif
