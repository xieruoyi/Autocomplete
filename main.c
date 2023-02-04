#include <stdlib.h>
#include <stdio.h>
#include "autocomplete.h"
#include <ctype.h>
#include <string.h>

int my_strcmp_rec(const void *str1, const void *str2)
{
    if(strcmp((const char*)str1,(const char*)str2) != 0 ){
        return strcmp((const char*)str1,(const char*)str2);
    }
    return my_strcmp_rec(str1+1,str2+1);
}

void read_in_terms(struct term **terms, int *pnterms, char *filename){
    char line[200];
    char number[20];
    int val;

    FILE *fp = fopen(filename, "r");
    fgets(number, sizeof(number), fp);
    val = atoi(number);//找出terms的数量
    *pnterms = val;
    *terms = (struct term *)malloc((*pnterms)*sizeof(struct term));
    for(int i = 0; i < val; i++){
        fgets(line, sizeof(line), fp); //read in at most sizeof(line) characters
                                     //(including '\0') into line

        (*terms+i)->weight = (double)(atof(line));
        printf("%f\n",(*terms+i)->weight);
        for(int j = 0;j<strlen(line);j++){
            if (line[j] =='\t'){
                int k = j+1;
                while(j<sizeof(line)&& line[j+1]!='\n'){
                    j++;
                }
                strncpy((*terms)[i].term,&line[k],j-k+1);
//                printf("%s\n",(*terms)[i].term);
            }
        }
        }
    qsort(*terms, val, sizeof(struct term),my_strcmp_rec);
    for(int i=0;i<=val;i++){
        printf("%s\n",(*terms)[i].term);
        printf("%d\n",((*terms)[i]).weight);
    }


}

int lowest_match(struct term *terms, int nterms, char *substr){
    int low = 0;
    int high = nterms-1;
    int mid = (low + high )/2;
    while (low+1 < high){

        if (strncmp((terms+mid)->term,substr,strlen(substr)) >= 0 && strncmp((terms+low)->term,substr,strlen(substr)) <= 0){
            high = mid;
            mid = (mid + low + 1)/2;
        }else if (strncmp((terms+mid)->term,substr,strlen(substr)) < 0 && strncmp((terms+high)->term,substr,strlen(substr)) >= 0){
            low = mid;
            mid = (mid + high + 1)/2;
        }

    }
    if(strncmp((terms+mid)->term,(terms+high)->term,strlen(substr)) == 0 &&strncmp((terms+mid)->term,substr,strlen(substr)) == 0){
        return high;
    }else if((strncmp((terms+mid)->term,(terms+low)->term,strlen(substr)) == 0 &&strncmp((terms+mid)->term,substr,strlen(substr)) == 0)){
        return low;
    }else if(strncmp((terms+mid)->term,substr,strlen(substr)) != 0){
        return -1;
    }
}

int highest_match(struct term *terms, int nterms, char *substr){
    int low = 0;
    int high = nterms-1;
    int mid = (low + high )/2;
    while (low+1 < high){

        if (strncmp((terms+mid)->term,substr,strlen(substr)) > 0 && strncmp((terms+low)->term,substr,strlen(substr)) <= 0){
            high = mid;
            mid = (mid + low)/2;
        }else if (strncmp((terms+mid)->term,substr,strlen(substr)) <= 0 && strncmp((terms+high)->term,substr,strlen(substr)) >= 0){
            low = mid;
            mid = (mid + high)/2;
        }

    }
    if(strncmp((terms+mid)->term,(terms+high)->term,strlen(substr)) == 0 &&strncmp((terms+mid)->term,substr,strlen(substr)) == 0){
        return high;
    }else if((strncmp((terms+mid)->term,(terms+low)->term,strlen(substr)) == 0 &&strncmp((terms+mid)->term,substr,strlen(substr)) == 0)){
        return low;
    }else if(strncmp((terms+mid)->term,substr,strlen(substr)) != 0){
        return -1;
    }

}

int cmpfunc (const void * a, const void * b) {
    double weighta = ((struct term *)a)->weight;
    double weightb = ((struct term *)b)->weight;
    return (weightb-weighta);
}

void autocomplete(struct term **answer, int *n_answer, struct term *terms, int nterms, char *substr){
    int b = highest_match(terms, nterms, substr);
    int a = lowest_match(terms,nterms, substr);


    *answer = (struct term *)malloc((*n_answer) * sizeof(struct term));
    *n_answer = b - a + 1;
    if(a==-1 || b==-1){
        *n_answer = 0;
    }

    for(int i = a; i <= b; i++){
        double w = (terms+i)->weight;
        (*answer + i)->weight = w;
        strcpy(((*answer)[i-a]).term,((terms)[i]).term);

//        printf("%s\n",((*answer)[i-a]).term);
    }
    qsort(*answer, *n_answer, sizeof(struct term), cmpfunc);
    for(int i = 0; i < *n_answer; i++){
        printf("%s\n",((*answer)[i]).term);
        printf("%d\n",((*answer)[i]).weight);
    }


}



int main(void)
{
    struct term *terms;
    int nterms;
    read_in_terms(&terms, &nterms, "City.txt");
//    lowest_match(terms, nterms, "Mo");
//    highest_match(terms, nterms, "Mo");

    struct term *answer;
    int n_answer;
    autocomplete(&answer, &n_answer, terms, nterms, "M");
    //free allocated blocks here -- not required for the project, but good practice


    return 0;
}
