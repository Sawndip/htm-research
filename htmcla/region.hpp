#ifndef REGION_HPP_
#define REGION_HPP_

#include <opencv2/opencv.hpp>

#include "common/types.hpp"
#include "htmcla/column.hpp"
#include "htmcla/htmcla.hpp"
#include "htmcla/synapse.hpp"

using namespace std;
using namespace cv;

class Region : public DataSource {
	private:
		// Column grid
		Column **columns;
		size_t cellsPerColumn;
		// Data source
		DataSource *dataSource;

	public:
		Region( ) = delete;
		// Initialize region
		Region( size_t height, size_t width, size_t cellsPerColumn = 1 );
		void init( size_t height, size_t width, size_t cellsPerColumn );
		void setDataSource( DataSource *src, double alpha = 0.0 );
		// Save/load region to/from file
		void save( std::string fileName );
		void load( std::string fileName );
		// Getters
		inline Column** getColumns( ) const {
			return this->columns;
		}
		inline DataSource* getDataSource( ) const {
			return this->dataSource;
		}
		inline size_t getCellsPerColumn( ) const {
			return this->cellsPerColumn;
		}
		double getValue( size_t i, size_t j, size_t k = 0 ) const override;
		// Get access to the column grid
		Column* operator [ ]( const size_t i ) const {
			return this->columns[i];
		}
		// Calculate mean number of connected synapses
		double calculateMeanConnectedSynapses( ) const;
		// Visualize the receptive fields
		Mat visualize( bool showActiveColumns = false ) const;
		// Decode input patch
		Mat decode( Mat activeColumns ) const;
		Mat decode( bitvector activeColumns ) const;
		// Print region statistics
		void printStatistics( ) const;
		// Clone region
		void clone( Region *region ) const;
};

#endif /* REGION_HPP_ */
