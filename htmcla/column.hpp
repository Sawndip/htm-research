#ifndef COLUMN_HPP_
#define COLUMN_HPP_

#include "cell.hpp"
#include "dendrite.hpp"

#include <opencv2/opencv.hpp>

using namespace cv;

class Column {
	private:
		// Center over input
		double ci, cj;
		// Cells
		vector<Cell> cells;
		// Proximal dendrite segment
		DendriteSegment proximal;
		// Overlap with the current input
		double overlap;
		// Column's state, i.e. inactive or active
		bool active;
		// Boosting factor
		double boost;
		// Overlap duty cycle
		double overlapity;
		// Activity duty cycle
		double activity;

	public:
		// Default constructor
		Column( );
		// Calculate
		double calculateOverlap( bool isDistanceDependent = false, double alpha = 0.0 );
		double calculateInhibition( Column** columns, double alpha = 0.0 );
		double calculateRFRadius( );
		int countConnectedSynapses( );
		// Setters
		void addCell( Cell c );
		void addSynapse( Synapse s );
		void setCenter( double ci, double cj );
		void setOverlap( double overlap );
		void setBoostedOverlap( double overlap );
		void setActive( bool active );
		void setBoost( double boost );
		void incBoost( double dBoost );
		void setOverlapity( double overlapity );
		void setActivity( double activity );
		// Getters
		const Cell& operator [ ]( const int i ) const {
			return cells[i];
		}
		double getCi( );
		double getCj( );
		std::vector<Cell>* getCells();
		std::vector<Synapse>* getSynapses();
		double getOverlap( );
		bool isActive( );
		double getBoost( );
		double getOverlapity( );
		double getActivity( );
		Mat getReceptiveField( );
		DendriteSegment& getProximalDendrite( );
};

#endif /* COLUMN_HPP_ */
