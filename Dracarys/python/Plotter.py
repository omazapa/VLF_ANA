
# @(#)VLF ANALYSIS:$Id$
# Author: Omar Zapata (Omar.Zapata@cern.ch) 2017


__version__ = '0.0.1'
__author__  = 'Omar Zapata (Omar.Zapata@cern.ch)'

from ROOT import gSystem as _gSystem
from ROOT import TTree as _TTree
from ROOT import TCut as _TCut
from ROOT.std import vector as _vector
from ROOT.std import string as _string


_gSystem.Load("libHarryPlotter.so")
from ROOT.Harry  import Plotter as _Plotter

class Plotter(object):
    def __init__(self,treename,branches,bins=100,xmin=-10.0, xmax=10.0):
        __fBranches = _vector("string")()
        for i in range(len(branches)):
            __fBranches.push_back(branches[i])
        self.plotter = _Plotter(treename,__fBranches,bins,xmin,xmax)
        
    def AddDirectory(self,path,alias,weight=1.0):
        self.plotter.AddDirectory(path,alias,weight)
        
    def AddFile(self,alias,filename, weight, nentries=_TTree.kMaxEntries):
        self.plotter.AddFile(alias,filename,weight,nentries)
    
    def SetCut(self,alias,cut):
        cuts=_TCut(cut)
        self.plotter.SetCut(alias,cuts)
        
    def SetCut(self,cut):
        cuts=_TCut(cut)
        self.plotter.SetCut(cuts) 
        
    def GetPlots(self):
        plotmap = self.plotter.GetPlots()
        plots = {}
        for key,value in plotmap:
            plots[key] = (value.first,value.second)
        return plots
    
    def SavePdf(self,filename,branch):
        self.plotter.SavePdf(filename,branch)
        
    def SaveFile(self,filename):
        self.plotter.SaveFile(filename)
        
    def SetSumw2(self):
        self.plotter.SetSumw2()
        
    def SavePdfs(self,dir="plots"):
        self.plotter.SavePdfs(dir)
