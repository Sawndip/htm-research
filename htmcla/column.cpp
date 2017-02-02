#include "column.hpp"
#include "htmcla.hpp"

Column::Column() {
	this->ci = -1;
	this->cj = -1;
    this->overlap = 0.0;
    this->active = false;
    this->boost = 1.0;
    this->overlapity = 0.0;
    this->activity = 0.0;
}

double Column::calculateOverlap( bool isDistanceDependent, double alpha ) {
	double overlap = 0.0;
	auto *synapses = proximal.getSynapses();

	if ( !isDistanceDependent ) {
		for ( auto syn : (*synapses) ) {
			if ( syn.isExcitatory() && syn.isConnected() ) {
				overlap += syn.getInputValue( );
			}
		}
	} else {
		// TODO
	}
	return overlap;
}

double Column::calculateInhibition( Column** columns, double alpha ) {
	double inhibition = 0.0;
	auto *synapses = this->proximal.getSynapses();

	for ( auto syn : (*synapses) ) {
		if ( syn.isInhibitory() && syn.isConnected() ) {
			inhibition += (columns[syn.getI()][syn.getJ()].getOverlap() - this->getOverlap());
		}
	}
	return inhibition;
}

double Column::calculateRFRadius( ) {
	auto *synapses = this->proximal.getSynapses();
	int mini = 1000000, maxi = 0;
	int minj = 1000000, maxj = 0;

	if ( synapses->size() > 0 ) {
		for ( auto syn : (*synapses) ) {
			if ( syn.isExcitatory() && syn.isConnected() ) {
			   auto ii = syn.getI( );
			   auto jj = syn.getJ( );

			   if ( ii < mini ) mini = ii;
			   if ( ii > maxi ) maxi = ii;
			   if ( jj < minj ) minj = jj;
			   if ( jj > maxj ) maxj = jj;
			}
		}
		return ((maxi - mini) + (maxj - minj) + 2) / 4;
	}
	return 0.0;
}

int Column::countConnectedSynapses( ) {
	auto *synapses = this->proximal.getSynapses();
	int res = 0;

	for ( auto syn : (*synapses) ) {
		if ( syn.isExcitatory() && syn.isConnected() ) {
			res++;
		}
	}
	return res;
}

void Column::addCell( Cell c ) {
	cells.push_back(c);
}

void Column::addSynapse( Synapse s ) {
	this->proximal.addSynapse(s);
}

void Column::setCenter( double ci, double cj ) {
	this->ci = ci;
	this->cj = cj;
}

void Column::setOverlap( double overlap ) {
	this->overlap = overlap;
}

void Column::setBoostedOverlap( double overlap ) {
	this->overlap = this->boost * overlap;
}

void Column::setActive( bool active ) {
	this->active = active;
}

void Column::setBoost( double boost ) {
	this->boost = boost;
}

void Column::incBoost( double dBoost ) {
	this->boost += dBoost;
}

void Column::setOverlapity( double overlapity ) {
	this->overlapity = overlapity;
}

void Column::setActivity( double activity ) {
	this->activity = activity;
}

double Column::getCi( ) {
	return this->ci;
}

double Column::getCj( ) {
	return this->cj;
}

std::vector<Cell>* Column::getCells() {
	return &this->cells;
}

std::vector<Synapse>* Column::getSynapses() {
	return this->proximal.getSynapses();
}

double Column::getOverlap( ) {
	return this->overlap;
}

bool Column::isActive( ) {
	return this->active;
}

double Column::getBoost( ) {
	return this->boost;
}

double Column::getOverlapity( ) {
	return this->overlapity;
}

double Column::getActivity( ) {
	return this->activity;
}

Mat Column::getReceptiveField( ) {
	const size_t inputHeight = 16;
	const size_t inputWidth = 16;
	Mat res = Mat::zeros( inputHeight, inputWidth, CV_16UC1 );
	auto *synapses = this->proximal.getSynapses();

	for ( auto syn : (*synapses) ) {
		if ( syn.isExcitatory() && syn.isConnected() ) {
			res.at<uint16_t>(syn.getI(),syn.getJ()) = 1;
		}
	}
	return res;
}

DendriteSegment& Column::getProximalDendrite( ) {
	return this->proximal;
}
