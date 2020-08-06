texture-demo-gcc (x86 and mc12101) works fine with next hal and nmpp commits:
hal - 6392ebdd
nmpp - 2fc2d508e

With hal commit e19c2b25 texture-demo-gcc sometimes crashes at demo3d-target0.exe (getHead() call). "make run" failed, but "make runhost, make runtarget0, make runtarget1" works fine. To fix this error demo3d-host.exe should be run manually and independently of target0 and target1.

