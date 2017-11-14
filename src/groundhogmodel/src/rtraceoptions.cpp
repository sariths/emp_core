/*****************************************************************************
	Glare

    Copyright (C) 2017  German Molina (germolinal@gmail.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*****************************************************************************/


#include "./rtraceoptions.h"

RTraceOptions::RTraceOptions()
{
	//n 1							// number of rendering processes
	//x 0        					// flush interval
	//y 0        					// y resolution
	//ld 							// limit distance off
	//h +							// output header
	//faa							// format input/output = ascii/ascii
	//ov        					// output value
	//w +							// warning messages on
	//i 							// irradiance calculation off
	//u +							// uncorrelated Monte Carlo sampling
	//bv +							// back face visibility on

	/* DIRECT */
	addOption("dt", 0.030000);		// direct threshold
	addOption("dc", 0.750000);		// direct certainty
	addOption("dj", 0.000000);		// direct jitter
	addOption("dr", 2);    			// direct relays
	addOption("dp", 512);      		// direct pretest density
	addOption("ds", 0.200000);		// direct sampling
	//dv +							// direct visibility on TODO: Allow booleans

	/* SPECULAR */
	addOption("st", 0.150000);		// specular threshold
	addOption("ss", 1.000000);		// specular sampling
	
	/* LIMIT */
	addOption("lr", 10);      		// limit reflection (Russian roulette)
	addOption("lw", 2.00e003);		// limit weight


	/* AMBIENT */
	//addOption("av", 0.000000);		// ambient value TODO:: Allow three values
	addOption("aw", 0);        		// ambient value weight
	addOption("ab", 0);        		// ambient bounces
	addOption("aa", 0.100000);		// ambient accuracy
	addOption("ar", 256);      		// ambient resolution
	addOption("ad", 1024);     		// ambient divisions
	addOption("as", 512);      		// ambient supersamples
	
	/* MEDIUM */
	//addOption("me", 0.9);			// mist extinction coefficient TODO: Allow three values
	//addOption("ma", 0.000000);		// mist scattering albedo TODO: Allow three values
	addOption("mg", 0.000000);		// mist scattering eccentricity
	addOption("ms", 0.000000);		// mist sampling distance
	
	
	addOption("am", 0.0);			// max photon search radius
};

void RTraceOptions::exposeOptions()
{
	/* DIRECT */
	//shadthresh = getOption("dt");
	//	shadcert = getOption("dc");
	//dstrsrc = getOption("dj");
	//directrelay = (int)getOption("dr");
	//vspretest = (int)getOption("dp");
	//--directvis = getOption("dv");
	//srcsizerat = getOption("ds");

	/* SPECULAR */
	//specthresh = getOption("st");
	//specjitter = getOption("ss");

	/* LIMIT */
	//maxdepth = (int)getOption("lr");
	//minweight = getOption("lw");

	/* AMBIENT */
	// ambval
	//ambvwt = (int)getOption("aw");
	ambacc = getOption("aa");
	//ambres = (int)getOption("ar");
	//ambdiv = (int)getOption("ad");
	//ambssamp = (int)getOption("as");
	ambounce = (int)getOption("ab");

	/* MEDIUM */
	//cextinction
	//salbedo
	//seccg = getOption("mg");
	//ssampdist = getOption("ms");
}