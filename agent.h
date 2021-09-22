

#ifndef AGENT_H
#define AGENT_H
#include <stdbool.h>

#define PAYOUT_T(OpportunityCost) 1 - 0.5*OpportunityCost
#define PAYOUT_R(OpportunityCost) 0.7 - 0.5*OpportunityCost
#define PAYOUT_P -0.2
#define PAYOUT_S -0.5
#define EXIT_PAYOFF_X -0.2



typedef struct Agent {
  float location;  //value between 0 and 1, probability to enter the global market 
  float trust; //likelyhood to enter the game
  float trustworthiness; //likelihood to cooperate in the game

  float totalPayout; //amount of total payout received in the simulation

  bool newcommer; //true if agent is a newcommer
  bool inMarket;  //true if agent decide to go to open market
  bool trusted;  //true if agent decidet to play PD in last interation
  bool cooperated;  //true if agent cooperated in last interattion
  bool participated; //true if agent participated in this iteration
}Agent;

Agent createAgent();
float uniformRandZeroOne();
void playPD(Agent *agent1, Agent *agent2, float opportunityCost);
void reinforcementLearning(Agent *agent, float payout, bool played);
void reinforcementLearningOneVariable(float *variable, float payout);

#endif