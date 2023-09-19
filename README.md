# eUage
	Program euage 1.0
	Written by Dr. Peter Zeitler (Lehigh University) in 2014
	C++ to be more easily compatible with Rich Ketcham's RDAAM code.
	
	This program was kludged together by grabbing the wrapper from my crankfunc code,
	just using that to call RDAAM/ZRDAAM. 
 
 	The Radiation Damage Accumulation & Annealing Model = RDAAM is discussed in [Flowers et al. (2009)] (https://doi.org/10.1016/j.gca.2009.01.015)
  	ZRDAAM is discussed in [Guenthner et al. (2013)](https://doi.org/10.2475/03.2013.01)
	
	Written only for apatite and zircon.
 	The wrapper pulls in one input file with required sample data, and another with the
	desired thermal history.
	
	It runs in a loop across a range of eU values, having the same U/Th/Sm proportions as 
	the sample.
	
	Zoning can't be modeled.
	
	usage:
	
	./euage <#of eU values>. <step between eU values>
