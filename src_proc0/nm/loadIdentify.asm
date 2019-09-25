
//void loadIdentity(float* matrix)
data ".data_demo3d"
	global _identify: word[16] = (float(1),0,0,0,
						  0,float(1),0,0,
						  0,0,float(1),0,
						  0,0,0,float(1));
end ".data_demo3d";


global _loadIdentify:label;
begin ".text_demo3d"
<_loadIdentify>
	ar5 = ar7-2;
	push ar0,gr0;
	push ar6,gr6;
	ar0 = _identify;
	ar6 = [--ar5];
	fpu 0 rep 8 vreg0 = [ar0++];
	fpu 0 rep 8 [ar6++] = vreg0;	
	pop ar6,gr6;
	pop ar0,gr0;
	return;
end ".text_demo3d";