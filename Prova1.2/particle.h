#ifndef PARTICLE_H
#define PARTICLE_H

#include "particletype.h"
#include "resonancetype.h"

#include <iostream>

class Particle{
public:
    Particle(std::string name, double fPx, double fPy_, double fPz_);
    
    static void AddParticleType(std::string name, double mass, int charge);
    static void AddParticleType(std::string name, double mass, int charge, double width);

    int const GetIndex();
    double const GetXMomentum();
    double const GetYMomentum();
    double const GetZMomentum();
    void const ParticlePrint();

    void SetParticle(int index);
    void SetParticle(std::string name);

    void static PrintArray();

private:
    static int FindParticle(std::string name);
    
    static const int fMaxNumParticleType_ = 10; //numero di particelle disponibili
    static ParticleType *fParticleType_[fMaxNumParticleType_]; //puntatore al tipo di particelle
    static int fNParticleType_; //contatore di particelle non vuote

    int fIParticle_ = 0; //indice della particella nell'array
    double fPx_ = 0; //componenti dell'impulso
    double fPy_ = 0;
    double fPz_ = 0;
};

#endif