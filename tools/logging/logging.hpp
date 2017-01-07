#ifndef LOGGING_HPP_
#define LOGGING_HPP_

#include <iostream>
#include <fstream>

using namespace std;

class Logger {
	private:
		static ofstream logFile;
		static ofstream dataFile;

	public:
		static void open();
		static void openData();
		static void close();
		static void info( string msg, bool newLine = true, bool infoPrefix = true );
		static void warning( string msg );
		static void error( string msg );
		static void str( string msg, bool hide = false );
		static void data( string msg );
};

#endif /* LOGGING_HPP_ */
