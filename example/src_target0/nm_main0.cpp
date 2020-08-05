#include "nmgl.h"
#include "nmglvs_nmc0.h"
#include <rod_dci_data.h>

/*float triangles[24]{
	0,0,
	0,31,
	31,31,
	
	0, 0,
	30, 0,
	30, 30,
	
};*/

float triangles[24]{
	0, 0,
	30, 0,
	30, 30,

	0, 0,
	30, 0,
	30, 30,

};

float color[48]{
	1,0,0, 1,
	1,0,0, 1,
	1,0,0, 1,

	1,0,1, 1,
	1,0,1, 1,
	1,0,1, 1,
	
};

/*extern "C" {
	int32_t RodDrawFragmentv1(t_RodDciFragment * pFragment);
}*/
int32_t RodDrawFragmentv1(t_RodDciFragment * pFragment);
extern t_RodDciFragment frs_1[];

int main()
{
	nmglvsNm0Init();

	nmglClearColor(0, 0, 0.4f, 1.0f);
	nmglMatrixMode(NMGL_PROJECTION);
	//nmglOrthof(0, 0.5, 0, 0.5, 0, 100);
	nmglOrthof(-2, 2, -2, 2, 0, 100);

	nmglMatrixMode(NMGL_MODELVIEW);
	

	nmglVertexPointer(2, NMGL_FLOAT, 0, triangles);
	nmglColorPointer(4, NMGL_FLOAT, 0, color);
	nmglEnableClientState(NMGL_VERTEX_ARRAY);
	nmglEnableClientState(NMGL_COLOR_ARRAY);
	int offset = 0;
	int offsetY = 1;
	while(nmglvsNm0Run()){
		nmglClear(NMGL_COLOR_BUFFER_BIT);
		RodDrawFragmentv1(frs_1);
		/*nmglTranslatef(1,0,0);
		offset++;
		if (offset == 384) {
			nmglLoadIdentity();
			nmglTranslatef(0, offsetY++, 0);
			offset = 0;
		}
		nmglDrawArrays(NMGL_TRIANGLES, 0, 6);*/

		nmglvsSwapBuffer();
	}
	nmglDisableClientState(NMGL_VERTEX_ARRAY);
	nmglDisableClientState(NMGL_COLOR_ARRAY);
	nmglvsExit_mc12101();
	return 0x600D600D;
}