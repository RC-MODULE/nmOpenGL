#include "nmtype.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
///////////////////////////////////////////////////////////////
//int abs(int x, int y);
//--------------------------------------------------------------
void printHistograms(nm32s *input,int quan, int dep);
int corruptImage(nm32s *input,int w, int h);
int compareImages(nm32s *input,nm32s *gold,int w, int h);
//--------------------------------------------------------------
///////////////////////////////////////////////////////////////
//CONFIGURES
//--------------------------------------------------------------
#define HISTOGRAM_SIZE  (16)
#define HISTOGRAMS_W 4
#define HISTOGRAMS_H 4
//--------------------------------------------------------------
#define RED_ARGB 0x00FF0000
#define GREEN_ARGB 0x0000FF00
#define BLUE_ARGB 0x000000FF
//--------------------------------------------------------------
#define pixel (int)(*(input+j*w+i))
#define gpixel (int)(*(gold+j*w+i))
//--------------------------------------------------------------
#define RED(x) (((x)&RED_ARGB)>>16)
#define GREEN(x) (((x)&GREEN_ARGB)>>8)
#define BLUE(x) ((x)&BLUE_ARGB)
//--------------------------------------------------------------
#define USED_IMAGE_SIDE 32
#define N_ERRORS USED_IMAGE_SIDE
//#define N_ERRORS 1
///////////////////////////////////////////////////////////////
void printHistograms(nm32s *input,int quan, int dep)
{
	int i=0;
	int j=0;
	printf("quan=%d\n",quan);
	printf("dep=%d\n",dep);
	for(i=0;i<quan;i++)
	{
		for(j=0;j<dep;j++)
		{
			printf("%d ",*(input+i*dep+j));
		}
		printf(" \n");
	}
}
///////////////////////////////////////////////////////////////
int sabs(int x, int y)
{
	if(x>y)
	{
		return x-y;
	}
	else
	{
		return y-x;
	}
	
}
///////////////////////////////////////////////////////////////
int corruptImage(nm32s *input,int w, int h)
{
	static int xs=0;
	static int ys=0;
	//int x=rand()%w;
	//int y=rand()%h;
	int i=0;
	int j=0;
	int nerr=0;
	for(i=xs;i<w;i++)
	{
		for(j=ys;j<h;j++)
		{
			*(input+h*i+j)=(nm32s)0xff;
			nerr++;
			if(nerr>=N_ERRORS)
			{
				xs=i;
				ys=j+1;
				if (ys == h)
				{
					ys=0;xs++;
				} 
				printf("*******\n");
				printf("xs=%d ys=%d\n",xs,ys);
				printf("*******\n");
				return 0;
			}
		}
	}
	return 0;


}
///////////////////////////////////////////////////////////////
int compareImages(nm32s *input,nm32s *gold, int w, int h)
{
int st=0;
int i,j;
double fr=1.0/3.0;
double fg=1.0/3.0;
double fb=1.0/3.0;
double fluency=0.0;
double rezult = -1.0;
int lum=0;
int glum=0;
double norma=w*h/(HISTOGRAMS_W*HISTOGRAMS_H*0.5);
int histo_w=w/HISTOGRAMS_W;//int32_t
int histo_h=h/HISTOGRAMS_H;//int32_t
int historgam_count=HISTOGRAMS_W*HISTOGRAMS_H;
int histograms[HISTOGRAMS_W*HISTOGRAMS_H][HISTOGRAM_SIZE];
int ghistograms[HISTOGRAMS_W*HISTOGRAMS_H][HISTOGRAM_SIZE];
int histo_index = 0;
int histo_gindex = 0;
int inner_index = 0;
int inner_gindex = 0;
int histo_diffs [HISTOGRAMS_W*HISTOGRAMS_H];
double sigma_summ=0.0;
double linear_summ=0.0;

	for(i=0;i<w*h;i++)
	{
		if (*(input+i) != *(gold+i)) st=1;
	}
	if (st == 0) return 1; //Exact match
	
	for(i=0;i<HISTOGRAMS_W*HISTOGRAMS_H;i++)
	{
		histo_diffs[i]=0;
		for(j=0;j<HISTOGRAM_SIZE;j++)
		{
			histograms[i][j]=0;	
			ghistograms[i][j]=0;	
		}
	}
	//printf("INIT=\n");	
	///printHistograms(&histograms[0][0],HISTOGRAMS_W*HISTOGRAMS_H,HISTOGRAM_SIZE);
	//printHistograms(&ghistograms[0][0],HISTOGRAMS_W*HISTOGRAMS_H,HISTOGRAM_SIZE);
	//printf("\n");
	for(j=0;j<h;j++)
	{
		for(i=0;i<w;i++)
		{
			lum=RED(pixel)*fr+GREEN(pixel)*fg+BLUE(pixel)*fb;//check and delete
			(*(input+j*w+i))=pixel&0xFF00FFFF | (lum<<16)&0x00FF0000;
			
			glum=RED(gpixel)*fr+GREEN(gpixel)*fg+BLUE(gpixel)*fb;//check and delete
			(*(gold+j*w+i))=gpixel&0xFF00FFFF|(glum<<16)&0x00FF0000;

			lum=RED(pixel)*0.3333+GREEN(pixel)*0.3333+BLUE(pixel)*0.3333;
			(*(input+j*w+i))=pixel&0xFF00FFFF | (lum<<16)&0x00FF0000;			

			glum=RED(gpixel)*0.3333+GREEN(gpixel)*0.3333+BLUE(gpixel)*0.3333;
			(*(gold+j*w+i))=gpixel&0xFF00FFFF|(glum<<16)&0x00FF0000;

			linear_summ += abs(lum-glum);	 

			inner_index=lum/HISTOGRAM_SIZE;
			inner_gindex=glum/HISTOGRAM_SIZE;

			histograms[histo_index][((unsigned int)(lum/HISTOGRAM_SIZE))&0xff]++;
			ghistograms[histo_gindex][((unsigned int)(glum/HISTOGRAM_SIZE))&0xff]++;
			//printf("histoindex=%d\n",histo_index);
			if(((i+1)!=w)&&((i+1)%histo_w ==0 ))
			{
				histo_index++;
				histo_gindex++;
			}

		}
		if((j+1)%histo_h == 0)
		{
			histo_index++;
			histo_gindex++;
			
		}
		else
		{
			histo_index-=(HISTOGRAMS_W-1);
			histo_gindex-=(HISTOGRAMS_W-1);
		}
		
	}
//Compare
	
	for(i = 0; i < historgam_count; ++i) {
		for(j = 0; j < HISTOGRAM_SIZE; ++j) {
			histo_diffs[i] += abs(histograms[i][j]- ghistograms[i][j]);
		}
	}
		
	for(i = 0; i < historgam_count; i++) {
		sigma_summ += (histo_diffs[i] / norma / (double) historgam_count);
	}
	
	//K = 1.0 - sigma_summ;
	

	
	linear_summ =  1.0 - (linear_summ / 255.0 / (w * h));
	printf("linear_summ=%lf\n",linear_summ);
	printf("sigma_summ=%lf\n",sigma_summ);
	printf("ending=%lf\n",((1.0 / (1.0 + pow(1.0 + (fluency + 1.0) * sqrt(1.0 / (1.0-sigma_summ)),
	                                        (-4.0 - fluency - (1.0 - sigma_summ) + 12.0 * (sigma_summ)))))));
	rezult = linear_summ * ( sigma_summ == 0?1.0:((1.0 / (1.0 + pow(1.0 + (fluency + 1.0) * sqrt(1.0 / (1.0-sigma_summ)),
	                                        (-4.0 - fluency - (1.0 - sigma_summ) + 12.0 * (sigma_summ)))))));


	printf ("\n\nrezult=%lf\n\n",rezult);
	printf("histograms:\n");
	printHistograms(&histograms[0][0],HISTOGRAMS_W*HISTOGRAMS_H,HISTOGRAM_SIZE);
	printf("ghistograms:\n");
	printHistograms(&ghistograms[0][0],HISTOGRAMS_W*HISTOGRAMS_H,HISTOGRAM_SIZE);
	if (rezult > 0.8) return 2; //Good match
	if (rezult > 0.5) 	  return 3; // Partial match
	return 0; // Bad match
}
/*
int main()
{
	
	nm32s *im1,*im2;
	int i=0;
	int v=0;
	int s=0;
	int nerr=0;
	nm32s image1[USED_IMAGE_SIDE*USED_IMAGE_SIDE];
	nm32s image2[USED_IMAGE_SIDE*USED_IMAGE_SIDE];
	int w=USED_IMAGE_SIDE,h=USED_IMAGE_SIDE;

	im1=image1;
	im2=image2;	
	
	for(i=0;i<USED_IMAGE_SIDE*USED_IMAGE_SIDE;i++)
	{
		v=rand()%55;		
		image1[i]=v;
		image2[i]=v;
	} 	
	printf("Statuses:\n   Exact match   =1\n   Good match    =2\n   Partial match =3\n   Bad match =0\n");
	s=compareImages(im1,im2,w,h);
	printf("Status when should be exact=%d\n",s);
	i=1;
	while(s!=0)
	{
		corruptImage(im1,w,h);
		printf("\n%i image1:\n",i);
		printf("==================================");
		printHistograms(&image1[0],USED_IMAGE_SIDE,USED_IMAGE_SIDE);
		printf("\nimage2\n");
		printHistograms(&image2[0],USED_IMAGE_SIDE,USED_IMAGE_SIDE);
		printf("==================================");
		s=compareImages(im1,im2,w,h);
		nerr+=N_ERRORS;
		printf("%d:Status after %d/%d(%6.2lf%%)errors =%d\n",i,nerr,USED_IMAGE_SIDE*USED_IMAGE_SIDE,(double)(((double)nerr/	(double)(USED_IMAGE_SIDE*USED_IMAGE_SIDE))*100),s);
		if (nerr >= (USED_IMAGE_SIDE*USED_IMAGE_SIDE)) {printf("Iteration safe border is reached\n");break;}
		i++;
	}
	return 0;
}
*/
