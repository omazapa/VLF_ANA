#include<Plotter.h>
#include<TROOT.h>

static UInt_t counter=0;

//______________________________________________________________________________
Plotter::Plotter(std::vector<TString> treenames,const std::vector<TString> branches, UInt_t bins,Double_t xmin, Double_t xmax):fBranches(branches),fNBins(bins),fXmin(xmin),fXmax(xmax),fOutput(nullptr)
{
    TString trees="(";
    for(auto &treename:treenames)
    {
        auto chain = new TChain(treename.Data());
        fChains.push_back(chain);
        trees+=" "+treename;
    }
    trees+=" )";
    
    for(auto branch:fBranches)
    {
        auto hstack = new THStack(Form("PlotterStack%d",counter),Form("Plotter Stack Branch=%s Trees=%s",branch.Data(),trees.Data()));
        fHStacks[branch.Data()]=hstack;
    }
    fCanvas = new TCanvas(Form("canvas%d",counter));
    counter++;
}


//______________________________________________________________________________
Plotter::Plotter(const Plotter &p)
{
    fChains = p.fChains;
    fHStacks = p.fHStacks;
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
    //TODO: free vectors of pointers and map
    if(fCanvas!=nullptr) delete fCanvas;
}
    
//______________________________________________________________________________
void Plotter::AddFile(const char *filename, const char *treename,Long64_t nentries)
{
    for(auto &chain:fChains)
    {
        chain->AddFile(filename,nentries,treename);
    }
}
    
//______________________________________________________________________________
void Plotter::AddFriend(TTree *chainOrTree,const char *alias)
{
    for(auto &chain:fChains)
    {
        chain->AddFriend(chainOrTree,alias);
    }
}
    
//______________________________________________________________________________
std::map<TString,THStack*>  &Plotter::GetPlots()
{
    auto color=5;
    for(auto &branch:fBranches)
    {
        auto bname=branch.Data();
        for(auto &chain:fChains)
        {
            auto cname=chain->GetName();
            if(chain->GetBranch(bname)==nullptr)
            {
                Error(Form("%s::%s(%d)",__FILE__,__FUNCTION__,__LINE__),"Branch %s not found omitting...",bname);
                continue;//branch dont exists, omitting and continue to the next branch
            }
            auto h=new TH1F(Form("%s%s",cname,bname),Form("%s%s",cname,bname),fNBins,fXmin,fXmax);
            chain->Draw(Form("%s>>%s",bname,Form("%s%s",cname,bname)));
            h->SetFillColor(color);
            color++;
            fHStacks[bname]->Add(h);
        }
    }
    //fCanvas->Close();
    return fHStacks;
}

//______________________________________________________________________________
void Plotter::Print()
{
    std::cout<<"---------------- Chains ----------------<<std::endl"<<std::endl;
    for(auto &chain:fChains)
    {
        chain->Print();
    }
    std::cout<<"---------------- HStacks ----------------<<std::endl"<<std::endl;
    for(auto &hstack:fHStacks)
    {
        hstack.second->Print();
    }
}



