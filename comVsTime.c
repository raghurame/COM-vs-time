#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>

/*
 * ARGUMENTS
 * ~~~~~~~~~
 *
 * argv[0] = program
 * argv[1] = input filename
 * argv[2] = output filename
 * argv[3] = number of timeframes to scan
 *
 */

typedef struct com
{
	float x, y, z;
} COM;

int nAtoms_atomType (FILE *input, int atomType)
{
	rewind (input);

	int nAtoms_total, nAtoms_selectedType = 0;
	char lineString[1000];

	int d1, d2;
	float d3, d4, d5;

	for (int i = 0; i < 9; ++i)
	{
		fgets (lineString, 1000, input);

		if (i == 3)
			sscanf (lineString, "%d\n", &nAtoms_total);
	}

	for (int i = 0; i < nAtoms_total; ++i)
	{
		fgets (lineString, 1000, input);
		sscanf (lineString, "%d %d %f %f %f\n", &d1, &d2, &d3, &d4, &d5);

		if (d2 == atomType)
			nAtoms_selectedType++;
	}

	return nAtoms_selectedType;
}

void findCOM (FILE *input, FILE *output, int nTimeframes_toCheck)
{
	rewind (input);
	
	char lineString[1000];
	int nAtoms_total;

	/* Dump variables */
	int d1, d2;
	float d3, d4, d5;

	/* COM variables */
	int nAtoms_1 = 0, nAtoms_2 = 0, nAtoms_3 = 0;

	float distance, distance_XY, distance_XZ, distance_YZ;

	COM com_chain, com_tube;

	com_chain.x = 0; com_chain.y = 0; com_chain.z = 0;
	com_tube.x = 0; com_tube.y = 0; com_tube.z = 0;

	for (int i = 0; i < 9; ++i)
	{
		fgets (lineString, 1000, input);

		if (i == 3)
			sscanf (lineString, "%d\n", &nAtoms_total);
	}

	rewind (input);

	for (int i = 0; i < nTimeframes_toCheck; ++i)
	{
		/* re-initializing variables */
		nAtoms_1 = 0; nAtoms_2 = 0;

		com_chain.x = 0; com_chain.y = 0; com_chain.z = 0; com_tube.x = 0; com_tube.y = 0; com_tube.z = 0;

		for (int j = 0; j < 9; ++j)
			fgets (lineString, 1000, input);

		for (int j = 0; j < nAtoms_total; ++j)
		{
			fgets (lineString, 1000, input);
			sscanf (lineString, "%d %d %f %f %f\n", &d1, &d2, &d3, &d4, &d5);

			if (d2 == 1)
			{
				nAtoms_1++;
				com_chain.x += d3;
				com_chain.y += d4;
				com_chain.z += d5;
			}
			else if (d2 == 2)
			{
				nAtoms_2++;
				com_tube.x += d3;
				com_tube.y += d4;
				com_tube.z += d5;
			}
		}

		com_chain.x /= nAtoms_1;
		com_chain.y /= nAtoms_1;
		com_chain.z /= nAtoms_1;

		com_tube.x /= nAtoms_2;
		com_tube.y /= nAtoms_2;
		com_tube.z /= nAtoms_2;

		/* Distance between two COMs - XYZ */
		distance = sqrt (pow ((com_chain.x - com_tube.x), 2) + pow ((com_chain.y - com_tube.y), 2) + pow ((com_chain.z - com_tube.z), 2));

		/* Distance between two COMs - XY */
		distance_XY = sqrt (pow ((com_chain.x - com_tube.x), 2) + pow ((com_chain.y - com_tube.y), 2));

		/* Distance between two COMs - XZ */
		distance_XZ = sqrt (pow ((com_chain.x - com_tube.x), 2) + pow ((com_chain.z - com_tube.z), 2));

		/* Distance between two COMs - YZ */
		distance_YZ = sqrt (pow ((com_chain.y - com_tube.y), 2) + pow ((com_chain.z - com_tube.z), 2));

		/* Priting the results */
		fprintf(output, "%d %.3f %.3f %.3f %.3f %.3f %.3f %.3f %.3f %.3f %.3f \n", i, com_chain.x, com_chain.y, com_chain.z, com_tube.x, com_tube.y, com_tube.z, distance, distance_XY, distance_XZ, distance_YZ);
	}

}

int main(int argc, char const *argv[])
{
	FILE *input, *output;
	input = fopen (argv[1], "r");
	output = fopen (argv[2], "w");

	// int nAtoms_1 = nAtoms_atomType (input, 1), nAtoms_2 = nAtoms_atomType (input, 2), nAtoms_3 = nAtoms_atomType (input, 3);

	// printf("nAtoms_1: %d\n", nAtoms_1);
	// printf("nAtoms_2: %d\n", nAtoms_2);
	// printf("nAtoms_3: %d\n", nAtoms_3);

	int nTimeframes_toCheck = atoi (argv[3]);

	findCOM (input, output, nTimeframes_toCheck);

	return 0;
}