#include "TH1F.h"
#include "TCanvas.h"
#include "TROOT.h"
#include "TFile.h"
#include "TStyle.h"

int main(){

    double pi = M_PI;

    TFile *file = TFile::Open("datidaestrarre.root");
    TFile *flMC_file = new TFile("locale.root");

    gStyle->SetOptFit(1);

    auto c1bis = new TCanvas();
    auto c2bis = new TCanvas();
    c1bis->Divide(2, 2);
    c2bis->Divide(1, 3);

    auto h1bis = new TH1F("h1", "Particles", 7, 0, 7);                          
    auto h2bis = new TH1F("h2", "Azimuthal angle distribution", 500, 0, 2 * pi); 
    auto h3bis = new TH1F("h3", "Polar angle distribution", 250, 0, pi);         
    auto h4bis = new TH1F("h4", "Impulse", 800, 0, 5.5);

    auto h12bis = new TH1F("h12", "From K* Decay", 400, 0.5, 1.2);         
    auto h13bis = new TH1F("h13", "K* extracted from invariant masses (P/K)", 400, 0, 5);
    auto h14bis = new TH1F("h14", "K* extracted from invariant masses (Charge)", 400, 0, 5);

    h1bis = (TH1F*)file->Get("h1");
    h2bis = (TH1F*)file->Get("h2");
    h3bis = (TH1F*)file->Get("h3");
    h4bis = (TH1F*)file->Get("h4");

    h12bis = (TH1F*)file->Get("h12");
    h13bis = (TH1F*)file->Get("h13");
    h14bis = (TH1F*)file->Get("h14");
    
    flMC_file->GetList()->Write();

    h1bis->GetYaxis()->SetTitle("Occorrenze");
    h2bis->GetYaxis()->SetTitle("Occorrenze");
    h3bis->GetYaxis()->SetTitle("Occorrenze");
    h4bis->GetYaxis()->SetTitle("Occorrenze");

    h12bis->GetYaxis()->SetTitle("Occorrenze");
    h13bis->GetYaxis()->SetTitle("Occorrenze");
    h14bis->GetYaxis()->SetTitle("Occorrenze");

    h1bis->GetXaxis()->SetTitle("Popolazioni");
    h2bis->GetXaxis()->SetTitle("Angolo Azimutale");
    h3bis->GetXaxis()->SetTitle("Angolo Polare");
    h4bis->GetXaxis()->SetTitle("Modulo Impulso");

    h12bis->GetXaxis()->SetTitle("Massa Invariante");
    h13bis->GetXaxis()->SetTitle("Massa Invariante");
    h14bis->GetXaxis()->SetTitle("Massa Invariante");

    c1bis->cd(1);
    h1bis->Draw();
    c1bis->cd(4);
    h2bis->Draw();
    c1bis->cd(3);
    h3bis->Draw();
    c1bis->cd(2);
    h4bis->Draw();

    c2bis->cd(1);
    h12bis->Draw();
    c2bis->cd(2);
    h13bis->Draw();
    c2bis->cd(3);
    h14bis->Draw();

    c1bis->Print("FINALE1.pdf", "pdf");
    c2bis->Print("FINALE2.pdf", "pdf");
    
}