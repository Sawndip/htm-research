#include "htmcla.hpp"
#include "region.hpp"

#include <fstream>

// Constructor
Region::Region( size_t height, size_t width, size_t cellsPerColumn ) {
	this->init(height, width, cellsPerColumn);
}

// Initialize region (without any connections)
void Region::init( size_t height, size_t width, size_t cellsPerColumn ) {
	this->height		 = height;
	this->width			 = width;
	this->cellsPerColumn = cellsPerColumn;
    this->columns		 = new Column*[this->height];
    this->dataSource	 = nullptr;

    // Create column grid
    for ( size_t i = 0; i < this->height; i++ ) {
        this->columns[i] = new Column[this->width];
        for ( size_t j = 0; j < this->width; j++ ) {
        	// Add cells to column
        	for ( size_t k = 0; k < cellsPerColumn; k++ ) {
        		this->columns[i][j].addCell(Cell());
        	}
        }
    }
}

// Set data source and update column grid
void Region::setDataSource( DataSource *src, double alpha ) {
	this->dataSource = src;

	// Set offsets and scaling factors
	const size_t sensoryInputHeight = this->dataSource->getHeight();
	const size_t sensoryInputWidth = this->dataSource->getWidth();
    const double dh = sensoryInputHeight * alpha;
    const double dw = sensoryInputWidth * alpha;
    const int oci = this->height / sensoryInputHeight;
    const int ocj = this->width / sensoryInputWidth;
    double di = (sensoryInputHeight - 2 * dh) * oci / (this->height - 1);
    double dj = (sensoryInputWidth - 2 * dw) *ocj / (this->width - 1);

    // Update column grid
    for ( size_t i = 0; i < this->height; i++ ) {
        for ( size_t j = 0; j < this->width; j++ ) {
            // Set column's center over input
        	this->columns[i][j].setCenter( dh + di * i / oci, dw + dj * j / ocj );
        }
    }
}

// Save region
void Region::save( std::string fileName ) {
	ofstream myfile(fileName.c_str());

	if ( myfile.is_open() ) {
		// Save region size and sensory input size
		myfile << this->height << " " << this->width << endl;
		// Save information about the columns
		for ( size_t i = 0; i < this->height; i++ ) {
			for ( size_t j = 0; j < this->width; j++ ) {
				auto *synapses = this->columns[i][j].getSynapses();
				myfile << i << " " << j << " " << this->columns[i][j].getBoost() << " " << synapses->size() << " ";
				// Save information about synapses
				for ( auto syn : (*synapses) ) {
					myfile << (( syn.isExcitatory() )? 'e' : 'i') << " " <<
						syn.getI() << " " << syn.getJ() << " " << syn.getPermanence();
				}
				myfile << endl;
			}
		}
		myfile.close();
	} else {
		cout << "Failure: unable to open file to save the region" << endl;
	}
}

// Load region
void Region::load( std::string fileName ) {
	ifstream myfile(fileName.c_str());

	if ( myfile.is_open() ) {
		size_t height, width;
		size_t i, j, ii, jj;
		size_t numOfSynapses;
		uchar type;
		double permanence, boost;

		// Get the region size
		myfile >> height;
		myfile >> width;

		// Initialize the region
		init( height, width, 1 );

		// Initialize all the columns
		while ( myfile >> i ) {
			myfile >> j;
			myfile >> boost;
			this->columns[i][j].setBoost( boost );
			myfile >> numOfSynapses;

			// Load excitatory and inhibitory synapses
			this->columns[i][j].getSynapses()->clear();
			for ( size_t syn = 0; syn < numOfSynapses; syn++ ) {
				myfile >> type;
				myfile >> ii;
				myfile >> jj;
				myfile >> permanence;

				if ( type == 'e' ) {
					Synapse synapse(ii, jj, 0, this->getDataSource(), SynapseType::EXCITATORY);
					synapse.setPermanence(permanence);
					this->columns[i][j].addSynapse(synapse);
				} else if ( type == 'i' ) {
					Synapse synapse(ii, jj, 0, nullptr, SynapseType::INHIBITORY);
					synapse.setPermanence(permanence);
					this->columns[i][j].addSynapse(synapse);
				}
			}
		}
		myfile.close();
	} else {
		cout << "Failure: unable to open a file to load the region" << endl;
	}
}

