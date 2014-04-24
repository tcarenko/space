#include "puzzle_common.h"
#include "puzzle.h"
#include "pgetopt.h"

typedef struct Opts_ {
    const char *file1;    
    const char *file2;
    int fix_for_texts;
    int exit;
    double similarity_threshold;
} Opts;

void usage(void)
{
    puts("\nUsage: puzzle-diff [-b <contrast barrier for cropping] [-c]\n"
         "   [-C <max cropping ratio>] [-e] [-E similarity threshold] [-h]\n"
         "   [-H <max height>] [-l <lambdas>] [-n <noise cutoff>]\n"
         "   [-p <p ratio>] [-t] [-W <max width>] <file 1> <file 2>\n\n"
         "Visually compares two images and returns their distance.\n\n"
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
    while ((opt = pgetopt(argc, argv, "b:cC:eE:hH:l:n:p:tW:")) != -1) {
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
    opts->file2 = *argv;
    
    return 0;
}

int main(int argc, char *argv[])
{
   
}
