#ifndef PARTICLE_H
#define PARTICLE_H

#include "particletype.h"
#include "resonancetype.h"

#include <iostream>
#include <cmath>

class Particle{
public:
    Particle(std::string name, double fPx, double fPy_, double fPz_);
    
    //static void AddParticleType(std::string name, double mass, int charge);
    static void AddParticleType(std::string name, double mass, int charge, double width = 0);

    int Decay2body(Particle &dau1,Particle &dau2) const;

    int GetIndex() const;
    double GetXMomentum() const;
    double GetYMomentum() const;
    double GetZMomentum() const;
    double GetMomentumModule() const;

    double GetMass() const;
    double GetInvMass(Particle& p);

    double ParticleEnergy() const;

    void ParticlePrint() const;

    void SetParticle(int index);
    void SetParticle(std::string name);
    void SetParticleMomentum(double xMomentum, double yMomentum, double zMomentum);

    void static PrintArray();

private:
    static int FindParticle(std::string name);

    void Boost(double bx, double by, double bz);
    
    static const int fMaxNumParticleType_ = 10; //numero di particelle disponibili
    static ParticleType *fParticleType_[fMaxNumParticleType_]; //puntatore al tipo di particelle
    static int fNParticleType_; //contatore di particelle non vuote

    int fIParticle_ = 0; //indice della particella nell'array
    double fPx_ = 0; //componenti dell'impulso
    double fPy_ = 0;
    double fPz_ = 0;
};

#endif