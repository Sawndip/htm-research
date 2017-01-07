#include "logging.hpp"

#include <fstream>
#include <sstream>

using namespace std;

ofstream Logger::logFile;
ofstream Logger::dataFile;

void Logger::open() {
	stringstream fileName;
	fileName << "experiment" << ".log";
	logFile.open( fileName.str().c_str(), std::ios::out );
}

void Logger::openData() {
	stringstream fileName;
	fileName << "data" << ".log";
	dataFile.open( fileName.str().c_str(), std::ios::out );
}

void Logger::close() {
	if (logFile.is_open()) {
		logFile.close();
	}
	if (dataFile.is_open()) {
		dataFile.close();
	}
}

void Logger::info( string msg, bool newLine, bool infoPrefix ) {
	if (infoPrefix) {
		cout << "[INFO] ";
		logFile << "[INFO] ";
	}
	cout << msg;
	logFile << msg;
	if (newLine) {
		cout << endl;
		logFile << endl;
	}
	cout << std::flush;
	logFile << std::flush;
}

void Logger::warning( string msg ) {
	cout << "[WARN] " << msg << endl;
	logFile << "[WARN] " << msg << endl << std::flush;
}

void Logger::error( string msg ) {
	cout << "[ERROR] " << msg << endl;
	logFile << "[ERROR] " << msg << endl << std::flush;
}

void Logger::str( string msg, bool hide ) {
	if (!hide) {
		cout << msg;
	}
	logFile << msg << std::flush;
}

void Logger::data( string msg ) {
	dataFile << msg << std::flush;
}
