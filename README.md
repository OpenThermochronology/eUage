# eUage

Program eUage 1.0

Written in 2014 by Dr. Peter Zeitler (Lehigh University) in C++ to be more easily compatible with Rich Ketcham's (UT Austin) RDAAM code.
	
This program was kludged together by grabbing the wrapper from the crankfunc code (i.e., Crank-Nicholson diffusion) and just using that to call RDAAM/ZRDAAM. 

The Radiation Damage Accumulation & Annealing Model (RDAAM) in the mineral apatite is published in [Flowers et al. (2009)](https://doi.org/10.1016/j.gca.2009.01.015). 
The ZRDAAM is the radiation damage model for zircon published in [Guenthner et al. (2013)](https://doi.org/10.2475/03.2013.01).

The wrapper pulls in one input file with required sample data, and another with the desired thermal history. Written only for apatite and zircon. It runs in a loop across a range of effective uranium (eU=U+0.238*Th) values, having the same U/Th/Sm proportions as the sample and provides output of the predicted date-eU relationship across the range of eU from the damage models. Minerals are modeled using spherical geometry and output 'raw' uncorrected ages and alpha-ejection corrected ages. Zoning can't be modeled.

## Input Files

### euage.in <br>
0 = apatite or 1 = zircon <br>
U ppm <br>
Th ppm <br>
Sm ppm <br>
grain radius in microns <br>

### euage_history.in <br>
number of tT pairs, i.e., 2 <br>
tT points in descending order <br>

i.e., <br>
2 <br>
500 150 <br>
0 0 <br>

## Usage

``` 
./euage <#of eU values>. <step between eU values>
```
