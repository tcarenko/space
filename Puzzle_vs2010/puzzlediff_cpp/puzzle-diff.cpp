extern "C" {
  #include "puzzle_common.h"
  #include "puzzle.h"
}
#include "pgetopt.hpp"
#include <vector>
#include <iostream>
#include <fstream>
#include <Windows.h>

using namespace std;

typedef struct NameDist
{
	const char* fileName;
 	double dist;
};

typedef struct Opts_ {
	Opts_() : writeInFile(false)
	{
	}
    const char *file1;    
    const char *file2;
	const char *directory;
	bool writeInFile;
	const char *fileToSave;
    int fix_for_texts;
    int exit;
    double similarity_threshold;
} Opts;

extern void listDir(const char* dirName, std::vector<std::string>& fnVec);
int partition(NameDist* a,int p, int r);
void qsort(NameDist *a,int p, int q);

void usage(void)
{
    puts("\nUsage: puzzle-diff [-b <contrast barrier for cropping] [-c]\n"
         "   [-C <max cropping ratio>] [-e] [-E similarity threshold] [-h]\n"
         "   [-H <max height>] [-l <lambdas>] [-n <noise cutoff>]\n"
		 "   [-p <p ratio>] [-t] [-W <max width>] [-o <file name>] <file 1> <directory>\n\n"
         //"Visually compares two images and returns their distance.\n\n"
         "-b <contrast barrier for cropping>\n"
         "-c : disable autocrop\n"
         "-C <max cropping ratio>\n"
         "-e : exit with 10 (images are similar) or 20 (images are not)\n"
         "-E <similarity threshold> : for -e\n"
         "-h : show help\n"
         "-H <width> : set max height\n"
         "-l <lambdas> : change lambdas\n"
         "-n <noise cutoff> : change noise cutoff\n"
         "-p <ratio> : set p ratio\n"
         "-t disable fix for texts\n"
         "-W <width> : set max width\n"
		 "-o <filename> : save results in a file"
         "\n");
    exit(EXIT_SUCCESS);
}

int parse_opts(Opts * const opts, PuzzleContext * context,
               int argc, char **argv) {
    int opt;
    extern char *poptarg;
    extern int poptind;

    opts->fix_for_texts = 1;
    opts->exit = 0;
    opts->similarity_threshold = PUZZLE_CVEC_SIMILARITY_THRESHOLD;
    while ((opt = pgetopt(argc, argv, "b:cC:eE:hH:l:n:p:tW:o:")) != -1) {
        switch (opt) {
        case 'b':
            puzzle_set_contrast_barrier_for_cropping(context, atof(poptarg));
            break;
        case 'c':
            puzzle_set_autocrop(context, 0);
            break;            
        case 'C':
            puzzle_set_max_cropping_ratio(context, atof(poptarg));
            break;
        case 'e':
            opts->exit = 1;
            break;
        case 'E':
            opts->similarity_threshold = atof(poptarg);
            break;
        case 'h':
            usage();
            /* NOTREACHED */
        case 'H':
            puzzle_set_max_height(context, strtoul(poptarg, NULL, 10));
            break;
        case 'l':
            puzzle_set_lambdas(context, strtoul(poptarg, NULL, 10));
            break;
        case 'n':
            puzzle_set_noise_cutoff(context, atof(poptarg));
            break;
        case 'p':
            puzzle_set_p_ratio(context, atof(poptarg));
            break;
        case 't':
            opts->fix_for_texts = 0;
            break;
        case 'W':
            puzzle_set_max_width(context, strtoul(poptarg, NULL, 10));
            break;
		case 'o':
			{
				opts -> writeInFile = true;
				opts -> fileToSave = poptarg;
				break;
			}
        default:
            usage();
            /* NOTREACHED */            
        }
    }
    argc -= poptind;
    argv += poptind;
    if (argc != 2) {
        usage();
    }
    opts->file1 = *argv++;
    opts->directory = *argv;
    
    return 0;
}

