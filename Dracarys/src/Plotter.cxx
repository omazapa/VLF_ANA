#include<Plotter.h>
#include<TROOT.h>
#include<TSystem.h>

using namespace Harry;

static UInt_t counter=0;

//______________________________________________________________________________
Plotter::Plotter(std::string treename,std::vector<std::string> branches, UInt_t bins,Double_t xmin, Double_t xmax):fBranches(branches),fNBins(bins),fXmin(xmin),fXmax(xmax),fOutput(nullptr),fTreeName(treename),fSumw2(kFALSE)
{    
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
void Plotter::AddDirectory(const char *path,const char *alias,Double_t weight)
{
    auto files=Find(path);        //getting paths for files *.root
    if(!fChains.count(alias))     //creating chain if dont exists
    {
        auto chain = new TChain(fTreeName.c_str());   
        fChains[alias]=chain;
    }
    
    for(auto &file:files)  //filling the chain with the files
       fChains[alias]->AddFile(file.c_str(),TTree::kMaxEntries,fTreeName.c_str());
    //NOTE: if you call two times this function changing the weight
    //all the previous weights will be change for the last one.
    fChains[alias]->SetWeight(weight);//setting weight 
    fChains[alias]->SetBranchStatus("*",1);//enabling all branches
}

//______________________________________________________________________________
void Plotter::AddFile(const char *alias,const char *filename, Double_t weight,Long64_t nentries)
{
    if(!fChains.count(alias))     //creating chain if dont exists
    {
        auto chain = new TChain(fTreeName.c_str());   
        fChains[alias]=chain;
    }
    fChains[alias]->AddFile(filename,nentries,fTreeName.c_str());
}

//______________________________________________________________________________
std::map<std::string, std::pair<std::vector<TH1F *>, TLegend *> > &Plotter::GetHists()
{
    for(auto &branch:fBranches){
            GetHists(branch.c_str());
    }
    return fHists;
}

//______________________________________________________________________________
std::pair<std::vector<TH1F*>,TLegend*>   &Plotter::GetHists(const Char_t *branch)
{
    auto color=2;
    std::vector<TH1F*> hists;
    TLegend *leg =nullptr;
    for(auto &chain:fChains)
    {
            auto cname=chain.first.c_str();
//TODO: check if branch or leaf exists            
//             if(chain.second->GetBranch(branch)==nullptr)
//             {
//                 Error(Form("%s::%s(%d)",__FILE__,__FUNCTION__,__LINE__),"Branch %s not found omitting...",branch);
//                 continue;//branch dont exists, omitting and continue to the next branch
//             }
//             if(chain.second->GetLeaf(branch)==nullptr)
//             {
//                 Error(Form("%s::%s(%d)",__FILE__,__FUNCTION__,__LINE__),"Leaf %s not found omitting...",branch);
//                 continue;//branch dont exists, omitting and continue to the next branch
//             }

            TCut cuts="";
            if(fCuts.count(cname)) cuts=fCuts[cname];
            TString lbranch=branch;
            lbranch=lbranch.ReplaceAll(")","").ReplaceAll("(","");
            chain.second->Draw(Form("%s>>%s(%d,%f,%f)",branch,Form("%s%s",cname,lbranch.Data()),fNBins,fXmin,fXmax),cuts && fCut,"goff");
//             if(!fHists.count(branch))//if histograms are not created
//             {
                if(!leg) leg = new TLegend(0.68,0.72,0.98,0.92);
                auto h=(TH1F*)gROOT->FindObject(Form("%s%s",cname,lbranch.Data()));
                h->SetName(Form("%s%s",cname,lbranch.Data()));
                h->SetTitle(Form("%s%s",cname,branch));
                h->SetFillColor(color);
                h->SetLineColor(color);
                // error as sqrt(sum of weights)
                if(fSumw2) h->Sumw2();//to compute the error on the weights
                hists.push_back(h);
                leg->AddEntry(h,cname,"l");
//             }else{
//                 leg=fHists[branch].second;
//             }
            
            color++;
    }
    fHists[branch]=std::pair<std::vector<TH1F*>,TLegend*>(hists,leg);
    return fHists[branch];
}

//______________________________________________________________________________
std::pair<THStack*,TLegend*>  &Plotter::GetHStack(const Char_t *branch)
{
    if(!fHists.count(branch))//if histograms are not created
    {
        GetHists(branch);
    }
    
    
    if(!fHStacks.count(branch))     //creating hstack if dont exists
    {
        std::string aliasnames="( ";
        std::vector<std::string> aliases;
        for(auto &chain:fChains)
        {
            auto name=chain.first;
            aliases.push_back(name);
            aliasnames+=name;
            aliasnames+=" ";
        }
        aliasnames+=")";
        
        auto hstack = new THStack(Form("PlotterStack%s",branch),Form("Plotter Stack Branch=%s Aliases=%s",branch,aliasnames.c_str()));
        fHStacks[branch]=std::pair<THStack*,TLegend*>(hstack,fHists[branch].second);
    }else{ //if exists just return the object;
        return fHStacks[branch];
    }

    for(auto &hist:fHists[branch].first)
    {
        fHStacks[branch].first->Add(hist);
    }
//     fHStacks[branch].second=fHists[branch].second;
    return fHStacks[branch];
}


//______________________________________________________________________________
std::map<std::string,std::pair<THStack*,TLegend*> > &Plotter::GetPlots()
{
        for(auto &branch:fBranches)
            GetHStack(branch.c_str());
        return fHStacks;
}

//______________________________________________________________________________
void Plotter::SetCut(const Char_t *alias, TCut cut)
{
        fCuts[alias]=cut;
}

//______________________________________________________________________________
void Plotter::SetCut(TCut cut)
{
    fCut=cut;
}

//______________________________________________________________________________
void Plotter::SavePdfs(const Char_t *dir)
{
    if(gSystem->AccessPathName(dir)) gSystem->mkdir(dir);
    
    for(auto &branch:fBranches){
        fCanvas = new TCanvas(Form("c%d",counter));
        counter++;
        auto hstack = GetHStack(branch.c_str());
        auto hs=(THStack*)hstack.first->Clone();
        auto legen=(TLegend*)hstack.second->Clone();
        hs->Draw();
        legen->Draw();
        fCanvas->Draw();
        auto filename=Form("%s/%s.pdf",dir,TString(branch.c_str()).ReplaceAll("(","").ReplaceAll(")","").Data());
        fCanvas->SaveAs(filename);
        delete fCanvas;
    }
    fCanvas=nullptr;
}


//______________________________________________________________________________
void Plotter::SavePdf(const Char_t *filename,const Char_t *branch)
{
    fCanvas = new TCanvas(Form("c%d",counter));
    counter++;
    auto hstack = GetHStack(branch);
    hstack.first->Draw();
    hstack.second->Draw();
    fCanvas->Draw();
    fCanvas->SaveAs(filename);
    delete fCanvas;
}

//______________________________________________________________________________
void Plotter::SaveFile(const Char_t *rootfile,const Char_t *mode)
{
    fOutput=new TFile(rootfile,mode);
    auto plots = GetPlots();
    for(auto &plot:plots)
    {
         TDirectory *branch = fOutput->mkdir(plot.first.c_str());
         branch->cd();
         auto hs=new THStack(plot.second.first->GetName(),plot.second.first->GetTitle());
         for(auto &hist:fHists[plot.first.c_str()].first)
         {
             auto h=(TH1F*)hist->Clone();
             h->Write();
             hs->Add(h);
         }
         hs->Write();//hstack
         fHists[plot.first.c_str()].second->Write();
         //plot.second.second->Write();//Legend
    }
//    fCut.Write("cuts");
    fOutput->Close();
}


//______________________________________________________________________________
void Plotter::Print()
{
    std::cout<<"---------------- Chains ----------------<<std::endl"<<std::endl;
    for(auto &chain:fChains)
    {
        std::cout<<chain.first<<std::endl;
        chain.second->Print();
    }
    std::cout<<"---------------- HStacks ----------------<<std::endl"<<std::endl;
    for(auto &hstack:fHStacks)
    {
        std::cout<<hstack.first<<std::endl;
        hstack.second.first->Print();
    }
}

//______________________________________________________________________________
std::vector<std::string> Plotter::Find(std::string path, std::string pattern)
{
    auto files=gSystem->GetFromPipe(Form("find %s -name \"%s\"",path.c_str(),pattern.c_str()));
    std::vector<std::string> strings;
    std::istringstream f(files.Data());
    std::string s;
    while (getline(f, s, '\n')) {
        strings.push_back(s);
    }
   return strings;
}



