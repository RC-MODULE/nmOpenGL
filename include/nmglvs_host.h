#ifndef __NMGLVS_HOST_H__
#define __NMGLVS_HOST_H__

/*class HostTargetCommunication {
private:
	HostTargetCommunication(){}		//����������� ����������
	~HostTargetCommunication() {}	//� ����������
public:
	static HostTargetCommunication &get() {
		static HostTargetCommunication htc;
		return htc;
	}

	int exit;
};*/

int nmglvsHostReadImage(int* dstImage);
int nmglvsHostInit();
int nmglvsExit(unsigned *result);

#endif
