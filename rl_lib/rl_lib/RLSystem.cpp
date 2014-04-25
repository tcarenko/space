#include "RLSystem.h"


void RL::RLSystem::setExperiment(RLEnvironment* const _environment){
	environment = _environment;
}

void RL::RLSystem::nextIteration(){
	/*
	 the agent is currently at 
	-the state prState 
	-choose the action 
	-get the new state from environment 
	-gets the response from the environment
	-get new Q value for the TRANSITION - Q value of the prState (of the certain action) will be updated
	(we get new value for the TRANSITION from prState to nextState)
	-put new Q value to a QTable
	*/

	static RLState  *prState = 0;
	RLState *nextState	= 0;
	RLAction *action	= 0;
	double reward		= 0; 
	double newQValue	= 0;
	double oldQValue	= 0;

	//make the agent act, e.g. get the next action from it
	action = agent ->act(prState);
	//get the state from the environment, applying the action
	nextState  = environment ->getState(action);
	//get reward from the environment
	reward = environment ->getFeedback(nextState);
	//having the reward, present state, next state, update the Q table
	newQValue = learning ->update(prState, nextState, action, reward);
	//put the new Q value in the table
	qTable ->put(prState, action, newQValue);

	tick++;
	/*check the terminal condition here. Q values will converge to Q* values - if the changes between the next and
	the previous iteration is less than some threshold we terminate the learning process*/

}