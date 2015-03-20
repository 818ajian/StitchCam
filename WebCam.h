//
// WebCam.h for Camera enumeration
//

#include <opencv2/highgui/highgui.hpp>
#include <string.h>

#ifdef _MSC_VER
#include <windows.h>
#include <dshow.h>
#define HAVE_MSMF
#define HAVE_VFW
#endif

using namespace cv;

typedef bool (*PENUMDEVICE)(void *pItem, char *pDeviceName);

class WebCam: public VideoCapture
{
public:
	// pCallback��NULL�̏ꍇ�A�J�����̑䐔������Ԃ�
	// pCallback���ݒ肳��Ă�����R�[���o�b�N�ŃJ��������Ԃ�
	// pItem�̓R�[���o�b�N�֐��œn�����
	static int EnumDevices(PENUMDEVICE pCallback, void *pItem);

	/*
		Usage:
		
		// enum callback function 
		static int enumCamera(void* pItem, char *pName)
		{
			printf("%s\n", pName);
			return 0;
		}

		int main()
		{
			int cameraCount;
			cameraCount = Camera::EnumDevices((PENUMDEVICE)&enumCamera, NULL);
		}
	*/

	//bool open(const string& filename, int mode);

public:
	void SetName(char *name) {strncpy(this->name, name, 80);}
	char *GetName() {return name; }

protected:
	char name[80];
};