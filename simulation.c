
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "simulation.h"


void runSimulation(){
  if (N_AGENTS % N_NEIGHBORHOODS !=0){
    printf("ERROR: N_AGENTS mod N_NEIGHBORHOODS != 0\n");
    return;
  }

  //init neighborhood
  Agent **neighborhoods = malloc(sizeof(Agent*) * N_NEIGHBORHOODS); //stores all neighborhoods with all agents
  int n_agents_neighborhood[N_NEIGHBORHOODS]; //stores number of agents in each neighborhood
  for (int i =0; i < N_NEIGHBORHOODS; i++){
    neighborhoods[i] =  malloc(sizeof(Agent) * N_AGENTS); //alloc N_AGENTS entries in case all agents move to the same neighborhood.
    for (int j = 0; j < N_AGENTS / N_NEIGHBORHOODS; j++){
      neighborhoods[i][j] = createAgent();
    }
    n_agents_neighborhood[i] = N_AGENTS / N_NEIGHBORHOODS;
  }


  //run iterations
  for (int i = 0; i < N_ITERATIONS; i++){
    runInteration(neighborhoods, n_agents_neighborhood);
    printValues(neighborhoods,n_agents_neighborhood,i);
  }


  //free memory
  for (int i =0; i < N_NEIGHBORHOODS; i++){
    free(neighborhoods[i]);
  }
  free(neighborhoods);

}

void runInteration(Agent **neighborhoods, int n_agents_neighborhood[N_NEIGHBORHOODS]){

  int n_agent_in_market = 0;
  Agent **agentsInMarket = malloc(sizeof(Agent *)* N_AGENTS);


  //let agents change neighborhood
  int newNeighborhood;
  for (int i =0; i < N_NEIGHBORHOODS; i++){
    for (int j = 0; j < n_agents_neighborhood[i]; j++){
      if (uniformRandZeroOne() < SOCIAL_MOBILITY){
        newNeighborhood = rand() % N_NEIGHBORHOODS;

        neighborhoods[newNeighborhood][n_agents_neighborhood[newNeighborhood]] = neighborhoods[i][j];
        neighborhoods[newNeighborhood][n_agents_neighborhood[newNeighborhood]].newcommer = true;
        n_agents_neighborhood[newNeighborhood] ++;

        neighborhoods[i][j] = neighborhoods[i][n_agents_neighborhood[i]-1];
        n_agents_neighborhood[i]--;
      }else{
        neighborhoods[i][j].newcommer = false;
      }
    }
  }


  //choose location for agents
  for (int i =0; i < N_NEIGHBORHOODS; i++){
    for (int j = 0; j < n_agents_neighborhood[i]; j++){
      if (neighborhoods[i][j].newcommer || neighborhoods[i][j].location > uniformRandZeroOne()){
        neighborhoods[i][j].inMarket = true;
        agentsInMarket[n_agent_in_market] = &neighborhoods[i][j];
        n_agent_in_market++;
      }else{
        neighborhoods[i][j].inMarket = false;
      }
    }
  }


  //pairing agents in free market
  shuffleFreeMarket(agentsInMarket,n_agent_in_market);
  float opportunityCostMarket = opportunityCost(n_agent_in_market);
  for (int i = 0; i < n_agent_in_market-1; i+=2){
    playPD(agentsInMarket[i], agentsInMarket[i+1], opportunityCostMarket);
    agentsInMarket[i]->participated = true;
    agentsInMarket[i+1]->participated = true;
  }
  if (n_agent_in_market % 2 == 1){
    agentsInMarket[n_agent_in_market-1]->participated = false;
  }

  //pairing agents in neighborhoods 
  int k;
  shuffelNeighborhoods(neighborhoods, n_agents_neighborhood);
  float opportunityCostNeighborhood;
  for (int i =0; i < N_NEIGHBORHOODS; i++){
    opportunityCostNeighborhood = opportunityCost(n_agents_neighborhood[i]);
    for (int j = 0; j < n_agents_neighborhood[i]; j++){
      k = j;
      while  (j + 1 != n_agents_neighborhood[i] && neighborhoods[i][j+1].inMarket){
        j++;
      }

      if (j+1 != n_agents_neighborhood[i]){
        playPD(&neighborhoods[i][k],&neighborhoods[i][j+1], opportunityCostNeighborhood);
        neighborhoods[i][k].participated = true;
        neighborhoods[i][j+1].participated = true;
      }else{
        neighborhoods[i][k].participated = false;
      }
    }
  } 

  free(agentsInMarket);
}

void shuffleFreeMarket(Agent **market, int n){
  Agent *temp;
  int randInt;
  for (int i =0; i < n-1; i++){
    randInt = rand() % (n - i) + i;
    temp = market[i];
    market[i] = market[randInt];
    market[randInt] = temp;
  }
}

//randomly shuffle agents
void shuffelNeighborhoods(Agent **neighborhoods, int n_agents_neighborhood[N_NEIGHBORHOODS]){

  int temp;
  Agent tempAgent;
  Agent *tempNeighborhood;
  int randInt;

  //randomly shuffle within neighborhood
  for (int i = 0; i < N_NEIGHBORHOODS; i++){
    for (int j =0 ; j < n_agents_neighborhood[i] -1; j++){
      randInt = rand() % (n_agents_neighborhood[i] - j) + j;
      tempAgent = neighborhoods[i][j];
      neighborhoods[i][j] = neighborhoods[i][randInt];
      neighborhoods[i][randInt] = tempAgent;
    }
  }
}

//calculate opportunity cost
float opportunityCost(int marketSize){
  return 1 - (((float)marketSize - 1) / (N_AGENTS -1) ); 
}

void printValues(Agent **neighborhoods, int n_agents_neighborhood[N_NEIGHBORHOODS], int iteration){
  float averageLocation =0, averageTrust = 0,averageTrustfulness = 0;
  Agent agent;
  float n_agentsParticipated = N_AGENTS;
  for (int i=0; i < N_NEIGHBORHOODS; i++){
    for (int j = 0; j < n_agents_neighborhood[i]; j++){
      agent = neighborhoods[i][j];
      if (agent.participated == false){
        n_agentsParticipated--;
        continue;
      }

      averageLocation += agent.inMarket == true;
      averageTrust += agent.trusted == true;
      averageTrustfulness += agent.cooperated == true;
    }
  }
  averageTrust /= n_agentsParticipated;
  averageTrustfulness /= n_agentsParticipated;
  averageLocation /= n_agentsParticipated;
  printf("\tIteration %d: Went to market = %f, Trusted = %f, Cooperated = %f \n"
    ,iteration,averageLocation, averageTrust,averageTrustfulness );
}
