//autor Omar.Zapata@cern.ch 3 nov 2017
//CMS - Universidad de Antioquia

#ifndef PLOTTER_H
#define PLOTTER_H

#include<TObject.h>
#include<TH1F.h>
#include<TCanvas.h>
#include<TChain.h>
#include<TString.h>
#include<TFile.h>
#include<THStack.h>
#include<map>
#include<iostream>

class Plotter:public TObject
{
protected:
    std::vector<TChain*>  fChains;        //Global chain to handle files and trees
    std::map<TString,THStack*> fHStacks;  //stack to plot multiple histograms
    std::vector<TString>  fBranches;      //branches to be plotted
    UInt_t fNBins;                        //number of bins in histograms
    Double_t fXmin;                       //low value for histograms
    Double_t fXmax;                       //high value for histograms
    TFile    *fOutput;                    //!file to save the plots
    TCanvas  *fCanvas;                    //Global canvas to draw
public:
    Plotter(std::vector<TString> treenames,const std::vector<TString> branches, UInt_t bins=100, Double_t xmin=-10.0, Double_t xmax=10.0);
    //copy constructor
    Plotter(const Plotter &p);
    ~Plotter();

    void AddFile(const char *filename, const char *treename="",Long64_t nentries=TTree::kMaxEntries);
    void AddFriend(TTree *chainOrTree,const char *alias = "");
    
    std::map<TString,THStack*> &GetPlots();
    
    void Print();
    ClassDef(Plotter,0);
};

#endif
