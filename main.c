#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include "init.h"
#include "dijkstr.h"
#include "bfs.h"

char* help=
    "\tPOMOC:\n"
    "\tNajpierw nalezy wczytac graf z pliku lub go wygenerowac\n"
    "\tGenerowanie grafu: -g -a [liczba wierszy (wysokosc) grafu] -b [liczba kolumn (szerokosc) grafu]\n"
    "\t-m [dolny zakres losowanych wartosci (nieujemne)] -n [gorny zakres losowanych wartosci (nieujemne)]\n"
    "\tWczytanie grafu z pliku: -i [nazwa pliku]\n"
    "\tZapisanie grafu do pliku: -o [nazwa pliku]\n"
    "\tSprawdzenie spojnosci grafu: -s\n"
    "\tWyznaczenie najkrotszej sciezki algorytmem dijkstry: -d -e[pierwszy wezel] -f [drugi wezel]\n"
    "\tWypisanie grafu: -p\n\n";

int main(int argc, char **argv) {
    graph_t* gr=NULL;
    char *fout=NULL;
    char *fin=NULL;
    int opt, height=10, width=10, sflag=0, gflag=0, pflag=0, dflag=0;
    double min=1, max=10;
    int fn=-1, sn=-1;

    while((opt = getopt(argc, argv, "ga:b:m:n:o:i:de:f:sp")) != -1) {
        switch(opt) {
            case 'g':
                gflag=1;
                break;
            case 'a':
                height = atoi(optarg);
                break;
            case 'b':
                width = atoi(optarg);
                break;
            case 'm':
                min = atof(optarg);
                break;
            case 'n':
                max = atof(optarg);
                break;
            case 'o':
                fout = optarg;
                break;
            case 'i':
                fin = optarg;
                break;
            case 'd':
                dflag=1;
                break;
            case 'e':
                fn = atoi (optarg);
                break;
            case 'f':
                sn = atoi (optarg);
                break;
            case 's':
                sflag=1;
                break;
            case 'p':
                pflag=1;
                break;
            default:
                fprintf (stderr, help, argv[0]);
                exit (EXIT_FAILURE);
        }
    }
    
    if(gflag==1) {	
        if(fin!=NULL) {
            printf("Nie mozna jednoczesnie wczytac grafu i go wygenerowac!\n");
            fprintf (stderr, help, argv[0]);
            return -1;
        }
        if(min>=9999999 || max>=9999999) {
            printf("Maksymalna wartosc na krawedziach grafu to 9999999\n");
            fprintf (stderr, help, argv[0]);
            return -1;
        }

        if(height>0 && width>0 && min>0 && max>0 && max>min) {
            graph_t* gr=graph_init(height,width,min,max);
            if(gr==NULL)
                return -1;
            if(pflag==1)
                graph_print(gr,height,width);
            if(fout!=NULL)
                graph_fwrite(fout,gr,height,width);
            if(sflag==1)
                bfs(gr,height,width);
            if(dflag==1) {
                if(fn>=height*width || sn>=height*width || fn<0 || sn<0 || fn==sn)
                    printf("Podano nieprawidlowe wezly dla algorytmu dijktstry\n");
                else
                    dijkstra(gr,height*width,fn,sn);
            }

            graph_free(&gr,height,width);
            return 0;
        }
        else {
            printf("Podane zle wartosci do generacji grafu\n");
            fprintf (stderr, help, argv[0]);
            return -1;
        }	
        
    }		
    else if(fin != NULL) {		
        graph_t* gr=graph_read(fin,&height,&width);
        if(gr==NULL)
            return -1;

        if(pflag==1)
            graph_print(gr,height,width);
        if(fout!=NULL)
            graph_fwrite(fout,gr,height,width);
        if(sflag==1)
            bfs(gr,height,width);
        if(dflag==1) {
            if(fn>=height*width || sn>=height*width || fn<0 || sn<0 || fn==sn)
                printf("Podano nieprawidlowe wezly dla algorytmu dijktstry\n");
            else
                dijkstra(gr,height*width,fn,sn);
        }
        graph_free(&gr, height, width);
        return 0;
    }
    else {
        printf("Nalezy podac graf\n");
        fprintf(stderr, help, argv[0]);
        return -1;
    }

    
    return 0;
}

