#include "climateFile.h"
#include <stdio.h>
#include <iostream>
#include "../solar.h"

// *** added by J.KAEMPF for compiling with MinGW *** //
//#include <cstring>
// ************************************************** //

#include "paths.h"

cClimateFile::cClimateFile(void)
{
	// set pointers to point to zero initially
	m_ptIgh=0;
	m_ptIdh=0;
}

cClimateFile::~cClimateFile(void)
{
	delete[] m_ptIgh;
	delete[] m_ptIdh;
}

bool cClimateFile::ReadClimateFile(char *FileName, int HourConvention, eClimateFileFormat ClimateFileFormat)
{
	FILE *InputFile;
	char *Line = new char[100];

	int hour,day;
	int i;

	// open file (checking that you can...)
	if ((InputFile=LoadFile(FileName))==NULL)
	{
		fprintf(stderr,"Error opening: %s\n",FileName);
		return false;
	}
	rewind(InputFile);
	// TODO: Add error checking

	// get rid of old climate data
	delete[] m_ptIgh;
	delete[] m_ptIdh;

	//Assume we've got 8760 points
	m_NumPoints=8760;

	double *Col1 = new double[m_NumPoints];
	double *Col2 = new double[m_NumPoints];

	// read in points for each hour of each day
	// points are assumed to be at 0:30, 1:30, ... from 1st Jan
	for (day=0; day<365; day++)
	{
		for (hour=0; hour<24; hour++)
		{
			if (fgets(Line,100,InputFile)==NULL)
			{
				// ran out of file
				fprintf(stderr,"Error processing climate file %s\n",FileName);
				return false;
			}

			sscanf(Line,"%lf %lf\n",&Col1[day*24+hour],&Col2[day*24+hour]);
		}
	}

	// Now convert data into appropriate form
	if (ClimateFileFormat==GLOBAL_DIFFUSE)
	{
		m_ptIgh=Col1;
		m_ptIdh=Col2;
	}
	else if (ClimateFileFormat == DIRECTHORIZONTAL_DIFFUSE)
	{
		m_ptIdh=Col2;
		for (i=0; i<m_NumPoints; i++)
		{
			Col1[i]=Col1[i]+Col2[i];
		}
		m_ptIgh=Col1;
	}
	else
	{
		// ran out of file
		fprintf(stderr,"Unknown climate file format!\n");
		return false;
	}


	if (!ValidateData())
	{
		fprintf(stderr,"Invalid Data!\n");
		return false;
	}
	fclose(InputFile);
	return true;
}

void cClimateFile::loadModelWeather(EmpModel * model, cClimateFile::eClimateFileFormat ClimateFileFormat)
{
    
    //int hour,day;
    //int i;
    
    
    // get rid of old climate data
    delete[] m_ptIgh;
    delete[] m_ptIdh;
    
    //Get data size
    Location * location = model->getLocation();
    double s_latitude = location->getLatitude()*PI/180.0;
    double s_longitude = -location->getLongitude()*PI/180.0;
    double s_meridian = -location->getTimeZone() * (-15*PI/180.0);
    
    const size_t m_NumPoints = location->getWeatherSize();
    
    if(m_NumPoints != 8760)
        throw "Your model requires a full hourly 365-day weather to calculate cumulative sky";
    
    m_ptIgh = new double[m_NumPoints];
    m_ptIdh = new double[m_NumPoints];

    // Translate the file
    for(size_t i=0; i<m_NumPoints; i++){
        const HourlyData * now = location->getHourlyData(i);
        float diffuse_horizontal = now->diffuse_horizontal;
        float direct_normal = now->direct_normal;
        
        // Calculate solar altitude
        int jd = jdate(now->month,now->day);
        double sd = sdec(jd);
        double st = now->hour + stadj(jd, -s_longitude, -s_meridian);
        double solar_altitude = salt(sd,st,s_latitude);
        
        
        m_ptIgh[i] = (direct_normal * sin(solar_altitude) + diffuse_horizontal); // direct_horizontal + diffuse_horizontal = global_horizontal
        m_ptIdh[i] = diffuse_horizontal;
        
        // This is for generating the input for the standalone gencumulativesky
        std::cerr << m_ptIgh[i] << " " << m_ptIdh[i] << std::endl;
        
        
    }
    
    
    
    
}




