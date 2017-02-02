#include "dendrite.hpp"

/* This routine returns the number of connected synapses on segment s
   that are active due to the given state at time t is greater than
   activationThreshold. The parameter state can be activeState, or learnState. */
bool DendriteSegment::getActiveState( int t, CellState state ) const {
	double activity;

	for ( auto syn : this->synapses ) {
		if ( syn.isConnected() ) {
            // Compute the number of connected synapses on segment s
            // that are active due to the given state at time t
			if ( state == CellState::ACTIVE_STATE ) {
				// TODO
			} else if ( state == CellState::LEARN_STATE ) {
				// TODO
			}
		}
	}
	return true;
}
