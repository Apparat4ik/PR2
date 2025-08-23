#pragma once

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
    
    CONFRACT,
    
    UNKNOWN,
    BACK,
    EXIT
};


void Classic_Solve();
void Log_solve();

void EuclidSolve();

void LaunchSypher();
void LaunchWithAtack();

void PrintSolution();



