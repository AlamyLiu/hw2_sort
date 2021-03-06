/*
 * main() entry
 */

#include <cerrno>
#include <iostream>
#include <string>
#include <fstream>		// ifstream, ofstream
#include <stdlib.h>		// atoi
#include <getopt.h>		// getopt_long()
#include <unistd.h>		// getopt()
//#include <regex>		// regex
#include <list>			// list of SIDLList class
#include "SIntFormat.hpp"	// validating Signed Integer format
#include "sidll.hpp"		// Double Linked-List
#include "MultiSort.hpp"	// Sorting methods
#include "DebugW.hpp"		// Debug

using namespace std;

const struct option long_opts[] =
{
	{"input",	required_argument,	0,	'i'},
	{"digitsPerNode",required_argument,	0,	'n'},
	{"algorithm",	required_argument,	0,	'a'},
	{"output",	required_argument,	0,	'o'},
	{"debug",	no_argument,		0,	'd'},
	{"test",	no_argument,		0,	't'}, /* internal */
	{0,0,0,0},
};

typedef struct _OPT_FLAGS {
	string	iFile;		/* Input file */
	string	oFile;		/* Output file */
	int	digitsPerNode;	/* digits per node */
	int	algorithm;	/* <select|insert|merge|heap|quick> */
	bool	debug;		/* Enable debug */
	bool	test;		/* Internal: misc testing function */
} OPT_FLAG;

#if 1
OPT_FLAG	optFlag;
#else
OPT_FLAG	optFlag = {
			.iFile = string( "" ),
			.oFile = string( "" ),
			.digitsPerNode = 1,
			.algorithm = SELECT,
			.debug = false,
			.test = false
		};
#endif

Debug		*dbg;		/* Warning: should be created before List */

bool validate_format( string signInteger )
{
	string str_sign ("+-");
	string str_digit("0123456789");
	int sign = 1;		/* Default to positive '+' */

	size_t pSign  = signInteger.find_first_of( str_sign );
	size_t pValue = signInteger.find_first_not_of( str_sign );

	if (pSign == pValue) {
		/* No sign: default */
	} else if (pSign + 1 != pValue) {
		/* Wrong format */
		if (pValue < pSign) {
			/* [:digit:][+-] */
		} else {
			/* [+-]+[:digit:] */
		}
		cerr << "Wrong Format: " << signInteger << endl;
		return false;
	} else {
		/* pSign < pValue */
		if (signInteger.at(pSign) == '-')
			sign = -1;

		/* If it's not '-', it must be '+' : find_first_of() */
		/* We don't handle '+' case, as it's default case */
	}

	string integer = signInteger.substr(pValue);	/* [[:digit:]] */
	if (integer.find_first_not_of( str_digit ) != std::string::npos) {
		/* [+-][[:digit:]]\w */
		cerr << "Wrong format: " << signInteger << endl;
		return false;
	}

	return true;
}

