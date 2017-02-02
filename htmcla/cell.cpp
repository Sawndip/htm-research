#include "cell.hpp"

void Cell::setState( CellState state, int t, bool value ) {
	if ( state == CellState::ACTIVE_STATE ) {
		this->activeState[t] = value;
	} else if ( state == CellState::PREDICTIVE_STATE ) {
		this->predictiveState[t] = value;
	} else if ( state == CellState::LEARN_STATE ) {
		this->learningState[t] = value;
	}
}

bool Cell::getState( CellState state, int t ) const {
	if ( state == CellState::ACTIVE_STATE ) {
		return this->activeState[t];
	} else if ( state == CellState::PREDICTIVE_STATE ) {
		return this->predictiveState[t];
	} else if ( state == CellState::LEARN_STATE ) {
		return this->learningState[t];
	}
	return false;
}
