
default: config_nmpu0
	

all: config_mc12101_0_gcc config_mc12101_1_gcc run outdir
	


run:


all_mc12101:
	testperf config -i ../../../include -b make_mc12101_nmpu0
	testperf config -i ../../../include -b make_mc12101_nmpu1 --point fixed
	testperf run

configure0 config_nmpu0:
	testperf config -i ../../../include -b make_mc12101_nmpu0-ld

configure1 config_nmpu1:
	testperf config -i ../../../include -b make_mc12101_nmpu1-ld --point fixed	

run:
	testperf run

kill:
	testperf del

outdir:
	testperf outdir