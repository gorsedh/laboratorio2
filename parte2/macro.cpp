#include <string>
#include <cmath>
#include <iostream>

#include "TH1F.h"
#include "TRandom.h"
#include "TMath.h"
#include "TCanvas.h"
#include "TROOT.h"
#include "TF1.h"

#include "particle.h"

void macro()
{
    //compilazione dei file di implementazioni necessari, collegati alle librerie in particle.h
    gROOT->LoadMacro("particletype.cpp+");
    gROOT->LoadMacro("resonancetype.cpp+");
    gROOT->LoadMacro("particle.cpp+");

    //dati del programma
    const int numberOfParticles = 100; //particelle per array
    const int N = 120;                 //massima size dell'array
    double phi, theta, impulse;

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

    std::cout << "Particelle caricate";

    //creazione delle canvas
    auto c1 = new TCanvas();
    auto c2 = new TCanvas();
    c1->Divide(3, 2);
    c2->Divide(3, 2);

    std::cout << "Canvas fatte";

    //i 12 grafici
    auto h1 = new TH1F("h1", "Particles", 7, 0, 7);                            //tipi di particelle generati
    auto h2 = new TH1F("h2", "Azimuthal angle distribution", 2000, 0, 2 * pi); //distribuzioni degli angoli azimutali (phi)
    auto h3 = new TH1F("h3", "Polar angle distribution", 1000, 0, pi);         // " " " polari (theta)
    auto h4 = new TH1F("h4", "Impulse", 1000, 0, 10);                          //impulso
    auto h5 = new TH1F("h5", "Transverse impulse", 1000, 0, 10);               //impulso trasverso (no z component)
    auto h6 = new TH1F("h6", "Energy", 1000, 0, 10);                           //energia della particella

    auto h7 = new TH1F("h7", "Invariant masses", 100, 0, 5);              //massa invariante fra tutte le particelle generate in ogni evento
    auto h8 = new TH1F("h8", "Invariant m. different charge", 100, 0, 5); //massa invariante fra tutte le particelle generate in ogni evento, in combinazioni con carica di segno discorde
    auto h9 = new TH1F("h9", "Invariant m. same charge", 100, 0, 5);      //massa invariante fra tutte le particelle generate in ogni evento, in combinazioni con carica di segno concorde
    auto h10 = new TH1F("h10", "P+/K-, P-/K+", 100, 0, 5);                //massa invariante fra tutte le particelle generate in ogni evento con combinazioni di tipo pione+/Kaone- e pione-/Kaone+
    auto h11 = new TH1F("h11", "P+/K+, P-/K-", 100, 0, 5);                //massa invariante fra tutte le particelle generate in ogni evento con combinazioni di tipo pione+/Kaone+ e pione-/Kaone-
    auto h12 = new TH1F("h12", "From K* Decay", 50, 0, 5);                //massa invariante fra le particelle generate in ogni evento che derivano dal decadimento della risonanza K*

    std::cout << "grafici fatti";

    gRandom->SetSeed();

    std::cout << "seme settato";

    for (int i = 0; i < 10 /*000*/; i++)
    { //i 100k eventi

        Particle particles[N];
        int extraParticles = 0; //particelle da aggiungere in fondo all'array per i decadimenti k*

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
                }

                //h11 P+/K+, P-/K- (0,2  1,3)
                if (((particles[k].GetIndex() == 0) && (particles[n].GetIndex() == 2)) ||
                    ((particles[k].GetIndex() == 1) && (particles[n].GetIndex() == 3)))
                {
                    h11->Fill(particles[k].GetInvMass(particles[n]));
                }
            }
        }

        if (extraParticles != 0)
        {
            for (int j = 0; j < extraParticles; j += 2)
            {
                h12->Fill(particles[numberOfParticles + j].GetInvMass(particles[numberOfParticles + 1 + j]));
            }
        }
    }

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
    c2->cd(6);
    h12->Draw();

    c1->Print("c1.pdf", "pdf");
    c2->Print("c2.pdf", "pdf");
}