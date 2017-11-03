#include<Plotter.h>
#include<TROOT.h>

static UInt_t counter=0;

//______________________________________________________________________________
Plotter::Plotter(TString treename,const std::vector<TString> branches, UInt_t bins,Double_t xmin, Double_t xmax):fBranches(branches),fNBins(bins),fXmin(xmin),fXmax(xmax),fOutput(nullptr)
{
    fChain  = new TChain(treename.Data());
    fHStack = new THStack(Form("PlotterStack%d",counter),Form("Plotter %d Stacked Histograms",counter));
    fCanvas = new TCanvas(Form("c%d",counter));
    counter++;
}

//______________________________________________________________________________
Plotter::Plotter(TChain *chain,const std::vector<TString> branches, UInt_t bins,Double_t xmin, Double_t xmax):fChain(chain),fBranches(branches),fNBins(bins),fXmin(xmin),fXmax(xmax),fOutput(nullptr)
{
    fHStack = new THStack(Form("PlotterStack%d",counter),Form("Plotter %d Stacked Histograms",counter));    
    counter++;
}

//______________________________________________________________________________
Plotter::Plotter(const Plotter &p)
{
    fChain = p.fChain;
    fHStack = p.fHStack;
    fBranches = p.fBranches; 
    fNBins = p.fNBins; 
    fXmin = p.fXmin; 
    fXmax = p.fXmax; 
    fOutput = p.fOutput; 
    fCanvas = p.fCanvas;
}

//______________________________________________________________________________
Plotter::~Plotter()
{
    if(fHStack!=nullptr) delete fHStack;
    if(fCanvas!=nullptr) delete fCanvas;
}
    
//______________________________________________________________________________
void Plotter::AddFile(const char *filename, Long64_t nentries, const char *treename)
{
    fChain->AddFile(filename,nentries,treename);
}
    
//______________________________________________________________________________
void Plotter::AddFriend(TTree *chainOrTree,const char *alias)
{
    fChain->AddFriend(chainOrTree,alias);
}
    
//______________________________________________________________________________
THStack *Plotter::GetPlot()
{
    auto color=5;
    for(auto &branch:fBranches)
    {
        auto bname=branch.Data();
        if(fChain->GetBranch(bname)==nullptr)
        {
            Error(Form("%s::%s(%d)",__FILE__,__FUNCTION__,__LINE__),"Branch %s not found omitting...",bname);
            continue;//branch dont exists, omitting and continue to the next branch
        }
        
        auto h=new TH1F(bname,bname,fNBins,fXmin,fXmax);
        fChain->Draw(Form("%s>>%s",bname,bname));
        h->SetFillColor(color);
        color++;
        fHStack->Add(h);
    }
    fHStack->Draw();
    return fHStack;
}

//______________________________________________________________________________
void Plotter::Print()
{
    fHStack->Print();
    fChain->Print();
}



