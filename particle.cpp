#include "particle.h"
#include "particletype.h"
#include "resonancetype.h"

#include <iostream>
#include <cmath>
#include <cstdlib>

int Particle::fNParticleType_ = 0;
ParticleType *Particle::fParticleType_[fMaxNumParticleType_];

Particle::Particle(std::string name, double fPx, double fPy, double fPz) : fPx_{fPx}, fPy_{fPy}, fPz_{fPz}
{
    fIParticle_ = FindParticle(name);
};

int Particle::FindParticle(std::string name)
{
    for (int i = 0; i < fNParticleType_; i++)
    {
        if (fParticleType_[i]->GetParticleName() == name)
        {
            return i;
        }
    }
    std::cout << name << " not found" << '\n';
    return fNParticleType_ + 1;
};

void Particle::Boost(double bx, double by, double bz)
{

  double energy = ParticleEnergy();

  //Boost this Lorentz vector
  double b2 = bx*bx + by*by + bz*bz;
  double gamma = 1.0 / sqrt(1.0 - b2);
  double bp = bx*fPx_ + by*fPy_ + bz*fPz_;
  double gamma2 = b2 > 0 ? (gamma - 1.0)/b2 : 0.0;

  fPx_ += gamma2*bp*bx + gamma*bx*energy;
  fPy_ += gamma2*bp*by + gamma*by*energy;
  fPz_ += gamma2*bp*bz + gamma*bz*energy;
}

void Particle::AddParticleType(std::string name, double mass, int charge, double width)
{

    int index = FindParticle(name);

    if (width == 0)
    {
        if (index < fNParticleType_)
        {
            std::cout << "Particle " << name << " already in the array\n";
            return;
        }

        if (fNParticleType_ < fMaxNumParticleType_)
        {
            fParticleType_[fNParticleType_] = new ParticleType(name, mass, charge);
            std::cout << "Particle " << name << " added to the array\n";
            fNParticleType_++;
            std::cout << "There are " << fMaxNumParticleType_ - fNParticleType_ << " spaces left in the array\n\n";
        }
        else
        {
            std::cout << "ARRAY IS FULL, DELETE SOME PARTICLES\n\n\n";
        }
    }
    else
    {

        if (index < fNParticleType_)
        {
            std::cout << "Resonance " << name << " already in the array\n";
            return;
        }

        if (fNParticleType_ < fMaxNumParticleType_)
        {
            fParticleType_[fNParticleType_] = new ResonanceType(name, mass, charge, width);
            std::cout << "Resonance " << name << " added to the array\n";
            fNParticleType_++;
            std::cout << "There are " << fMaxNumParticleType_ - fNParticleType_ << " spaces left in the array\n\n";
        }
        else
        {
            std::cout << "ARRAY IS FULL, DELETE SOME PARTICLES\n\n\n";
        }
    }
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int Particle::Decay2body(Particle &dau1,Particle &dau2) const {
  if(GetMass() == 0.0){
    printf("Decayment cannot be preformed if mass is zero\n");
    return 1;
  }
  
  double massMot = GetMass();
  double massDau1 = dau1.GetMass();
  double massDau2 = dau2.GetMass();

  if(fIParticle_ > -1){ // add width effect

    // gaussian random numbers

    float x1, x2, w, y1, y2;
    
    double invnum = 1./RAND_MAX;
    do {
      x1 = 2.0 * rand()*invnum - 1.0;
      x2 = 2.0 * rand()*invnum - 1.0;
      w = x1 * x1 + x2 * x2;
    } while ( w >= 1.0 );
    
    w = sqrt( (-2.0 * log( w ) ) / w );
    y1 = x1 * w;
    y2 = x2 * w;

    massMot += fParticleType_[fIParticle_]->GetWidth() * y1;

  }

  if(massMot < massDau1 + massDau2){
    printf("Decayment cannot be preformed because mass is too low in this channel\n");
    return 2;
  }
  
  double pout = sqrt((massMot*massMot - (massDau1+massDau2)*(massDau1+massDau2))*(massMot*massMot - (massDau1-massDau2)*(massDau1-massDau2)))/massMot*0.5;

  double norm = 2*M_PI/RAND_MAX;

  double phi = rand()*norm;
  double theta = rand()*norm*0.5 - M_PI/2.;
  dau1.SetParticleMomentum(pout*sin(theta)*cos(phi),pout*sin(theta)*sin(phi),pout*cos(theta));
  dau2.SetParticleMomentum(-pout*sin(theta)*cos(phi),-pout*sin(theta)*sin(phi),-pout*cos(theta));

  double energy = sqrt(fPx_*fPx_ + fPy_*fPy_ + fPz_*fPz_ + massMot*massMot);

  double bx = fPx_/energy;
  double by = fPy_/energy;
  double bz = fPz_/energy;

  dau1.Boost(bx,by,bz);
  dau2.Boost(bx,by,bz);

  return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int Particle::GetIndex() const { return fIParticle_; };
double Particle::GetXMomentum() const { return fPx_; };
double Particle::GetYMomentum() const { return fPy_; };
double Particle::GetZMomentum() const { return fPz_; };
double Particle::GetMomentumModule() const { return sqrt(fPx_ * fPx_ + fPy_ * fPy_ + fPz_ * fPz_); };

double Particle::GetMass() const
{
    int i = this->fIParticle_;
    return fParticleType_[i]->GetMass();
};

double Particle::GetInvMass(Particle &p)
{
    double totE = pow(ParticleEnergy() + p.ParticleEnergy(), 2);
    double totP = pow(GetXMomentum() + p.GetXMomentum(), 2) + pow(GetYMomentum() + p.GetYMomentum(), 2) + pow(GetZMomentum() + p.GetZMomentum(), 2);
    double invMass = sqrt(totE - totP);
    return invMass;
};

double Particle::ParticleEnergy() const
{ //this function returns the total energy of the particle (c is assumed as 1)
    double energy = 0;
    energy = sqrt(pow(this->GetMass(), 2) + pow(this->GetMomentumModule(), 2));
};

void Particle::ParticlePrint() const
{
    int ParticleIndex = GetIndex();
    std::cout << "Index is " << ParticleIndex << "\n";
    std::cout << "Particle type is " << fParticleType_[ParticleIndex]->GetParticleName() << "\n";
    std::cout << "Momentum of the particle is:\n x: " << GetXMomentum() << "\n y: " << GetYMomentum()
              << "\n z: " << GetZMomentum() << "\n\n";
};

void Particle::SetParticle(int index)
{
    if (index < fNParticleType_)
    {
        fIParticle_ = index;
    }
    else
    {
        std::cout << "No particle is associated to the index given in SetParticle()\n";
    }
};

void Particle::SetParticle(std::string name)
{
    int index = FindParticle(name);
    if (index < fNParticleType_)
    {
        fIParticle_ = index;
    }
    else
    {
        std::cout << "No particle is associated to the name given in SetParticle\n";
    }
};

void Particle::SetParticleMomentum(double xMomentum, double yMomentum, double zMomentum)
{
    fPx_ = xMomentum;
    fPy_ = yMomentum;
    fPz_ = zMomentum;
};

void Particle::PrintArray()
{
    std::cout << "\n\033[0;36mArray of particles contains " << fNParticleType_ << " particles:\033[0m\n\n\033[1;33m";
    for (int i = 0; i < fNParticleType_; i++)
    {
        fParticleType_[i]->Print();
        std::cout << '\n';
    }
    std::cout << "\033[0m\n";
}
