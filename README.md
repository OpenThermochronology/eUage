# eUage
	Program euage 1.0
	Written by Dr. Peter Zeitler (Lehigh University) in 2014
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
