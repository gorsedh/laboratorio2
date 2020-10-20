#include "particletype.h"
#include "resonancetype.h"
#include "particle.h"

#include <iostream>

int main(){
    Particle::AddParticleType("pippo", 10.1, 7);
    Particle::AddParticleType("pluto", 10, 7, 4.5);
    Particle::AddParticleType("gigio", 10, 7, 4.5);
    /*Particle::AddParticleType("a", 10.1, 7);
    Particle::AddParticleType("b", 10.1, 7);
    Particle::AddParticleType("c", 10.1, 7);
    Particle::AddParticleType("d", 10.1, 7);
    Particle::AddParticleType("f", 10.1, 7);
    Particle::AddParticleType("g", 10.1, 7);
    Particle::AddParticleType("h", 10.1, 7);
    Particle::AddParticleType("i", 10.1, 7);
    Particle::AddParticleType("j", 10.1, 7);
    Particle::AddParticleType("k", 10.1, 7);*/
    //Particle::PrintArray();

    auto f = Particle("pluto", 2, 3, 4);
    f.ParticlePrint();
}

//Ho fatto fino ai getters bisogna aggiungere quella roba
//abbiamo il probllema che quando non trova la particella c'è un segmentation fault
//possibili soluzioni, aggiungere opzione per costruire particella nuova quando chiamata