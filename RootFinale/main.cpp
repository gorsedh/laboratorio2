#include <string>
#include <cmath>
#include <iostream>

#include "TH1F.h"
#include "TRandom.h"
#include "TMath.h"
#include "TCanvas.h"
#include "TROOT.h"
#include "TF1.h"
#include "TFile.h"
#include "TFitResult.h"
#include "TStyle.h"

#include "particle.h"

int main()
{
    auto startTime = std::chrono::system_clock::now();
    std::cout << "\nSimulation Started\n";

    //compilazione dei file di implementazioni necessari, collegati alle librerie in particle.h
    gROOT->LoadMacro("particletype.cpp+");
    gROOT->LoadMacro("resonancetype.cpp+");
    gROOT->LoadMacro("particle.cpp+");

    //dati del programma
    int const numberOfParticles = 100; //particelle per array
    int const N = 120;                 //massima size dell'array
    int const events = 100000;         //generazioni
    double phi, theta, impulse;

    //grafica
    int const binsForProgressBar = 20;
    int const PercentEvents = events / binsForProgressBar;
    int barFilling = 0;

    //dati delle particelle in uso e altre variabili necessarie
    double const massP = 0.13957;
    double const massK = 0.49367;
    double const massProton = 0.93827;
    double const massKres = 0.89166;
    double pi = M_PI;

    //caricamento delle particelle necessarie
    Particle::AddParticleType("Pion+", massP, 1);           //0 //80% spawn
    Particle::AddParticleType("Pion-", massP, -1);          //1
    Particle::AddParticleType("Kaon+", massK, 1);           //2 //10% spawn
    Particle::AddParticleType("Kaon-", massK, -1);          //3
    Particle::AddParticleType("Proton+", massProton, 1);    //4 //9% spawn
    Particle::AddParticleType("Proton-", massProton, -1);   //5
    Particle::AddParticleType("Kstar", massKres, 0, 0.050); //6 //1% spawn

    //root file
    TFile *file = new TFile("data.root", "recreate");

    //creazione delle canvas
    auto c1 = new TCanvas();
    auto c2 = new TCanvas();
    c1->Divide(3, 2);
    c2->Divide(3, 2);

    //i 12 grafici
    auto h1 = new TH1F("h1", "Particles", 7, 0, 7);                           //tipi di particelle generati
    auto h2 = new TH1F("h2", "Azimuthal angle distribution", 500, 0, 2 * pi); //distribuzioni degli angoli azimutali (phi)
    auto h3 = new TH1F("h3", "Polar angle distribution", 250, 0, pi);         // " " " polari (theta)
    auto h4 = new TH1F("h4", "Impulse", 800, 0, 5.5);                         //impulso
    auto h5 = new TH1F("h5", "Transverse impulse", 600, 0, 4);                //impulso trasverso (no z component)
    auto h6 = new TH1F("h6", "Energy", 600, 0, 5);                            //energia della particella

    auto h7 = new TH1F("h7", "Invariant masses", 400, 0, 5);              //massa invariante fra tutte le particelle generate in ogni evento
    auto h8 = new TH1F("h8", "Invariant m. different charge", 400, 0, 5); //massa invariante fra tutte le particelle generate in ogni evento, in combinazioni con carica di segno discorde
    auto h9 = new TH1F("h9", "Invariant m. same charge", 400, 0, 5);      //massa invariante fra tutte le particelle generate in ogni evento, in combinazioni con carica di segno concorde
    auto h10 = new TH1F("h10", "P+/K-, P-/K+", 400, 0, 5);                //massa invariante fra tutte le particelle generate in ogni evento con combinazioni di tipo pione+/Kaone- e pione-/Kaone+
    auto h11 = new TH1F("h11", "P+/K+, P-/K-", 400, 0, 5);                //massa invariante fra tutte le particelle generate in ogni evento con combinazioni di tipo pione+/Kaone+ e pione-/Kaone-
    auto h12 = new TH1F("h12", "From K* Decay", 400, 0.5, 1.2);           //massa invariante fra le particelle generate in ogni evento che derivano dal decadimento della risonanza K*
    //Per le operazioni sugli istogrammi in parte 3
    auto h10b = new TH1F("h10b", "P+/K-, P-/K+", 5000, 0.5, 1.5);  //massa invariante fra tutte le particelle generate in ogni evento con combinazioni di tipo pione+/Kaone- e pione-/Kaone+
    auto h11b = new TH1F("h11b", "P+/K+, P-/K-", 5000, 0.5, 1.5);  //massa invariante fra tutte le particelle generate in ogni evento con combinazioni di tipo pione+/Kaone+ e pione-/Kaone-
    auto h12b = new TH1F("h12b", "From K* Decay", 5000, 0.5, 1.5); //massa invariante fra le particelle generate in ogni evento che derivano dal decadimento della risonanza K*

    h12->Sumw2();
    //std::cout << "grafici fatti";

    gRandom->SetSeed();

    std::cout << "\nGeneration in progress\n";

    for (int i = 0; i < events; i++)
    { //i 100k eventi

        Particle particles[N];
        int extraParticles = 0; //particelle da aggiungere in fondo all'array per i decadimenti k*
        if (i % PercentEvents == 0)
        {
            //std::cout << i << '\n';
            std::cout << "[";
            for (int bF = 0; bF < barFilling; bF++)
            {
                std::cout << "\033[0;32m*\033[0m";
            }
            for (int bE = 0; bE < binsForProgressBar - barFilling; bE++)
            {
                std::cout << " ";
            }
            std::cout << "]\n";

            barFilling++;
        }
        for (int j = 0; j < numberOfParticles; j++)
        { //i nostri array da 100

            phi = gRandom->Uniform(2 * pi);
            h2->Fill(phi);
            theta = gRandom->Uniform(pi);
            h3->Fill(theta);
            impulse = gRandom->Exp(1);
            h4->Fill(impulse);

            double PX, PY, PZ;
            PX = impulse * sin(theta) * cos(phi);
            PY = impulse * sin(theta) * sin(phi);
            PZ = impulse * cos(theta);
            particles[j].SetParticleMomentum(PX, PY, PZ);

            double transverseImpulse = sqrt(PX * PX + PY * PY);
            h5->Fill(transverseImpulse);

            double p = gRandom->Uniform(1);
            if (p < 0.40)
            { //Pion+
                particles[j].SetParticle("Pion+");
                h1->Fill(0);
            }
            else if (p < 0.80)
            { //Pion-
                particles[j].SetParticle("Pion-");
                h1->Fill(1);
            }
            else if (p < 0.85)
            { //Kaon+
                particles[j].SetParticle("Kaon+");
                h1->Fill(2);
            }
            else if (p < 0.90)
            { //Kaon-
                particles[j].SetParticle("Kaon-");
                h1->Fill(3);
            }
            else if (p < 0.945)
            { //Proton+
                particles[j].SetParticle("Proton+");
                h1->Fill(4);
            }
            else if (p < 0.99)
            { //Proton-
                particles[j].SetParticle("Proton-");
                h1->Fill(5);
            }
            else if (p < 0.995)
            { //K* into Pion+ Kaon-
                particles[j].SetParticle("Kstar");
                h1->Fill(6);
                particles[numberOfParticles + extraParticles].SetParticle("Pion+");
                particles[numberOfParticles + extraParticles + 1].SetParticle("Kaon-");
                particles[j].Decay2body(particles[numberOfParticles + extraParticles], particles[numberOfParticles + extraParticles + 1]);
                extraParticles++;
                extraParticles++;
            }
            else
            { //K* into Pion- Kaon+
                particles[j].SetParticle("Kstar");
                h1->Fill(6);
                particles[numberOfParticles + extraParticles].SetParticle("Pion-");
                particles[numberOfParticles + extraParticles + 1].SetParticle("Kaon+");
                particles[j].Decay2body(particles[numberOfParticles + extraParticles], particles[numberOfParticles + extraParticles + 1]);
                extraParticles++;
                extraParticles++;
            }

            double energy = particles[j].ParticleEnergy();
            h6->Fill(energy);
        }

        //finita la generazione devo andare a fare le operazioni di scorrimento sull'array

        for (int k = 0; k < numberOfParticles + extraParticles - 1; k++)
        {
            for (int n = k + 1; n < numberOfParticles + extraParticles; n++)
            {
                h7->Fill(particles[k].GetInvMass(particles[n])); //h7 massa invariante fra tutte le particelle generate in ogni evento

                //h8 combinazione di carica discorde (positive 0,2,4 ///\\\ negative 1,3,5)
                if (((particles[k].GetIndex() == 0 || particles[k].GetIndex() == 2 || particles[k].GetIndex() == 4) && (particles[n].GetIndex() == 1 || particles[n].GetIndex() == 3 || particles[n].GetIndex() == 5)) ||
                    ((particles[k].GetIndex() == 1 || particles[k].GetIndex() == 3 || particles[k].GetIndex() == 5) && (particles[n].GetIndex() == 0 || particles[n].GetIndex() == 2 || particles[n].GetIndex() == 4)))
                {
                    h8->Fill(particles[k].GetInvMass(particles[n]));
                }

                //h9 combinazione di carica concorde (positive 0,2,4 ///\\\ negative 1,3,5)
                if (((particles[k].GetIndex() == 0 || particles[k].GetIndex() == 2 || particles[k].GetIndex() == 4) && (particles[n].GetIndex() == 0 || particles[n].GetIndex() == 2 || particles[n].GetIndex() == 4)) ||
                    ((particles[k].GetIndex() == 1 || particles[k].GetIndex() == 3 || particles[k].GetIndex() == 5) && (particles[n].GetIndex() == 1 || particles[n].GetIndex() == 3 || particles[n].GetIndex() == 5)))
                {
                    h9->Fill(particles[k].GetInvMass(particles[n]));
                }

                //h10 P+/K-, P-/K+ (0,3  1,2)
                if (((particles[k].GetIndex() == 0) && (particles[n].GetIndex() == 3)) ||
                    ((particles[k].GetIndex() == 1) && (particles[n].GetIndex() == 2)))
                {
                    h10->Fill(particles[k].GetInvMass(particles[n]));
                    h10b->Fill(particles[k].GetInvMass(particles[n]));
                }

                //h11 P+/K+, P-/K- (0,2  1,3)
                if (((particles[k].GetIndex() == 0) && (particles[n].GetIndex() == 2)) ||
                    ((particles[k].GetIndex() == 1) && (particles[n].GetIndex() == 3)))
                {
                    h11->Fill(particles[k].GetInvMass(particles[n]));
                    h11b->Fill(particles[k].GetInvMass(particles[n]));
                }
            }
        }

        if (extraParticles != 0)
        {
            for (int j = 0; j < extraParticles; j += 2)
            {
                h12->Fill(particles[numberOfParticles + j].GetInvMass(particles[numberOfParticles + 1 + j]));
                h12b->Fill(particles[numberOfParticles + j].GetInvMass(particles[numberOfParticles + 1 + j]));
            }
        }
    }

    std::cout << "Generation completed\n\n";

    gStyle->SetOptFit(1);

    c1->cd(1);
    h1->Draw();
    c1->cd(2);
    h2->Draw();
    c1->cd(3);
    h3->Draw();
    c1->cd(4);
    h4->Draw();
    c1->cd(5);
    h5->Draw();
    c1->cd(6);
    h6->Draw();
    c2->cd(1);
    h7->Draw();
    c2->cd(2);
    h8->Draw();
    c2->cd(3);
    h9->Draw();
    c2->cd(4);
    h10->Draw();
    c2->cd(5);
    h11->Draw();

    std::cout << "\033[0;31mGAUSSIAN FITS OF K* RESONANCES\033[0m" << '\n';
    auto fit6a = new TF1("Fit K* Events", "gaus", 0.60, 1.30); //relativo fit
    h12->Fit(fit6a);

    c2->cd(6);
    h12->Draw();

    std::cout << '\n';
    c1->Print("c1.pdf", "pdf");
    c2->Print("c2.pdf", "pdf");
    std::cout << '\n';

    //parte 3
    //9.1 check ingressi -> corretti

    //9.2 check ingressi di h1
    double PionPCount = h1->GetBinContent(1);
    double PionNCount = h1->GetBinContent(2);
    double KaonPCount = h1->GetBinContent(3);
    double KaonNCount = h1->GetBinContent(4);
    double ProtonPCount = h1->GetBinContent(5);
    double ProtonNCount = h1->GetBinContent(6);
    double KstarCount = h1->GetBinContent(7);
    double const totalGenerated = numberOfParticles * events;
    std::cout << "\nGenerated particles:\n";
    std::cout << "Pion+   are " << PionPCount / totalGenerated << " ± " << h1->GetBinError(1) / totalGenerated << ",           0.40 expected\n";
    std::cout << "Pion-   are " << PionNCount / totalGenerated << " ± " << h1->GetBinError(2) / totalGenerated << ",           0.40 expected\n";
    std::cout << "Kaon+   are " << KaonPCount / totalGenerated << " ± " << h1->GetBinError(3) / totalGenerated << ",           0.05 expected\n";
    std::cout << "Kaon-   are " << KaonNCount / totalGenerated << " ± " << h1->GetBinError(4) / totalGenerated << ",           0.05 expected\n";
    std::cout << "Proton+ are " << ProtonPCount / totalGenerated << " ± " << h1->GetBinError(5) / totalGenerated << ",           0.045 expected\n";
    std::cout << "Proton- are " << ProtonNCount / totalGenerated << " ± " << h1->GetBinError(6) / totalGenerated << ",           0.045 expected\n";
    std::cout << "K*      are " << KstarCount / totalGenerated << " ± " << h1->GetBinError(7) / totalGenerated << ",           0.01 expected\n\n";

    //9.3 angle fit
    TCanvas *c3 = new TCanvas("c3", "Fit angles");
    c3->Divide(2, 1);
    auto fit1 = new TF1("Fit azimuthal angle", "pol0", 0, 2 * pi);
    auto fit2 = new TF1("Fit polar angle", "pol0", 0, pi);
    std::cout << "\033[0;31mAZIMUTHAL AND POLAR ANGLES FITS\033[0m" << '\n';
    c3->cd(1);
    h2->Fit(fit1);
    c3->cd(2);
    h3->Fit(fit2);
    std::cout << '\n';
    c3->Print("c3.pdf", "pdf");
    std::cout << '\n';

    //9.4 exponential fit
    TCanvas *c4 = new TCanvas("c4", "Impulse Fit");
    auto fit3 = new TF1("Fit impulse", "expo", 0, 5.5);
    c4->cd(1);
    std::cout << "\033[0;31mIMPULSE EXPONENTIAL FIT\033[0m" << '\n';
    h4->Fit(fit3);
    std::cout << '\n';
    c4->Print("c4.pdf", "pdf");
    std::cout << '\n';

    //10
    TCanvas *c5 = new TCanvas("c5", "Impulse Fit");
    c5->Divide(2, 3);
    c5->cd(1);
    h10->Draw();
    c5->cd(2);
    h11->Draw();
    c5->cd(3);
    h12b->Draw();
    c5->cd(4);

    std::cout << "\033[0;31mGAUSSIAN FITS OF K* RESONANCES\033[0m" << '\n';
    auto fit7 = new TF1("Fit K* Resonance", "gaus", 0.60, 1.30); //relativo fit
    h12->Fit(fit7);

    std::cout << "\033[0;31mGAUSSIAN FITS OF K* RESONANCES (Pion/Kaon)\033[0m" << '\n';
    auto h13 = new TH1F("h13", "K* extracted from invariant masses (P/K)", 400, 0, 5);
    h13->Sumw2();
    h13->Add(h10, h11, 1, -1);
    auto fit5a = new TF1("Fit subtraction discordant/concordant Pion Kaon", "gaus", 0.60, 1.30); //relativo fit
    h13->Fit(fit5a);

    c5->cd(5);
    std::cout << "\033[0;31mGAUSSIAN FITS OF K* RESONANCES (Particles by charge)\033[0m" << '\n';
    auto h14 = new TH1F("h14", "K* extracted from invariant masses (Charge)", 400, 0, 5);
    h14->Sumw2();
    h14->Add(h8, h9, 1, -1);
    auto fit5c = new TF1("Fit subtraction invariant masses by charge", "gaus", 0.65, 1.15); //relativo fit
    h14->Fit(fit5c);

    std::cout << '\n';
    c5->Print("c5.pdf", "pdf");
    std::cout << '\n';

    //fine parte 3

    file->Write();
    file->Close();

    //end notification
    std::cout << '\a';

    //time taken for execution
    auto endTime = std::chrono::system_clock::now();
    std::chrono::duration<double> diff = endTime - startTime;
    std::cout << "\nTime required: " << diff.count() << " s\n";
}