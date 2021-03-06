/* This file contains the main script of KVFinder.
It is responsible for controlling the execution flux of KVFinder software, according to the user definitions.
A brief explanation of the most important parts may be found in the source code */

/* Import builtin modules */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <sys/stat.h>
//#include <sys/wait.h> /* Windows */
#include <omp.h>

/* Import custom modules */
#include "dictionaryprocessing.h"
#include "pdbprocessing.h"
#include "matrixprocessing.h"
#include "argparser.h"
#include "tomlprocessing.h"
#include "resultsprocessing.h"

/* Main function */
int
main (int argc,
      char **argv)
{

	/* Evaluate elapsed time */
	struct timeval toc, tic;
	gettimeofday (&tic, NULL);

	/* Get date and time */
	time_t t = time (NULL);
	struct tm *tm = localtime (&t);

	/* Command to allow nested thread creation */
	omp_set_nested (1);

	/* Setting number of cores */
	int ncores = omp_get_num_procs () - 1;
	omp_set_num_threads (ncores);

	/* Global variables */
	double h, probe_in, probe_out, volume_cutoff, ligand_cutoff, removal_distance, norm1, norm2, norm3, Vvoxel, multiple;
	double X1, Y1, Z1, X2, Y2, Z2, X3, Y3, Z3, X4, Y4, Z4;
	double bX1, bY1, bZ1, bX2, bY2, bZ2, bX3, bY3, bZ3, bX4, bY4, bZ4;
	int ligand_mode, surface_mode, whole_protein_mode, resolution_mode, box_mode, kvp_mode;
	static int verbose_flag = 0;
	int m, n, o, i, j, k, ncav, tablesize, iterator;
	char TABLE[TABLE_SIZE][RES_SIZE], PDB_NAME[NAME_MAX], LIGAND_NAME[NAME_MAX], dictionary_name[DIC_NAME_MAX], OUTPUT[NAME_MAX], BASE_NAME[NAME_MAX];
	char boxmode_flag[6], resolution_flag[7], whole_protein_flag[6], mode_flag[6], surface_flag[6], step_flag[6], kvpmode_flag[6];
	char log_buffer[4096], *output, *output_folder, *output_pdb, *output_results, pdb_name[NAME_MAX];
	dict *DIC[TABLE_SIZE];
	FILE *parameters_file, *log_file;
	atom *p;
	int ***A, ***S;

	if (argc == 1) {
		/* Check if parameters.toml exists */
		if (access ("parameters.toml", F_OK)) {

			/* Print header */
			print_header ();
		    /* Print usage */
			print_usage ();
			/* File does not exist */
			exit (-1);

		}
		/* Read parameters TOML files */
		toml *parameters = readTOML (param, "parameters.toml"); /* Read TOML file inside struct TOML */

		/* Save TOML parameters from struct TOML parameters to KVFinder variables */
		X1 = parameters->X1; Y1 = parameters->Y1; Z1 = parameters->Z1;
		X2 = parameters->X2; Y2 = parameters->Y2; Z2 = parameters->Z2;
		X3 = parameters->X3; Y3 = parameters->Y3; Z3 = parameters->Z3;
		X4 = parameters->X4; Y4 = parameters->Y4; Z4 = parameters->Z4;
		bX1 = parameters->bX1; bY1 = parameters->bY1; bZ1 = parameters->bZ1;
		bX2 = parameters->bX2; bY2 = parameters->bY2; bZ2 = parameters->bZ2;
		bX3 = parameters->bX3; bY3 = parameters->bY3; bZ3 = parameters->bZ3;
		bX4 = parameters->bX4; bY4 = parameters->bY4; bZ4 = parameters->bZ4;
		strcpy (PDB_NAME, parameters->PDB_NAME);
		strcpy (OUTPUT, parameters->OUTPUT);
		strcpy (BASE_NAME, parameters->BASE_NAME);
		strcpy (LIGAND_NAME, parameters->LIGAND_NAME);
		strcpy (dictionary_name, parameters->dictionary_name);
		strcpy (resolution_flag, parameters->resolution_flag);
		volume_cutoff = parameters->volume_cutoff;
		ligand_cutoff = parameters->ligand_cutoff;
		removal_distance = parameters->removal_distance;
		probe_in = parameters->probe_in;
		probe_out = parameters->probe_out;
		h = parameters->h;
		whole_protein_mode = parameters->whole_protein_mode;
		box_mode = parameters->box_mode;
		surface_mode = parameters->surface_mode;
		kvp_mode = parameters->kvp_mode;
		ligand_mode = parameters->ligand_mode;

    /* Free struct TOML */
		free (param);
		free (parameters);
	}
	else {
		/* Save command line arguments inside KVFinder variables */
		verbose_flag = argparser (argc,
		                          argv,
		                          &box_mode,
		                          &kvp_mode,
		                          &ligand_mode,
		                          &surface_mode,
		                          &whole_protein_mode,
		                          PDB_NAME,
		                          LIGAND_NAME,
		                          dictionary_name,
		                          OUTPUT,
		                          BASE_NAME,
		                          resolution_flag,
		                          &h,
		                          &probe_in,
		                          &probe_out,
		                          &volume_cutoff,
		                          &ligand_cutoff,
		                          &removal_distance,
		                          &X1, &Y1, &Z1, &X2, &Y2, &Z2, &X3, &Y3, &Z3, &X4, &Y4, &Z4,
		                          &bX1, &bY1, &bZ1, &bX2, &bY2, &bZ2, &bX3, &bY3, &bZ3, &bX4, &bY4, &bZ4);
	}
	resolution_input (resolution_flag, &Vvoxel, &resolution_mode, &h); /*Set Vvoxel, step size and resolution_mode*/
	tablesize = define_table (TABLE, dictionary_name); /*Read dictionary file and return number of residues*/
	if (verbose_flag)
	    fprintf (stdout, "> Loading atomic dictionary file\n");
	dictionary_load (DIC, tablesize, dictionary_name); /*Dictionary Loaded*/

	/* Preparing files paths */
	/* Windows */
  output = combine(OUTPUT, "\\KV_Files\\");
	_fullpath(pdb_name, PDB_NAME, 500);

	/* Create KV_Files folder */
	mkdir (output); /* Windows */

	/* Create log_file */
	log_file = fopen (combine (output, "KVFinder.log"), "a+"); /* Open log file and append information */
	memset (log_buffer, '\0', sizeof (log_buffer)); /* Create buffer */
	setvbuf (log_file, log_buffer, _IOFBF, 4096); /* Define buffer as writing buffer of size 4096 */

	/* Create BASE_NAME folder for the running analysis */
	output = combine (output, BASE_NAME); /* Include BASE_NAME folder in KV_Files */
	mkdir (output); /* Windows */
	output_folder = combine (output, "\\"); /* Include bar after appending BASE_NAME */ /* Windows */
	output = combine (output_folder, BASE_NAME); /* Include BASE_NAME to output path in BASE_NAME folder */

	/* Create output PDB and results file names */
	output_results = combine (output, ".KVFinder.results.toml"); /* Create a output path to results file */
	output_pdb = combine (output, ".KVFinder.output.pdb"); /* Create a output path to output PDB file */

    /* Print in shell the PDB path that is running in KVFinder */
	if (!verbose_flag)
	    fprintf (stdout, "[PID %u] Running parKVFinder for: %s\n", getpid (), pdb_name);

	fprintf (log_file, "==========\tSTART\tRUN\t=========\n\n"); /* Print the start tag for the run inside log */
	fprintf (log_file, "Date and time: %s\n", asctime (tm));
	fprintf (log_file, "Running parKVFinder for: %s\n", pdb_name); /* Print path to PDB file */
	fprintf (log_file, "Dictionary: %s\n", dictionary_name); /* Print path to dictionary */
	/* if Resolution is Off, print Resolution and Step size */
	if (strcmp (resolution_flag, "Off") == 0)
	    fprintf(log_file, "Resolution: %s\nInput step size: %.2lf\n", resolution_flag, h);
	/* Else, print resolution */
	else
	    fprintf (log_file, "Resolution: %s\n", resolution_flag);

	if (verbose_flag)
	    fprintf (stdout, "> Calculating grid dimensions\n");

	if (whole_protein_mode) {

		X1 = 999999; Y1 = 999999; Z1 = 999999;
		X2 = -999999; Y3 = -999999; Z4 = -999999;

        /* Create a linked list (dictionary) for PDB file | saves only position (x,y,z) and chain */
		PDB_load2 (PDB_NAME);

		/*Reduces box to protein size*/
		for (p = v; p != NULL; p = p->next) {

			if (p->x < X1)
			    X1 = (p->x);
			if (p->y < Y1)
			    Y1 = (p->y);
			if (p->z < Z1)
			    Z1 = (p->z);
			if (p->x > X2)
			    X2 = (p->x);
			if (p->y > Y3)
			    Y3 = (p->y);
			if (p->z > Z4)
			    Z4 = (p->z);

		}

	    /* Free van der Waals radius dictionary from memory */
		free_atom ();

	}	 /* If it will be used a user defined search space, without the Probe Out Adjustment define its limits */
	 else
	    if (box_mode) {};

	/* Resizing step: increases the grid step size until the number of points on the grid be smaller than Vvoxel */
	if (resolution_mode) {
		/* Defines the grid inside the search space | Point 1 is the reference of our grid */
		/* Calculate distances between points in box and reference */
		norm1 = sqrt ((X2-X1)*(X2-X1) + (Y2-Y1)*(Y2-Y1) + (Z2-Z1)*(Z2-Z1));
		norm2 = sqrt ((X3-X1)*(X3-X1) + (Y3-Y1)*(Y3-Y1) + (Z3-Z1)*(Z3-Z1));
		norm3 = sqrt ((X4-X1)*(X4-X1) + (Y4-Y1)*(Y4-Y1) + (Z4-Z1)*(Z4-Z1));

		/*Grid steps based on distances*/
		m = (int)(norm1/h); /* X axis */
		n = (int)(norm2/h); /* Y axis */
		o = (int)(norm3/h); /* Z axis */

        /* While volume of unitary box is less than Vlim, increment h and recalculate box sizes */
		while ((norm1/m)*(norm2/n)*(norm3/o) < Vvoxel) {
			h += 0.05;
			m = (int)(norm1/h);
			n = (int)(norm2/h);
			o = (int)(norm3/h);
			fprintf (log_file, "Step size too small! Resizing... New step size = %.2lf\n", h);
		}
		fprintf (log_file, "Chosen step size = %.2lf\n", h);
	}
	/* Convert step size to string */
	sprintf (step_flag, "%.2lf", h);

	if (whole_protein_mode) {

		/*Transforms coordinates to step size multiples*/
		X1 = roundf ((X1 + fabs((double)((int)(X1 * 1000) % (int)(h * 1000)) / 1000) - h) * 1000) / 1000;
		Y1 = roundf ((Y1 + fabs((double)((int)(Y1 * 1000) % (int)(h * 1000)) / 1000) - h) * 1000) / 1000;
		Z1 = roundf ((Z1 + fabs((double)((int)(Z1 * 1000) % (int)(h * 1000)) / 1000) - h) * 1000) / 1000;
		X2 = roundf ((X2 - fabs((double)((int)(X2 * 1000) % (int)(h * 1000)) / 1000) + h) * 1000) / 1000;
		Y3 = roundf ((Y3 - fabs((double)((int)(Y3 * 1000) % (int)(h * 1000)) / 1000) + h) * 1000) / 1000;
		Z4 = roundf ((Z4 - fabs((double)((int)(Z4 * 1000) % (int)(h * 1000)) / 1000) + h) * 1000) / 1000;

		/* Increase box for probe_out in step size multiple value */
		multiple = roundf ((probe_out-((double)((int)(probe_out*1000)%(int)(h*1000))/1000))*1000)/1000;
		X1 -= (multiple); Y1 -= (multiple); Z1 -= (multiple);
		X2 += (multiple);
		Y3 += (multiple);
		Z4 += (multiple);
		X3 = X1; X4 = X1;
		Y2 = Y1; Y4 = Y1;
		Z2 = Z1; Z3 = Z1;

	}

	/* Defines the grid inside the search space | Point 1 is the reference of our grid */
	/* Calculate distances between points in box and reference */
	norm1 = sqrt ((X2 - X1) * (X2 - X1) + (Y2 - Y1) * (Y2 - Y1) + (Z2 - Z1) * (Z2 - Z1));
	norm2 = sqrt ((X3 - X1) * (X3 - X1) + (Y3 - Y1) * (Y3 - Y1) + (Z3 - Z1) * (Z3 - Z1));
	norm3 = sqrt ((X4 - X1) * (X4 - X1) + (Y4 - Y1) * (Y4 - Y1) + (Z4 - Z1) * (Z4 - Z1));

	/* Grid steps based on distances */
	m = (int)(norm1 / h)+1; /*X axis*/
	n = (int)(norm2 / h)+1; /*Y axis*/
	o = (int)(norm3 / h)+1; /*Z axis*/

	/* Calculate data used on the spatial manipulation of the protein */
	sina = (Y4 - Y1) / norm3;
	cosa = (Y3 - Y1) / norm2;
	cosb = (X2 - X1) / norm1;
	sinb = (Z2 - Z1) / norm1;

    /* Calculate Voxel volume */
	Vvoxel = h*h*h;

    /* Write voxel volume inside log file */
	fprintf (log_file, "Unitary box volume: %.4lf Angstrons^3\n", Vvoxel);
	/* Write m, n and o axis inside log file */
	fprintf (log_file, "m: %d\tn: %d\to: %d\n", m, n, o);
	/* Write sina, sinb, cosa and cosb inside log file */
	fprintf (log_file, "sina: %.2lf\tsinb: %.2lf\t\ncosa: %.2lf\tcosb: %.2lf\n", sina, sinb, cosa, cosb);

    if(verbose_flag)
        fprintf(stdout, "> Reading PDB coordinates\n");

    /* Protein Coordinates Extraction */
    /* Create a linked list for PDB information */
    /* Save coordinates (x,y,z), atom radius, residue number and chain */
    if (PDB_load (DIC, tablesize, TABLE, PDB_NAME, probe_in, m, n, o, h, X1, Y1, Z1, &log_file)) {

        if (verbose_flag)
            fprintf(stdout, "> Creating grid\n");

        /* Matrix Allocation and Initialization */
        /* int ***A: Grid representing empty spaces and surface points along marked by small probe
        int ***S: Grid representing empty spaces and surface points along marked by big probe
        double ***M: Grid representing depth in each cavity point */
        A = (int ***) calloc (m, sizeof (int**));
        S = (int ***) calloc (m, sizeof (int**));
        for (i = 0; i < m; i++) {
            A[i] = (int **) calloc (n, sizeof (int *));
            S[i] = (int **) calloc (n, sizeof (int *));
            for (j = 0; j < n; j++) {
                A[i][j] = (int *) calloc (o, sizeof (int));
                S[i][j] = (int *) calloc (o, sizeof (int));
                for (k = 0; k < o; k++) {
                    A[i][j][k] = 1;
                    S[i][j][k] = 1;
                }
            }
        }

        if (verbose_flag)
            fprintf (stdout, "> Filling grid with probe in surface\n");
        /* Mark the grid with 0, leaving a small probe size around the protein */
        Matrix_fill (A, m, n, o, h, probe_in, X1, Y1, Z1);

        /* Mark space occupied by a small probe size from protein surface */
        if (surface_mode)
            Matrix_surf (A, m, n, o, h, probe_in);

        if (verbose_flag)
            fprintf (stdout, "> Filling grid with probe out surface\n");
        /* Mark the grid with 0, leaving a big probe size around the protein */
        Matrix_fill (S, m, n, o, h, probe_out, X1, Y1, Z1);
        /* Mark space occupied by a big probe size from protein surface */
        Matrix_surf (S, m, n, o, h, probe_out);

        if(verbose_flag)
            fprintf (stdout, "> Defining biomolecular cavities\n");
        /* Mark points where small probe passed and big probe did not */
        Matrix_subtract (S, A, m, n, o, h, removal_distance);

        /* Ligand adjustment mode */
        if (ligand_mode) {
            if (verbose_flag)
                fprintf (stdout, "> Adjusting ligand\n");
            /* Free linked list (dictionary) from memory */
            free_atom ();
            /* Creates a linked list for Ligand information* | saves position (x,y,z), atom radius, resnumber, chain */
            PDB_load (DIC, tablesize, TABLE, LIGAND_NAME, probe_in, m, n, o, h, X1, Y1, Z1, &log_file);
            /* Mark regions that do not belong to ligand_cutoff */
            Matrix_adjust (A, m, n, o, h, ligand_cutoff, X1, Y1, Z1);
            /* Free linked list (dictionary) from memory */
            free_atom ();
            /* Create a linked list for PDB information* | saves position (x,y,z), atom radius, resnumber, chain */
            PDB_load (DIC, tablesize, TABLE, PDB_NAME, probe_in, m, n, o, h, X1, Y1, Z1, &log_file);
        }

        /* Box adjustment mode is ON */
        if (box_mode) {
            if (verbose_flag)
                fprintf (stdout, "> Filtering grid points\n");
            /* The points outside the user defined search space are excluded here */
            Matrix_filter (A, S, m, n, o, h, bX1, bY1, bZ1, bX2, bY2, bZ2, norm1);
        }

        /* Computing Volume and Grouping Cavities */
        if (verbose_flag)
            fprintf (stdout, "> Calculating volume\n");

        /*Remove outlier points*/
        filter_outliers (A, m, n, o);

        /* Return tag (number of cavities) with volume lower than volume_cutoff and, also, creates a linked list
        containing volume of each tag. tag starts at integer 2 */
        ncav = DFS_search (A, m, n, o, h, volume_cutoff)-1;

        /* If KVFinder have not found cavities, go to end of script and finish execution */
        if (ncav == 0) {
            fprintf(stdout, "> parKVFinder found no cavities!\n");
            goto NOCAV;
        }

        /* Create KVFinder_results structure */
        /* Allocate memory for KVresults structure */
        KVFinder_results = (KVresults*) calloc (ncav, sizeof (KVresults));

        /* Save volume data inside linked list (node) in KVFinder_results structure */
        node *p;
        for (p = V; p != NULL; p = p->next)
            KVFinder_results[(p->pos) - 2].volume = p->volume;
        /* Free linked list for cavities volume from memory */
        free_node ();
        free (p);
        free (V);

        /* Define surface points of each cavity */
        if (verbose_flag)
            fprintf (stdout, "> Calculating surface points\n");
        /* Mark surface points inside S, and remove unnecessary points inside S */
        Matrix_surface (A, S, m, n, o, h, X1, Y1, Z1);

        /* Computing Surface Area */
        if (verbose_flag)
            fprintf (stdout, "> Calculating area\n");
        /* Calculate surface area of each cavity and return number of cavities */
        Area_search (S, m, n, o, h, ncav);

        if (verbose_flag)
            fprintf(stdout, "> Retrieving residues surrounding cavities\n");
        /* Define interface residues for each cavity */
        Matrix_search (A, S, m, n, o, h, probe_in, X1, Y1, Z1, ncav);

        /* Free PDB linked list (dictionary) from memory */
        NOCAV: free_atom ();

        /* Turn ON(1) filled cavities option */
        if (verbose_flag)
            fprintf (stdout, "> Writing cavities PDB file\n");
        /* Export Cavities PDB */
        Matrix_export (A, S, kvp_mode, m, n, o, h, ncav, output, output_pdb, X1, Y1, Z1);

    }

	/* Clean 3D-grids from memory */
	free_matrix (A, m, n, o); /*Free int A grid from memory*/
	free_matrix (S, m, n, o); /*Free int S grid from memory*/

	/* Write results file */
	if (verbose_flag)
	    fprintf (stdout, "> Writing results file\n");
	write_results (output_results, PDB_NAME, output_pdb, LIGAND_NAME, resolution_flag, step_flag, ncav); /* Windows */

	/*Evaluate elapsed time*/
    gettimeofday (&toc, NULL);
	printf ("done!\n");
	printf ("Elapsed time: %.2lf seconds\n", (double)(toc.tv_sec - tic.tv_sec)+((toc.tv_usec-tic.tv_usec)/1000000.0)); /*Print the elapsed time*/
	fprintf (log_file, "Elapsed time: %.2lf seconds\n", (double)(toc.tv_sec - tic.tv_sec)+((toc.tv_usec-tic.tv_usec)/1000000.0)); /*Print the elapsed time for the run inside log*/
	fprintf (log_file, "\n==========\tEND\tRUN\t=========\n\n"); /*Print the end tag for the run inside log*/

	/*Write log.txt*/
	fflush (log_file);
	fclose (log_file);

	return 0;

}