bool cClimateFile::ValidateData()
{
	// TODO: Write this function!
	return true;
}

double cClimateFile::GetDirectRad(double hour, int day)
{
	// interpolate linearly between two closest hours
	double hourdiff, hour1;
	int pointer;

	// find the half hour before current time (climate file has data for 0:30, 1:30,2:30, etc...)
	hour1=(int)hour;
	hourdiff=hour-hour1;
	if (hourdiff >= 0.5) hour1+= 0.5;
	else hour1-=0.5;

	pointer=(day-1)*24+(int)(hour1-0.5);

	// if pointer < 0, don't have data for specified day
	if (pointer < 0) return -9999;

	float ratio, diff;

	ratio=(float)(hour-hour1);
	// TODO: CHeck this (first/last hours of day)
	diff= (float)((m_ptIgh[pointer+1]-m_ptIdh[pointer+1])-(m_ptIgh[pointer]-m_ptIdh[pointer]));

	return (m_ptIgh[pointer]-m_ptIdh[pointer]) + diff*ratio;
}

double cClimateFile::GetDiffuseRad(double hour, int day)
{
	// interpolate linearly between two closest hours
	float hour1, hourdiff;
	int pointer;

	// find the half hour before current time (climate file has data for 0:30, 1:30,2:30, etc...)
	hour1= (float)(hour);
	hourdiff= (float)(hour-hour1);
	if (hourdiff >= 0.5) hour1+= 0.5;
	else hour1-=0.5;

	pointer=(day-1)*24+(int)(hour1-0.5);

	// if pointer < 0, don't have data for specified day
	if (pointer < 0) return -9999;

	float ratio, diff;

	ratio= (float)(hour-hour1);
	diff= (float)(m_ptIdh[pointer+1]-m_ptIdh[pointer]);

	return m_ptIdh[pointer] + diff*ratio;
}

double cClimateFile::GetGlobalRad(double hour, int day)
{
	// interpolate linearly between two closest hours
	float hour1, hourdiff;
	int pointer;

	// find the half hour before current time (climate file has data for 0:30, 1:30,2:30, etc...)
	hour1= (float)(hour);
	hourdiff= (float)(hour-hour1);
	if (hourdiff >= 0.5) hour1+= 0.5;
	else hour1-=0.5;

	pointer=(day-1)*24+(int)(hour1-0.5);

	// if pointer < 0, don't have data for specified day
	if (pointer < 0) return -9999;

	float ratio, diff;

	ratio= (float)(hour-hour1);
	diff= (float)(m_ptIgh[pointer+1]-m_ptIgh[pointer]);
	return m_ptIgh[pointer] + diff*ratio;
}


// THIS ROUTINE 'BORROWED' FROM GENDAYLIT
FILE* cClimateFile::LoadFile( char *fname)			/* find file and open for reading */
{
	FILE  *fp;
	char  pname[MAXPATH];
	char *libpath=NULL;
    char  *sp, *cp;

	if (fname == NULL)
		return(NULL);

	if (ISDIRSEP(fname[0]) || fname[0] == '.')	/* absolute path */
		return(fopen(fname, "r"));

	if (libpath == NULL) {			/* get search path */
		libpath = getenv(ULIBVAR);
        // *** modified by J.KAEMPF to remove depreciated warning *** //
		if (libpath == NULL)
            strcpy(libpath, DEFPATH);
			//libpath = DEFPATH;
        // ********************************************************** //
	}
						/* check search path */
	sp = libpath;
	do {
		cp = pname;
		while (*sp && (*cp = *sp++) != PATHSEP)
			cp++;
		if (cp > pname && !ISDIRSEP(cp[-1]))
			*cp++ = DIRSEP;
		strcpy(cp, fname);
		if ((fp = fopen(pname, "r")) != NULL)
			return(fp);			/* got it! */
	} while (*sp);
						/* not found */
	return(NULL);
}
