

#ifndef SIMULATION_H
#define SIMULATION_H

#define N_AGENTS 1000
#define N_ITERATIONS 2000
#define N_NEIGHBORHOODS 10
#define SOCIAL_MOBILITY  0.1

#include "agent.h"

void runSimulation();
void runInteration(Agent **neighborhoods, int n_agents_neighborhood[N_NEIGHBORHOODS]);
void shuffleFreeMarket(Agent **market, int n);
void shuffelNeighborhoods(Agent **neighborhoods, int n_agents_neighborhood[N_NEIGHBORHOODS]);
float opportunityCost(int marketSize);
void printValues(Agent **neighborhoods, int n_agents_neighborhood[N_NEIGHBORHOODS], int iteration);


#endif