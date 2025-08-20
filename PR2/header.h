#include <iostream>
#include <vector>
#include <stdint.h>
#include <random>
#include <cmath>
#include <sstream>
#include <limits>
#include <cstdint>
#include <cassert>

using namespace std;

enum class Sellection{
    AXMODP,
    
    CLASSIC,
    LOG,

    EUCLID,
    
    ELGAMAL,
    
    ATACK,
    NOATTACK,
    
    CONFRACT
};
