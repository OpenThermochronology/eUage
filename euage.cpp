/*
	Program euage 1.0
	Written by Peter Zeitler in 2014
	C++ to be more easily compatible with Rich Ketcham's RDAAM code.
	
	This program was kludged together by grabbing the wrapper from my crankfunc code,
	just using that to call RDAAM/ZRDAAM. Written only for apatite and zircon.
	
	The wrapper pulls in one input file with required sample data, and another with the
	desired thermal history.
	
	It runs in a loop across a range of eU values, having the same U/Th/Sm proportions as 
	the sample.
	
	Zoning can't be modeled.
	
	usage:
	
	./euage <#of eU values>. <step between eU values>

*/

using namespace std;

#include <cstring>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <time.h>
#include <vector>
#include "tchar.h"
#include "ZRDAAM.h" // from Rich Ketcham

int main(int argc, char * argv[])
{

// declarations
	
	FILE *ofp, *ifp;  // for file I/O
	
	TTPath path;  // for RDAAM, custom data type
	TTPathPoint mypoint;  // for RDAAM, custom data type
	
	int mineral, ttpairs_in;
	long i, ieu;
	double U_sample, TH_sample, SM_sample, U, TH, SM, radius;
	double EUS, ZMAG;

// ***** Start up

	if(argc < 3) // there are only one or fewer values and we need two (besides the program name)
	{
			puts("\n  euage ERROR: not enough parameters");
			puts("  USAGE: ./euage <#eu's< <eU-stepsize-for-zir>\n");
			exit(1);
	}
	if(argc > 3) // there are more than two and thus extra inputs
	{
			puts("\n  euage WARNING: too many parameters - using only first two");
			puts("  USAGE: ./euage <#eu's< <eU-stepsize-for-zir>\n");
	}
		
	printf("\n");

	printf("*** PROGRAM euage() 1.0alpha ***\n\n");

	EUS = atoi(argv[1]);	
	ZMAG = atof(argv[2]);

	vector<double> eu_save(EUS);
	vector<double> raw_age(EUS);
	vector<double> alpha_age(EUS);

// get data from files

	ifp = fopen("euage.in", "r");
	fscanf(ifp, "%d", &mineral);
	assert((mineral >= 0) and (mineral <= 1));  // 0 apatite, 1 zircon
	
	fscanf(ifp, "%lf", &U_sample);		// ppm
	fscanf(ifp, "%lf", &TH_sample);	// ppm
	fscanf(ifp, "%lf", &SM_sample);	// ppm
	
	fscanf(ifp, "%lf", &radius);   // microns

// echo inputs as record of what should be happening
	switch (mineral)  // illegal values ruled out on input
	{
		case 0:    // apatite
			printf("  modeling apatite with spherical geometry, RDAAM, and alpha-ejection\n");
		break;
		case 1:    // zircon
			printf("  modeling zircon with spherical geometry, ZRDAAM, and alpha-ejection\n");
		break;
	}
	
	printf("\n       Radius: %6.2f microns\nU-Th-Sm (ppm): %6.2f %6.2f %6.2f\n\n",radius, U_sample, TH_sample, SM_sample);

	fclose(ifp);

// now get the input thermal history (from file)
	ifp = fopen("euage_history.in", "r");

	fscanf(ifp, "%d", &ttpairs_in);		// oldest pair first
	assert ((ttpairs_in > 1) && (ttpairs_in <= 1001));

	vector<double> intime(ttpairs_in);
	vector<double> intemp(ttpairs_in);
	
	for (i = 0; i < ttpairs_in; i++)
	{
		fscanf(ifp, "%lf%lf",&intime[i], &intemp[i]);
	}
	fclose(ifp);
	
// ***** do the actual work

	double age, corrAge, eU, eU_sample;
	
	eU_sample = U_sample + 0.2376 * TH_sample + 0.0012 * SM_sample;

	for (ieu=0;ieu<EUS;ieu++)
	{
		eU = ZMAG * (static_cast<double> (ieu) + 1.);
		
		if (mineral == 0)  // apatite
		{
			// turn local eU value into its component U, Th, and Sm values, assuming same proportion as sample
			U = U_sample * eU / eU_sample;
			TH = TH_sample * eU / eU_sample;
			SM = SM_sample * eU / eU_sample;

			RDAAM_Init(HE_PREC_BEST, radius, U, TH, SM); // precision, radius, U, Th, Sm
	   
			path.clear();
			for (i = ttpairs_in-1; i >= 0; i--)  // RDAAM wants most recent pair first
			{
				mypoint.time = intime[i];
				mypoint.temperature = intemp[i];
				path.push_back(mypoint);   // Present day is first entry in path
			}	
   
			int success = RDAAM_Calculate(&path, age, corrAge);
		}
		else  // zircon
		{
			// turn local eU value into component U, Th, and Sm values having same proportion as sample
			U = U_sample * eU / eU_sample;
			TH = TH_sample * eU / eU_sample;
			SM = SM_sample * eU / eU_sample;			
			
			ZRDAAM_Init(HE_PREC_BEST, radius, U, TH, SM); // precision, radius, U, Th, Sm
	//		RDAAM_Init(HE_PREC_BEST, 55., 15., 15., 15.); // precision, radius, U, Th, Sm
	   
			path.clear();
			for (i = ttpairs_in-1; i >= 0; i--)  // RDAAM wants most recent pair first
			{
				mypoint.time = intime[i];
				mypoint.temperature = intemp[i];
				path.push_back(mypoint);   // Present day is first entry in path
			}	
   
			int success = RDAAM_Calculate(&path, age, corrAge);
		}
		eu_save[ieu] = eU;
		raw_age[ieu] = age;
		alpha_age[ieu] = corrAge;
	}
// ***** Wrap up

	ofp = fopen("eugage_OUT.xls","w");
	
	printf("  eU     Raw(Ma) Corr(Ma)\n");
	printf("-------------------------\n");
	for (ieu=0;ieu<EUS;ieu++)
	{	
		printf("%4.0f\t%7.2f\t%7.2f\n",eu_save[ieu],raw_age[ieu],alpha_age[ieu]);
		fprintf(ofp,"%4.0f\t%7.2f\t%7.2f\n",eu_save[ieu],raw_age[ieu],alpha_age[ieu]);
	}
	
	fclose(ofp);

	return 0;
} // end of main()

