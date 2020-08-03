#include <stdio.h> 
#include <stdlib.h>

int get_amm_poligone(
	FILE *fp 
)
{
	int i=0;
	char str[200];
	
	char str2count[] = "facet";

	while(fscanf(fp,"%s",str) != EOF){
		if(strcmp(str2count,str)==0){
			i++;
		}
	} 

	fclose(fp);
	return(i);	
}
