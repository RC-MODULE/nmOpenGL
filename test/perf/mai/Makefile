
default: config_nmpu0
	

all: config_mc12101_0_gcc config_mc12101_1_gcc run outdir
	


run:


all_mc12101: configure0 configure1
	testperf run

configure0 config_nmpu0:
	testperf config -i ../../../include/nmgltex_nm0.h -b make_mc12101_nmpu0-ld
	testperf config -i ../../../include/service.h -b make_mc12101_nmpu0-ld

configure1 config_nmpu1:
	testperf config -i ../../../include/nmgltex_nm0.h -b make_mc12101_nmpu1-ld --point fixed	
	testperf config -i ../../../include/service.h -b make_mc12101_nmpu0-ld --point fixed	

run:
	testperf run

kill:
	testperf kill

outdir:
	testperf outdir