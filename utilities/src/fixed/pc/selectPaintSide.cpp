#include "nmpp.h"
extern "C" {

	void selectPaintSide(nm32s* crossProd,int X, int Y, nm32s* pDst, int nSize){
		for(int i=0;i<nSize;i++)
			if (crossProd[i]<0)
				pDst[i]=X;
			else
				pDst[i]=Y;
	}

}