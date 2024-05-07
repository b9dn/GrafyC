#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "init.h"

double losuj(int a, int b) {
    return (double)rand() / RAND_MAX * (b-a) + a;
}

int czy_bylo(graph_t* gr, int i, int j, int nr_wezla) {
    for(int k = 0; k < j; k++) {
        if(gr[i].edg[k]->node == nr_wezla)
            return 1;
    }
    return 0;
}

void gr_intro(graph_t* gr, int rows, int columns) {
    for(int i = 0; i < rows*columns; i++) {
        gr[i].node = i;
        gr[i].edg = malloc(sizeof(graph_t*) * 4);
        gr[i].val_edg = malloc(sizeof(double) * 4);
        gr[i].nmb_edg = 0;
    }

}

graph_t* graph_read(char *file, int* height, int* width) {
    int a,rows,columns; // liczba wierszy to wysokosc grafu a kolumn jego szerokosc
    FILE *in = fopen (file,"r");
    if(in==NULL) {
        printf("Nie udalo sie odczytac pliku\n");
        return NULL;
    }
    if(fscanf(in,"%i %i\n",&rows,&columns)!=2 || rows <= 0 || columns <= 0) {
        printf("Zly format pliku\n");
        return NULL;
    }
    
    *height = rows;
    *width = columns;

    graph_t* gr = malloc(sizeof(graph_t) *rows*columns);
    char* str = malloc(sizeof(char) * 1000);
    char* tmpstr = str;

    gr_intro(gr,rows,columns);

    int offset = 0;
    int i = 0; // aktualny wezel
    int j = 0;
    int curr_col = 1, curr_row = 1;
    int x;

    while(fgets(str, 1000, in) != NULL) {
        if(curr_col > columns) {
            curr_col = 1;
            curr_row++;
        }

        while( (x = sscanf( str, "%i :%lf%n", &a, &(gr[i].val_edg[j]), &offset )) == 2) {
            if(a >= 0 && a < columns*rows  // czy a jest w ogole w przedziale, czy sie nie powtarza i edg > 0
                        && czy_bylo(gr, i, j, a) == 0 && gr[i].val_edg[j] > 0 
                        && gr[i].val_edg[j] < INT_MAX) {
                if(a-columns == i || a+columns == i || //spr czy mozliwe polaczenia
                    (a+1 == i && curr_col-1 < columns) || 
                    (a-1 == i && curr_col+1 > 1)) {
                        
                    gr[i].edg[j++] = &gr[a];
                    gr[i].nmb_edg++;
                    str += offset;
                }
                else {
                    str += offset;
                    printf("Podano nieprawidlowe polaczenie do wezla %i", i);
                    graph_free(&gr, rows, columns);
                    return NULL;
                }

            }
            else {
                str += offset;
                printf("Bledny format linii %i we wczytanym pliku\n", i+2);
                graph_free(&gr, rows, columns);
                return NULL;
            }

            if(j > 3)	
                break;
        }

        if(x == 1) {
            printf("Bledny format linii %i we wczytanym pliku\n", i+2);
            graph_free(&gr, rows, columns);
            return NULL;
        }

        for(int b = j; b < 4; b++)  // zapelnienie pozostalych, 
            gr[i].edg[b] = NULL; // nieuzywanych polaczen wart NULL

        j = 0;
        str = tmpstr;

        curr_col++;
        i++;
        if(i >= columns*rows)  break;
    }

    free(str);
    fclose(in);
    return gr;
}

graph_t* graph_init(int height, int width, double a, double b) { // a i b to przedzial losowania <a,b>
    graph_t* graf = malloc(sizeof(graph_t) *height*width);
    int curr_row = 1; // aktualny wiersz
    int curr_col = 1; //  -,,-   kolumna

    srand( time(NULL) );
    
    for(int i = 0; i < height*width; i++) {
        if(curr_col > width) {
            curr_col = 1;
            curr_row++;
        }

        graf[i].node = i; // ktory wezel
        graf[i].nmb_edg = 0; // liczba krawedzi
        graf[i].edg = malloc(sizeof(graph_t*) * 4); // wskazniki do innych wezlow
        graf[i].val_edg = malloc(sizeof(double) * 4); // wartosci krawedzi

        if(i-width >= 0) { // sprawdza czy jest polaczenie w gore
            graf[i].nmb_edg++;
            graf[i].edg[0] = &(graf[i - width]);
            graf[i].val_edg[0] = losuj(a, b);	
        }
        else {
            graf[i].edg[0] = NULL;
            graf[i].val_edg[0] = 0;
        }

        ////////////////////////////
        if(i+1 < curr_row * width) { // w prawo
            graf[i].nmb_edg++;
            graf[i].edg[1] = &(graf[i + 1]);
            graf[i].val_edg[1] = losuj(a, b);
        }
        else {
            graf[i].edg[1] = NULL;
            graf[i].val_edg[1] = 0;
        }

        ////////////////////////////
        if(i+width < width*height) { // w dol
            graf[i].nmb_edg++;
            graf[i].edg[2] = &(graf[i + width]);
            graf[i].val_edg[2] = losuj(a, b);
        }
        else {
            graf[i].edg[2] = NULL;
            graf[i].val_edg[2] = 0;
        }

        ////////////////////////////
        if(i-1 >= (curr_row - 1) * width) { // w lewo
            graf[i].nmb_edg++;
            graf[i].edg[3] = &(graf[i - 1]);
            graf[i].val_edg[3] = losuj(a, b);
        }
        else {
            graf[i].edg[3] = NULL;
            graf[i].val_edg[3] = 0;
        }
        
        curr_col++;

    }
    
    return graf;
}

void graph_free(graph_t** graf, int height, int width) {
    for(int i = 0; i < height*width; i++) {
        free( (*graf)[i].edg );
        free( (*graf)[i].val_edg );
    }
    free(*graf);
    *graf = NULL;
} 

void graph_print(graph_t* graf, int height, int width) {
    for(int i = 0; i < height*width; i++) {
        for(int j = 0; j < 4; j++) {
            if( graf[i].edg[j] != NULL )
                printf("%i: %lf ", graf[i].edg[j]->node, graf[i].val_edg[j]);
        }
        printf("\n");
    }
}

void graph_fwrite(char* fout ,graph_t* graf, int height, int width) {
    FILE* out = fopen(fout, "w");
    if(out == NULL) {
        printf("Blad - plik %s", fout);
        exit(1);
    }
    fprintf(out, "%i %i\n", height, width);
    for(int i = 0; i < height*width; i++) {
        for(int j = 0; j < 4; j++) {
            if(graf[i].edg[j] != NULL)
                fprintf(out, "%i: %f ", graf[i].edg[j]->node, graf[i].val_edg[j]);
        }
        fprintf(out, "\n");
    }
    fclose(out);
}

