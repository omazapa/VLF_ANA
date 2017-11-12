//autor Omar.Zapata@cern.ch 3 nov 2017
//CMS - Universidad de Antioquia

#ifndef PLOTTER_H
#define PLOTTER_H

//ROOT Libraries
#include<TObject.h>
#include<TH1F.h>
#include<TCanvas.h>
#include<TChain.h>
#include<TString.h>
#include<TFile.h>
#include<THStack.h>
#include<TLegend.h>
#include<TCut.h>

//STD Libraries
#include<map>
#include<iostream>

class Plotter:public TObject
{
public:
    std::map<std::string,TChain*>  fChains;   //Global chain to handle files and trees
    std::map<std::string,std::pair<THStack*,TLegend*> > fHStacks;  //stack to plot multiple histograms
    std::vector<std::string>  fBranches;      //branches to be plotted
    UInt_t fNBins;                        //number of bins in histograms
    Double_t fXmin;                       //low value for histograms
    Double_t fXmax;                       //high value for histograms
    TFile    *fOutput;                    //!file to save the plots
    TCanvas  *fCanvas;                    //Global canvas to draw
    std::string  fTreeName;               //Global Tree name
    std::map<std::string,TCut>  fCuts;     //Cuts to apply  
public:
    Plotter(std::string treename,const std::vector<std::string> branches, UInt_t bins=100, Double_t xmin=-10.0, Double_t xmax=10.0);
    //copy constructor
    Plotter(const Plotter &p);
    ~Plotter();

    /**
     * Method to add root files to a TChain given a path.
     * \param path directory to search root files
     * \param alias category of the trees in the root files (Signal, Bgk0, Bgk1...)
     * \param weight weight to apply to all trees in the chain.
     */
    void AddDirectory(const Char_t *path,const Char_t *alias,Double_t weight=1);
    
    /**
     * Method to add a single root file to a TChain given a path and alias.
     * \param alias category of the trees in the root files (Signal, Bgk0, Bgk1...)
     * \param filename directory to root file
     * \param weight weight to apply to all trees in the chain.
     */
    void AddFile(const Char_t *alias,const Char_t *filename, Double_t weight,Long64_t nentries=TTree::kMaxEntries);
    
    /**
     * Method to apply a Cuts to TChain while the histograms are created.
     * \param alias category of the trees in the root files (Signal, Bgk0, Bgk1...) (TChain)
     * \param TCut object with the cuts https://root.cern.ch/doc/master/classTCut.html NOTE: mutiples cuts can be addressed in TCut object
     */
    void SetCut(const Char_t *alias, TCut cut);
    
    std::map<std::string,std::pair<THStack*,TLegend*> > &GetPlots();

    void Print();
    
public:
    std::pair<THStack*,TLegend*> &GetPlot(const Char_t *branch);
    std::vector<std::string> Find(std::string path, std::string pattern="*.root");
    ClassDef(Plotter,0);
};

#endif