double Region::getValue( size_t i, size_t j, size_t k ) const {
	return (this->columns[i][j][k].getState(CellState::ACTIVE_STATE, 1))? 1.0 : 0.0;
}

// Calculate mean number of connected synapses for individual column
double Region::calculateMeanConnectedSynapses( ) const {
	double meanConnectedSynapses = 0.0;

    // Look for all connected synapses and calculate mean number for individual column
    for ( size_t i = 0; i < this->height; i++ ) {
        for ( size_t j = 0; j < this->width; j++ ) {
        	meanConnectedSynapses += this->columns[i][j].countConnectedSynapses( );
        }
    }
    meanConnectedSynapses /= (this->height * this->width);
    return meanConnectedSynapses;
}

// Visualize receptive fields
Mat Region::visualize( bool showActiveColumns ) const {
	const size_t sensoryInputHeight = this->dataSource->getHeight();
	const size_t sensoryInputWidth = this->dataSource->getWidth();
	Mat img = Mat::zeros(
		this->height * sensoryInputHeight + this->height + 1,
		this->width * sensoryInputWidth + this->width + 1,
		CV_8UC3
	);
	// Set background
	img.setTo(cv::Scalar(150, 150, 150));
	// Visualize grid
	for ( size_t i = 0; i < img.rows; i += sensoryInputHeight + 1 ) {
		for ( size_t j = 0; j < img.cols; j++ ) {
			img.at<Vec3b>(i,j) = Vec3b(70, 70, 70);
		}
	}
	for ( size_t i = 0; i < img.rows; i++ ) {
		for ( size_t j = 0; j < img.cols; j += sensoryInputWidth + 1 ) {
			img.at<Vec3b>(i,j) = Vec3b(70, 70, 70);
		}
	}
	// Visualize receptive fields
	for ( size_t i = 0; i < height; i++ ) {
		for ( size_t j = 0; j < width; j++ ) {
			size_t di = i * sensoryInputHeight + (i + 1);
			size_t dj = j * sensoryInputWidth + (j + 1);

			if ( showActiveColumns && columns[i][j].isActive( ) ) {
				rectangle( img, Point( dj - 1, di - 1 ), Point( dj + sensoryInputWidth, di + sensoryInputHeight ),
				           Scalar( 0, 0, 255 ), 1, 8 );
			}

			auto *synapses = this->columns[i][j].getSynapses();
			for ( auto syn : (*synapses) ) {
				if ( syn.isExcitatory() && syn.isConnected() ) {
					if ( syn.getK() == 0) {
						img.at<Vec3b>(di + syn.getI( ), dj + syn.getJ( )) = Vec3b(255, 255, 255);
					} else {
						img.at<Vec3b>(di + syn.getI( ), dj + syn.getJ( )) = Vec3b(0, 0, 0);
					}
				}
			}
		}
	}
	return img;
}

// Decode input patch
Mat Region::decode( Mat activeColumns ) const {
	const size_t sensoryInputHeight = this->dataSource->getHeight();
	const size_t sensoryInputWidth = this->dataSource->getWidth();
	Mat res = Mat::zeros( sensoryInputHeight, sensoryInputWidth, CV_16UC1 );
	for ( size_t i = 0; i < this->height; i++ ) {
		for ( size_t j = 0; j < this->width; j++ ) {
			size_t z = (i * this->width + j) / 8;
			size_t bit = (i * this->width + j) % 8;
			uchar r = activeColumns.at<uchar>(1,z) & (0x01 << bit);

			if ( r != 0 ) {
				Mat rf = this->columns[i][j].getReceptiveField( );
				res += rf;
			}
		}
	}
	return res;
}

