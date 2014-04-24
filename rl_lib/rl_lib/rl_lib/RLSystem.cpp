#include "RLSystem.h"


void RL::RLSystem::setExperiment(RLEnvironment* const _environment){
	environment = _environment;
}

void RL::RLSystem::nextIteration(){
	static RLState  *state = 0, *nextState = 0;
	RLAction *action = 0;
	double reward = 0;

	//get the next action, considering the present state
	action = agent ->getAction(state);
	//make the agent act, e.g. make a transition to the state
	state  = agent ->act(action);
	//get the next state - need to know the next state to make the Update
	nextState = agent ->getNextState();
	//get the response from the environment on the state
	reward = environment ->giveFeedback(state);
	//having the reward, present state, next state, update the Q table
	learning ->update(state, nextState, action, reward);

	tick++;
	//check the terminal condition here:

}