int main(int argc, char* argv[])
{
	/* Syntax
	   sort "input=<in.txt>;digitsPerNode=<number>;algorithm=<select|insert|merge|heap|quick>;ouput=<out.txt>".
	 */
	if (argc < 2) {
		cerr << "Usage: " << argv[0] << " \"input=<in.txt>;" \
			"digitsPerNode=<number>;" \
			"algorithm=<select|insert|merge|head|quick>;" \
			"output=<out.txt>\"" << endl;
		return -EINVAL;
	}

	// Retrieve options
	int opt_index = 0;
	int opt;
	string algorithm;
	while ( (opt = getopt_long(argc, argv, "dta:i:n:o:",
		long_opts, &opt_index)) != -1) {
		switch (opt) {
		case 'i': optFlag.iFile = string( optarg );	break;
		case 'o': optFlag.oFile = string( optarg );	break;
		case 'n': optFlag.digitsPerNode = atoi(optarg);	break;
		case 'a': /* Algorithm */
			algorithm = string( optarg );
			if (algorithm.compare("select") == 0)
				optFlag.algorithm = SELECT;
			else if (algorithm.compare("insert") == 0)
				optFlag.algorithm = INSERT;
			else if (algorithm.compare("merge") == 0)
				optFlag.algorithm = MERGE;
			else if (algorithm.compare("heap") == 0)
				optFlag.algorithm = HEAP;
			else if (algorithm.compare("quick") == 0)
				optFlag.algorithm = QUICK;
			else /* unknown algorithm */
				optFlag.algorithm = UNKNOWN;
			break;
		case 'd': optFlag.debug = true;		break;
		case 't': optFlag.test = true;		break;
		case '?': /* Unknown option (ignore) */
		default : /* Do nothing */		break;
		} // End of switch(opt)
	} // End of while(opt)

	// Options error detection
	ifstream ifs( optFlag.iFile.c_str() );
	ofstream ofs( optFlag.oFile.c_str(), ios::out | ios::app );
	if (optFlag.iFile.empty()) {
		cerr << "Input file ?" << endl;
		return -EINVAL;
	} else {
		if (! ifs.good()) {
			cerr << "File " << optFlag.iFile \
				<< " does not exist!" << endl;
			return -EINVAL;
		}
	}
	if (optFlag.oFile.empty()) {
		cerr << "Output file ?" << endl;
		return -EINVAL;
	}
	if (optFlag.algorithm == UNKNOWN) {
		cerr << "Unknown algorithm!" << endl;
		return -EINVAL;
	}
	if (optFlag.digitsPerNode < 1) {
		cerr << "digitsPerNode could not be " \
			<< optFlag.digitsPerNode << endl;
		return -EINVAL;
	}

	/* Testing mode: call test() function instead of sorting */
	if (optFlag.test) {
		optFlag.algorithm = UNKNOWN;
	}

	// create Debug class (DEFAULT_DEBUG_LEVEL), borrow 'opt' variable
	opt = (optFlag.debug ? DEBUG_LEVEL_INFO : DEBUG_LEVEL_DEBUG);
	dbg = new (std::nothrow) Debug(opt);
	if (!dbg) {
		cerr << "Unable to create Debug system!" << endl;
		return -ENOMEM;
	}
	*dbg << "Input: " << optFlag.iFile << endl;
	*dbg << "Digits Per Node: " << optFlag.digitsPerNode << endl;
	*dbg << "algorithm: " << optFlag.algorithm << endl;
	*dbg << "Output: " << optFlag.oFile << endl;


	// Parse input file
	MultiSort mySort;
	string line;

	while (ifs >> line) {
		/* Trim trailing ... */
		line.erase(line.find_last_not_of(" \t\n\r\f\v") + 1);
		*dbg << "--- line: " << line << endl;

		/* Validate format /^[+-][[:digit:]]$*/
		SIntFormat fmt(line);
		*dbg << "Format: sign = " << fmt.getSign() \
			<< ", digits = " << fmt.getDigits() << endl;
		if (! fmt.isValid()) {
			cerr << "Invalid format: " << line << " ... skip!" << endl;
			continue;
		}

		/* Create a linked-list containing the number */
		SIDLList* num = new(nothrow)
			SIDLList(fmt.getSign(), fmt.getDigits(), optFlag.digitsPerNode);

		if (! num) {
			cerr << "Why allocation failed ?" << std::endl;
			break;
		}

		*dbg << *num;

		mySort.addItem( num );
//		intList.push_back( num );
//		intList.addInteger( line );
	}
	ifs.close();

	*dbg << mySort.size() << " numbers" << endl;
	*dbg << mySort;


	/* Now it's time to SORT */
	int rc;
	switch (optFlag.algorithm) {
	case SELECT:	rc = mySort.SelectSort();	break;
	case INSERT:	rc = mySort.InsertSort();	break;
	case MERGE:	rc = mySort.MergeSort();	break;
	case HEAP:	rc = mySort.HeapSort();		break;
	case QUICK:	rc = mySort.QuickSort();	break;
	default:
		if (optFlag.test) {
			rc = mySort.test();
			break;
		}

		rc = -EINVAL;
		cerr << "This should not happen!" << endl;
		break;
	} /* End of switch(optFlag.algorithm) */

	/* Output the result */
	if (! rc) {
		/* Now the result */
		cout << "----- Result -----" << endl;
		cout << mySort;

		/* And the statictics */
		mySort.statistics( ofs, algorithm );
	}
	ofs.close();

	// Free resources
	mySort.free();
	delete dbg;

	return 0;
}
