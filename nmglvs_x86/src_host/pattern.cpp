#include "nmpp.h"
#include <cmath>
#include "demo3d_nm1.h"


void fillLineRight(	int x1,
					int y1,
					int x2, 
					int y2, 
					int width, 
					int height, 
					nm8s* dst, 
					int color)
{
	
if (x2==x1)
	{
		x1 = (x1==width) ? 0: x1;
		for (int y=0;y<height;y++)
		{
			for (int x=x1;x<width;x++)
				dst[y*width+x]=color;
		}
		return;
	}

	if (y2==y1)
	{
		for (int y=0;y<height;y++)
		{
			for (int x=x1;x<width;x++)
				dst[y*width+x]=color;
		}
		return;
	}


	float k,b;
	if (x1==0 && y1==0)
	{
		k =(float)y2/x2;
		for (int y=0;y<height;y++)
		{
			double xst = y/k;
			//xst = floor(xst + 0.5);
			xst = (xst>0) ?	xst: 0;
			for (int x=xst;x<width;x++)
				dst[y*width+x]=color;
		}
		return;
	}

	k = (float)(y2-y1)/(x2-x1);
	b = y2-k*x2;
	for (int y=0;y<height;y++)
	{
		float xst = (float)(y-b)/k;
		//xst = floor(xst + 0.5);
		xst = (xst>0) ?	xst: 0;
		for (int x=xst;x<width;x++)
			dst[y*width+x]=color;
	}
	return;
}

void fillLineLeft(	int x1,
					int y1,
					int x2, 
					int y2, 
					int width, 
					int height, 
					nm8s* dst, 
					int color)
{
	
	if (x2==x1)
	{
		x1 = (x1==width || x1<0) ? 0: x1;
		for (int y=0;y<height;y++)
		{
			for (int x=0;x<x2;x++)
				dst[y*width+x]=color;
		}
		return;
	}

	if (y2==y1)
	{
		for (int y=0;y<height;y++)
		{
			for (int x=0;x<x2;x++)
				dst[y*width+x]=color;
		}
		return;
	}


	float k,b;
	if (x1==0 && y1==0)
	{
		k =(float)y2/x2;
		for (int y=0;y<height;y++)
		{
			float xst = y/k;
			xst = floor(xst + 0.5);
			//xst++;
			xst = (xst<width) ?	xst: width;
			for (int x=0;x<xst;x++)
				dst[y*width+x]=color;
		}
		return;
	}

	k = (float)(y2-y1)/(x2-x1);
	b = y1-k*x1;
	for (int y=0;y<height;y++)
	{
		float xst = (float)(y-b)/k;
		xst = floor(xst + 0.5);
		//xst++;
		xst = (xst<width) ?	xst: width;
		for (int x=0;x<xst;x++)
			dst[y*width+x]=color;
	}
	return;
}

void fillPattern(nm8s* pDstSource,int width, int height)
{
	int x=0;
	int y=0;
	int size = width*height;		//size of one pattern
	int nOffSets_X = OFFSETS;
	int step=STEP_PTRN;
	int color= 1;
	int cnt=0;
	

//-------------------fill-right--------------
//step on the left side
	for(y=0;y<height;y+=step)
	{
			nm8s* dst = nmppsAddr_8s(pDstSource,y/step*OFFSETS*size);
			for(int i=0;i<OFFSETS;i++)
			{
				nm8s* dsti =nmppsAddr_8s(dst,i*size);
				if (y != 0)
					fillLineRight(i, 0, i - width, y, width, height, (nm8s*)dsti, color);
				else
					fillLineRight(0, 0, 0, height, width, height, (nm8s*)dsti, color);
			}
	}	
//step on the up side
	for (x = 0; x<width; x += step)
	{
		nm8s* dst = nmppsAddr_8s(pDstSource, (x + height) / step*OFFSETS*size);
		for (int i = 0; i<OFFSETS; i++)
		{
			nm8s* dsti = nmppsAddr_8s(dst, i*size);
			fillLineRight(i, 0, x + i - width, height, width, height, (nm8s*)dsti, color);
		}
	}
	for(x=0;x<width;x+=step)
	{
			nm8s* dst = nmppsAddr_8s(pDstSource,(x+height+width)/step*OFFSETS*size);
			for(int i=0;i<OFFSETS;i++)
			{
				nm8s* dsti =nmppsAddr_8s(dst,i*size);
				fillLineRight(i, 0,  x+i, height, width, height, (nm8s*) dsti, color);
			}
	}
//step on the right side
	for(y=0;y<height;y+=step)
	{
			nm8s* dst = nmppsAddr_8s(pDstSource,(y+(height+ 2*width))/step*OFFSETS*size);
			for(int i=0;i<OFFSETS;i++)
			{
				nm8s* dsti =nmppsAddr_8s(dst,i*size);
				fillLineRight(i, 0, width +i, height-y, width, height, (nm8s*) dsti, color);
			}
	}
//------------------------------------



//----------------------------------------fill-left--------------
//step on the left side
	for(y=0;y<height;y+=step)
	{
			nm8s* dst = nmppsAddr_8s(pDstSource,(y+ (2*height + 2*width))/step*OFFSETS*size);
			for(int i=0;i<OFFSETS;i++)
			{
				nm8s* dsti =nmppsAddr_8s(dst,i*size);
				if(y!=0)
					fillLineLeft(i, 0, i- width, y, width, height, (nm8s*) dsti, color);
				else
					fillLineLeft(0, 0, 0, height, width, height, (nm8s*)dsti, 0);
			}
	}	
//step on the up side
	for(x=0;x<width;x+=step)
	{
			nm8s* dst = nmppsAddr_8s(pDstSource,(x+ (3*height + 2*width))/step*OFFSETS*size);
			for(int i=0;i<OFFSETS;i++)
			{
				nm8s* dsti =nmppsAddr_8s(dst,i*size);
				fillLineLeft(i, 0,  x+i- width, height, width, height, (nm8s*) dsti, color);
			}
	}
	for (x = 0; x<width; x += step)
	{
			nm8s* dst = nmppsAddr_8s(pDstSource, (x + (3 * height + 3 * width)) / step*OFFSETS*size);
			for (int i = 0; i<OFFSETS; i++)
			{
				nm8s* dsti = nmppsAddr_8s(dst, i*size);
				fillLineLeft(i, 0, x + i, height, width, height, (nm8s*)dsti, color);
			}
	}
//step on the right side
	for(y=0;y<height;y+=step)
	{
			nm8s* dst = nmppsAddr_8s(pDstSource,(y+ (3 * height + 4* width))/step*OFFSETS*size);
			for(int i=0;i<OFFSETS;i++)
			{
				nm8s* dsti =nmppsAddr_8s(dst,i*size);
				fillLineLeft(i, 0, width +i, height-y, width, height, (nm8s*) dsti, color);
			}
	}
//------------------------------------

}