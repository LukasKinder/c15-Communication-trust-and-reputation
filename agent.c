
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "agent.h"

float uniformRandZeroOne(){
  return (float)rand() / (float)RAND_MAX ;
}

Agent createAgent(){
  Agent agent;
  agent.location = uniformRandZeroOne();
  agent.trust = uniformRandZeroOne();
  agent.trustworthiness = uniformRandZeroOne();
  agent.newcommer = false;
  return agent;
}

void playPD(Agent *agent1, Agent *agent2, float opportunityCost){
  agent1->trusted = uniformRandZeroOne() < agent2->trustworthiness 
      ||  uniformRandZeroOne() < agent1->trust? true : false;
  agent2->trusted = uniformRandZeroOne() < agent1->trustworthiness 
      ||  uniformRandZeroOne() < agent2->trust? true : false;
    
  agent1->cooperated = uniformRandZeroOne() < agent1->trustworthiness;
  agent2->cooperated = uniformRandZeroOne() < agent2->trustworthiness;

  float payoutAgent1, payoutAgent2;
  if (!agent1->trusted || ! agent2->trusted){
    payoutAgent1 = EXIT_PAYOFF_X;
    payoutAgent2 = EXIT_PAYOFF_X;
  }else{
    if (agent1->cooperated && agent2->cooperated){
      payoutAgent1 = PAYOUT_R(opportunityCost);
      payoutAgent2 = PAYOUT_R(opportunityCost);
    }else if (!agent1->cooperated && agent2->cooperated){
      payoutAgent1 = PAYOUT_T(opportunityCost);
      payoutAgent2 = PAYOUT_S;
    }
    else if (agent1->cooperated && !agent2->cooperated){
      payoutAgent1 = PAYOUT_S;
      payoutAgent2 = PAYOUT_T(opportunityCost);
    }else{
      payoutAgent1 = PAYOUT_P;
      payoutAgent2 = PAYOUT_P;
    }
  }

  agent1->totalPayout += payoutAgent1;
  agent2->totalPayout += payoutAgent2;

  reinforcementLearning(agent1,payoutAgent1);
  reinforcementLearning(agent2,payoutAgent2);
}

void reinforcementLearning(Agent *agent, float payout){
  if (agent->inMarket){
    reinforcementLearningOneVariable(&(agent->location), payout);
  }else{
    reinforcementLearningOneVariable(&(agent->location), payout * (-1));
  }

  if (agent->trusted){
    reinforcementLearningOneVariable(&(agent->trust), payout);
  }else{
    reinforcementLearningOneVariable(&(agent->trust), payout * (-1));
  }

  if (agent->cooperated){
    reinforcementLearningOneVariable(&(agent->trustworthiness), payout);
  }else{
    reinforcementLearningOneVariable(&(agent->trustworthiness), payout * (-1));
  }
}

void reinforcementLearningOneVariable(float *variable, float payout){
  if (payout > 0){
    *variable = *variable + (1-*variable)*payout;
  }else{
    *variable = *variable + *variable*payout;
  }

}