int main(int argc, char *argv[])
{
	Opts opts;
	PuzzleContext context;
	puzzle_init_context(&context); 
	//parsing input parameters
	parse_opts(&opts, &context, argc, argv);
	//vector of files names
	vector<string> filesNameList;
	//directory
	const char* directory = opts.directory;
	//get the list of file names
	listDir(directory, filesNameList);
	//array of results
	NameDist* nameDistArr = new NameDist[filesNameList.size()];
	//main loop
	cilk_for(int i = 0; i < filesNameList.size(); i++)
	{
		//what time is it?
		#ifdef DEBUGMODE
		SYSTEMTIME sysT;
		GetSystemTime(&sysT);
		//display time
		cout <<"iteration : " << i <<"->start_time : " << sysT.wSecond <<" s , " << sysT.wMilliseconds << " ms" << endl;
		#endif
		Opts opts_local = opts;
		PuzzleCvec cvec1; 
		PuzzleCvec cvec2; 
		double d;
		opts_local.file2 = filesNameList[i].c_str();

		puzzle_init_cvec(&context, &cvec1);
		puzzle_init_cvec(&context, &cvec2);
		if (puzzle_fill_cvec_from_file(&context, &cvec1, opts_local.file1) != 0) { 
			fprintf(stderr, "Unable to read [%s]\n", opts_local.file1);
		}

		if (puzzle_fill_cvec_from_file(&context, &cvec2, opts_local.file2) != 0) {
			fprintf(stderr, "Unable to read [%s]\n", opts_local.file2);
		}

		d = puzzle_vector_normalized_distance(&context, &cvec1, &cvec2,
		opts_local.fix_for_texts);
		puzzle_free_cvec(&context, &cvec1);
		puzzle_free_cvec(&context, &cvec2);
		puzzle_free_context(&context);
		if (opts_local.exit == 0) {
			#ifdef DEBUGMODE
			//printf("%g\n", d);
			cout << "iteration : " << i <<"->result : " << d << endl;
			#endif
			//add to result array
			nameDistArr[i].dist = d;
			nameDistArr[i].fileName = strdup(opts_local.file2); 
		}
		if (d >= opts_local.similarity_threshold) {
		
		}	
		//what time is it?
		#ifdef DEBUGMODE
		GetSystemTime(&sysT);
		//display time
		cout <<"iteration : " << i <<"->end_time : " << sysT.wSecond <<" s , " << sysT.wMilliseconds << " ms" << endl;
		#endif DEBUGMODE
	}
	//have the resulting array. Sorting:
	qsort(nameDistArr, 0, filesNameList.size() - 1);
	//for displaying results
	bool similar = true;
	int simIter = 0;
	//save in file?
	if(opts.writeInFile == true)
	{
		FILE *fp;
		fp = fopen(opts.fileToSave, "w");
		if(fp != NULL)
		{	
			//put results to the file
			fprintf(fp, "*** Pictures found to be identical/close resemblance to %s ***\n\n", opts.file1);
			for(int i = 0; i < filesNameList.size(); i++)
			{
				if (i < 10 && nameDistArr[i].dist <= THRESHOLD)
				{//10 identical pictures
					fprintf(fp, "%g\n", nameDistArr[i].dist);
					fprintf(fp, "%s\n", nameDistArr[i].fileName);
				}
				else
				{//10 similar pictures
					if(similar)
					{
						fprintf(fp, "\n*** Pictures found to be similar to %s ***\n\n", opts.file1);
						similar = false;
					}
					if(nameDistArr[i].dist <= THRESHOLD)
					{
						//goto next iteration, because we already have 10 identical pictures
						simIter = i;
						continue;
					}
					else if(i - simIter <= 10 )
					{
						fprintf(fp, "%g\n", nameDistArr[i].dist);
						fprintf(fp, "%s\n", nameDistArr[i].fileName);
					}
				}
			}
			fclose(fp);
			cout << "results saved in file" << endl;
		}
		else
		{
			cout << "problems with file opening";
		}
	}
	else
	{
		//print on the screen
		printf("*** Pictures found to be identical/close resemblance to %s ***\n", opts.file1);
		for(int i = 0; i < filesNameList.size(); i++)
		{
			if (i < 10 && nameDistArr[i].dist <= THRESHOLD)
			{//10 identical pictures
				printf("%g\n", nameDistArr[i].dist);
				printf("%s\n", nameDistArr[i].fileName);
			}
			else
			{//10 similar pictures
				if(similar)
				{
					printf("\n*** Pictures found to be similar to %s ***\n\n", opts.file1);
					similar = false;
				}
				if(nameDistArr[i].dist <= THRESHOLD)
				{
					//goto next iteration, because we already have 10 identical pictures
					simIter = i;
					continue;
				}
				else if(i - simIter <= 10 )
				{
					printf("%g\n", nameDistArr[i].dist);
					printf("%s\n", nameDistArr[i].fileName);
				}
			}
		}
	}
}

int partition(NameDist* a,int p, int r)
{
   NameDist tmp;
   double x; 
   int j, i; 
   x = a[p].dist; j = r+1; i = p-1;
   while(1){
      while (a[--j].dist > x);
      while (a[++i].dist < x);
      if(i < j) 
      {
         tmp  = a[i];
         a[i] = a[j];
         a[j] = tmp;
      } else {
         return j;
      }
   }
}
void qsort(NameDist *a,int p, int q)
{
    int r;
    if (p < q)
    {
        r = partition (a, p, q);
       cilk_spawn qsort (a, p, r);
       cilk_spawn qsort (a, r+1, q);
    }
	else
	{
		
	}
}