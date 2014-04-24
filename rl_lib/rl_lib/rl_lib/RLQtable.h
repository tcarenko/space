#pragma once
#ifndef RL_QTABLE_H
#define RL_QTABLE_H

#include "RLState.h"
#include "RLAction.h"

namespace RL {
	///RLQTable pure abstract class
	class RLQtable {
	public:
		/* puts value into the table
		*/
		virtual void put(RLState* const , RLAction* const , double) = 0;

		/*gets value from the table
		*/
		virtual double get(RLState* const, RLAction* const) = 0;
	};
}
#endif