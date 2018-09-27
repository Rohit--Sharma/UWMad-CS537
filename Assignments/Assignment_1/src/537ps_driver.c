///////////////////////////////////////////////////////////////////////////////
//
// Description:     Driver method where execution begins
// Course:          CS537 - Introduction to Operating Systems (Fall-2018)
// Organization:    University of Wisconsin-Madison
//
// Authors:         Rohit Kumar Sharma, M. Giri Prasanna
// Email:           rsharma@cs.wisc.edu, mugundakrish@wisc.edu
// Created on:      September 26, 2018
//
///////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "537ps_header.h"

/** The entry point of code. It just calls options_processor()
 * 		where rest of the processing happens
 *  :param argc: number of commandline arguments passed
 *  :param argv: array of all the argc commandline arguments
 */
int main(int argc, char *argv[])
{
	options_processor(argc, argv);
	return 0;
}
