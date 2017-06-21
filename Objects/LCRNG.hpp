#ifndef LCRNG_HPP
#define LCRNG_HPP

#include <string>

class LCRNG {

private:
    unsigned long add;
    unsigned long mult;
    unsigned long multR;
    unsigned long addR;
    unsigned long seed;
    
public:
    
    LCRNG();
    
    LCRNG(unsigned long seed);
    
    void reSeed(unsigned long newSeed);

    void setConst(unsigned long mult, unsigned long add, unsigned long multR, unsigned long addR);

    void setpokeRNG();

    void setxdRNG();

    void setaRNG();

    unsigned long getSeed();
    
    unsigned long next32Bit();

    unsigned long next16Bit();
        
    unsigned long prev32Bit();

    unsigned long prev16Bit();
        
    unsigned long advanceFrames(int frames);
    
    unsigned long reverseFrames(int frames);

};

#endif //LCRNG_HPP