// Decode input patch
Mat Region::decode( bitvector activeColumns ) const {
	const size_t sensoryInputHeight = this->dataSource->getHeight();
	const size_t sensoryInputWidth = this->dataSource->getWidth();
	Mat res = Mat::zeros( sensoryInputHeight, sensoryInputWidth, CV_16UC1 );
	for ( size_t i = 0; i < this->height; i++ ) {
		for ( size_t j = 0; j < this->width; j++ ) {
			if ( activeColumns.at( i * this->width + j ) ) {
				Mat rf = this->columns[i][j].getReceptiveField( );
				res += rf;
			}
		}
	}
	return res;
}
// Print region statistics
void Region::printStatistics() const {
	int numOkCCol[2] = {0, 0};
	int numOkDCol[2] = {0, 0};
	double totalCPerm[2] = {0.0, 0.0};
	double totalDPerm[2] = {0.0, 0.0};
	int totalCSyn[2] = {0, 0};
	int totalDSyn[2] = {0, 0};

	for ( size_t i = 0; i < this->height; i++ ) {
		for ( size_t j = 0; j < this->width; j++ ) {
			auto *synapses = this->columns[i][j].getSynapses( );
			double sumCPerm[2] = {0.0, 0.0};
			double sumDPerm[2] = {0.0, 0.0};
			int numCSyn[2] = {0, 0};
			int numDSyn[2] = {0, 0};

			for ( auto syn : (*synapses) ) {
				size_t k = ( syn.isExcitatory() )? 0 : 1;
				if ( syn.isConnected() ) {
					numCSyn[k]++;
					sumCPerm[k] += syn.getPermanence();
				} else {
					numDSyn[k]++;
					sumDPerm[k] += syn.getPermanence();
				}
			}

			for ( size_t k = 0; k < 2; k++ ) {
				totalCSyn[k] += numCSyn[k];
				totalDSyn[k] += numDSyn[k];
				if ( numCSyn[k] > 0 ) {
					totalCPerm[k] += sumCPerm[k] / numCSyn[k];
					numOkCCol[k]++;
				}
				if ( numDSyn[k] > 0 ) {
					totalDPerm[k] += sumDPerm[k] / numDSyn[k];
					numOkDCol[k]++;
				}
			}
		}
	}

	double statistics[5][2];
	for ( size_t k = 0; k < 2; k++ ) {
		statistics[0][k] = static_cast<double>(totalCSyn[k]) / (this->height * this->width);
		statistics[1][k] = static_cast<double>(totalDSyn[k]) / (this->height * this->width);
		statistics[2][k] = totalCPerm[k] / numOkCCol[k];
		statistics[3][k] = totalDPerm[k] / numOkDCol[k];
		statistics[4][k] = numOkCCol[k];
	}
	cout.precision(2);
	cout << "**************************************************************" << endl;
	cout << "Region statistics of excitatory/inhibitory connections:" << endl;
	cout << "- Average num. of connected synapses\t\t" << fixed << statistics[0][0] << "/" << statistics[0][1] << endl;
	cout << "- Average num. of disconnected synapses\t\t" << fixed << statistics[1][0] << "/" << statistics[1][1] << endl;
	cout << "- Average perm. of connected synapses\t\t" << fixed << statistics[2][0] << "/" << statistics[2][1] << endl;
	cout << "- Average perm. of disconnected synapses\t" << fixed << statistics[3][0] << "/" << statistics[3][1] << endl;
	cout << "- Num. of columns with connected synapses\t" << static_cast<int>(statistics[4][0]) << "/" << static_cast<int>(statistics[4][1]) << endl;
}

void Region::clone( Region *region ) const {
	region->init( this->height, this->width, this->cellsPerColumn );
	region->setDataSource( this->dataSource );
	for ( size_t i = 0; i < this->height; i++ ) {
		for ( size_t j = 0; j < this->width; j++ ) {
			auto *synapses = columns[i][j].getSynapses();
			for ( auto syn : (*synapses) ) {
				Synapse synapse(syn.getI(), syn.getJ(), 0, this->dataSource, syn.getType());
				synapse.setPermanence( syn.getPermanence() );
				(*region)[i][j].addSynapse( synapse );
			}
		}
	}
}
