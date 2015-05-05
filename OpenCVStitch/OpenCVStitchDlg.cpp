
// OpenCVStitchDlg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "OpenCVStitch.h"
#include "OpenCVStitchDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace cv;

extern "C"
{
#include <cuda_runtime_api.h>
#pragma comment(lib, "cudart.lib")
}

#ifdef _DEBUG
//#pragma comment(lib, "opencv_calib3d2410d.lib")
#pragma comment(lib, "opencv_core2410d.lib")
//#pragma comment(lib, "opencv_features2d2410d.lib")
//#pragma comment(lib, "opencv_flann2410d.lib")
#pragma comment(lib, "opencv_gpu2410d.lib")
#pragma comment(lib, "opencv_highgui2410d.lib")
#pragma comment(lib, "opencv_imgproc2410d.lib")
//#pragma comment(lib, "opencv_legacy2410d.lib")
//#pragma comment(lib, "opencv_ml2410d.lib")
//#pragma comment(lib, "opencv_nonfree2410d.lib")
//#pragma comment(lib, "opencv_objdetect2410d.lib")
//#pragma comment(lib, "opencv_ocl2410d.lib")
#pragma comment(lib, "opencv_stitching2410d.lib")
//#pragma comment(lib, "opencv_video2410d.lib")
#else
//#pragma comment(lib, "opencv_calib3d2410.lib")
#pragma comment(lib, "opencv_core2410.lib")
#pragma comment(lib, "opencv_features2d2410.lib")
//#pragma comment(lib, "opencv_flann2410.lib")
#pragma comment(lib, "opencv_gpu2410.lib")
#pragma comment(lib, "opencv_highgui2410.lib")
#pragma comment(lib, "opencv_imgproc2410.lib")
//#pragma comment(lib, "opencv_legacy2410.lib")
//#pragma comment(lib, "opencv_ml2410.lib")
//#pragma comment(lib, "opencv_nonfree2410.lib")
//#pragma comment(lib, "opencv_objdetect2410.lib")
//#pragma comment(lib, "opencv_ocl2410.lib")
#pragma comment(lib, "opencv_stitching2410.lib")
//#pragma comment(lib, "opencv_video2410.lib")
#endif


// CStitchCamDlg �_�C�A���O
#define TIMER_STITCH	1
#define TIMER_PREVIEW	2

const float WIDTH = 640.0;
const float HEIGHT = 480.0;
const float FPS = 30.0;
const char STITCHED[] = "Stitched";

// �A�v���P�[�V�����̃o�[�W�������Ɏg���� CAboutDlg �_�C�A���O

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

// ����
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// COpenCVStitchDlg �_�C�A���O




COpenCVStitchDlg::COpenCVStitchDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(COpenCVStitchDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void COpenCVStitchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CAM1, m_cam1);
	DDX_Control(pDX, IDC_CAM2, m_cam2);
	DDX_Control(pDX, IDC_CAM3, m_cam3);
	DDX_Control(pDX, IDC_CAM4, m_cam4);
	DDX_Control(pDX, IDC_WIDE, m_qvga);
	DDX_Control(pDX, IDC_SAVE, m_save);
	DDX_Control(pDX, IDC_START, m_start);
	DDX_Control(pDX, IDC_STATUS, m_status);
}

BEGIN_MESSAGE_MAP(COpenCVStitchDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_PREVIEW, &COpenCVStitchDlg::OnBnClickedPreview)
	ON_BN_CLICKED(IDC_CARIBRATE, &COpenCVStitchDlg::OnBnClickedCaribrate)
	ON_BN_CLICKED(IDC_STOP, &COpenCVStitchDlg::OnBnClickedStop)
	ON_BN_CLICKED(IDC_START, &COpenCVStitchDlg::OnBnClickedStart)
	ON_BN_CLICKED(IDC_FILE, &COpenCVStitchDlg::OnBnClickedFile)
	ON_BN_CLICKED(IDC_CAM1, &COpenCVStitchDlg::OnBnClickedCam1)
	ON_BN_CLICKED(IDC_CAM2, &COpenCVStitchDlg::OnBnClickedCam2)
	ON_BN_CLICKED(IDC_CAM3, &COpenCVStitchDlg::OnBnClickedCam3)
	ON_BN_CLICKED(IDC_CAM4, &COpenCVStitchDlg::OnBnClickedCam4)
END_MESSAGE_MAP()

CWnd *pDlg;

// enum camera callback
static void callback(void *pItem, char *pDeviceName)
{
	char buffer[80] = "";
	size_t wlen;
	int *idcCam = (int*)pItem;
	if (*idcCam <= IDC_CAM4) {
		CButton *item = (CButton*)pDlg->GetDlgItem(*idcCam);

		item->EnableWindow();
		item->SetCheck(BST_CHECKED);
		item->ShowWindow(SW_SHOWNORMAL);
		strcpy(buffer, pDeviceName);
		// Buffalo�̃J�����̍s����CR,LF�������Ă���̂ŁA���������
		for (wlen = strlen(buffer) - 1; 0 < wlen; wlen--) {
			if (buffer[wlen] == 13 || buffer[wlen] == 10)
				buffer[wlen] = 0;
		}
		item->SetWindowText(buffer);
	}
	(*idcCam)++;
}

// COpenCVStitchDlg ���b�Z�[�W �n���h���[

BOOL COpenCVStitchDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// "�o�[�W�������..." ���j���[���V�X�e�� ���j���[�ɒǉ����܂��B

	// IDM_ABOUTBOX �́A�V�X�e�� �R�}���h�͈͓̔��ɂȂ���΂Ȃ�܂���B
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���̃_�C�A���O�̃A�C�R����ݒ肵�܂��B�A�v���P�[�V�����̃��C�� �E�B���h�E���_�C�A���O�łȂ��ꍇ�A
	//  Framework �́A���̐ݒ�������I�ɍs���܂��B
	SetIcon(m_hIcon, TRUE);			// �傫���A�C�R���̐ݒ�
	SetIcon(m_hIcon, FALSE);		// �������A�C�R���̐ݒ�

	// TODO: �������������ɒǉ����܂��B
	int count = 0;
	if ((count = gpu::getCudaEnabledDeviceCount()) > 0)
	{
		stitcher = StitchImage::createDefault(true);	// CUDA���g�p�ł���ꍇ
		CString buffer;
		buffer.Format("%d CUDA device detected.", count);
		m_status.SetWindowText(buffer);
	}
	else
	{
		stitcher = StitchImage::createDefault(false);	// CUDA���g�p�ł��Ȃ��ꍇ
		m_status.SetWindowText("No CUDA device detected.");
	}
	pDlg = this;
	int idcCam = IDC_CAM1;
	count = WebCam::EnumDevices((PENUMDEVICE)&callback, &idcCam);

	iValue = 0;

	fileOpened = false;

	return TRUE;  // �t�H�[�J�X���R���g���[���ɐݒ肵���ꍇ�������ATRUE ��Ԃ��܂��B
}

void COpenCVStitchDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �_�C�A���O�ɍŏ����{�^����ǉ�����ꍇ�A�A�C�R����`�悷�邽�߂�
//  ���̃R�[�h���K�v�ł��B�h�L�������g/�r���[ ���f�����g�� MFC �A�v���P�[�V�����̏ꍇ�A
//  ����́AFramework �ɂ���Ď����I�ɐݒ肳��܂��B

void COpenCVStitchDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �`��̃f�o�C�X �R���e�L�X�g

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// �N���C�A���g�̎l�p�`�̈���̒���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �A�C�R���̕`��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ���[�U�[���ŏ��������E�B���h�E���h���b�O���Ă���Ƃ��ɕ\������J�[�\�����擾���邽�߂ɁA
//  �V�X�e�������̊֐����Ăяo���܂��B
HCURSOR COpenCVStitchDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// ���ϒl���Z�o���ĕ\��
void COpenCVStitchDlg::UpdateValue(double v)
{
	CString buffer;
	double sum = 0;
	value[iValue] = v;
	iValue = (iValue + 1) % AVERAGE_SIZE;

	for (int i = 0; i < AVERAGE_SIZE; i++)
	{
		sum += value[i];
	}
	buffer.Format("%.2fmsec", sum / AVERAGE_SIZE);
	m_status.SetWindowText(buffer);
}


// �J�����J�n
void COpenCVStitchDlg::DoOpen()
{
	if (fileOpened)
	{
		return;
	}
	camera.clear();
	for (int id = IDC_CAM1; id <= IDC_CAM4; id++)
	{
		CButton *item = (CButton*)GetDlgItem(id);
		if (item->GetCheck() == BST_CHECKED)
		{
			char name[80];
			bool res;
			item->GetWindowText(name, 80);

			WebCam cam = WebCam();
			res = cam.open(id - IDC_CAM1);
			if (res) {
				CString buffer;
				if (m_qvga.GetCheck() == BST_CHECKED) {
					res = cam.set(CV_CAP_PROP_FRAME_WIDTH, 320.0F);
					res = cam.set(CV_CAP_PROP_FRAME_HEIGHT, 240.0F);
					buffer.Format("%d %s QVGA", id - IDC_CAM1 + 1, name);
				} else {
					res = cam.set(CV_CAP_PROP_FRAME_WIDTH, WIDTH);
					res = cam.set(CV_CAP_PROP_FRAME_HEIGHT, HEIGHT);
					buffer.Format("%d %s", id - IDC_CAM1 + 1, name);
				}
				res = cam.set(CV_CAP_PROP_FPS, FPS);
				res = cam.set(CV_CAP_PROP_FOURCC, CV_FOURCC('M','J','P','G'));

				cam.SetName((LPCSTR)buffer);
				camera.push_back(cam);
			}
		}
	}
}

void COpenCVStitchDlg::DoPreview()
{
	for (size_t i = 0; i < camera.size(); i++)
	{
		Mat image;
		camera[i] >> image;
		if (image.empty() == false) {
			imshow(camera[i].GetName(), image);
		}
	}
}

void COpenCVStitchDlg::DoStitch()
{
	Mat stitched;
	vector<Mat> images;

	for (size_t i = 0; i < camera.size(); i++)
	{
		Mat image;
		camera[i] >> image;

		imshow(camera[i].GetName(), image);
		images.push_back(image);
	}
	if (2 <= camera.size())
	{
		int64 t = getTickCount();
		if (StitchImage::Status::OK == stitcher.composePanorama(images, stitched)) {
			UpdateValue((getTickCount() - t) / getTickFrequency() * 1000);
			imshow(STITCHED, stitched);
			if (writer.isOpened())
			{
				writer << stitched;
			}
		} else {
			MessageBox("���������ł̃G���[", "�L�����u���[�g");
		}
		images.clear();
	}
	else
	{
		MessageBox("�J���������Ȃ����߃L�����u���[�g�ł��܂���");
	}	
}

void COpenCVStitchDlg::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
	case TIMER_STITCH:
		DoStitch();
		break;
	
	case TIMER_PREVIEW:
		DoPreview();
		break;
	}

	CDialogEx::OnTimer(nIDEvent);
}

void COpenCVStitchDlg::OnBnClickedPreview()
{
	DoOpen();
	pTimer = SetTimer(TIMER_PREVIEW, 30, NULL);	// 30msec
}


void COpenCVStitchDlg::OnBnClickedCaribrate()
{
	DoOpen();
	Mat stitched;
	vector<Mat> images;
	// 
	for (size_t i = 0; i < camera.size(); i++)
	{
		Mat image;
		camera[i] >> image;
		if (image.empty() == false) {
			imshow(camera[i].GetName(), image);
			images.push_back(image);
		}
	}
	if (2 <= camera.size())
	{
		int64 t = getTickCount();
		stitcher.estimateTransform(images);
		if (StitchImage::Status::OK == stitcher.composePanorama(images, stitched)) {
			CString buffer;
			buffer.Format("%.2fmsec", (getTickCount() - t) / getTickFrequency() * 1000);
			m_status.SetWindowText(buffer);
			imageSize = Size(stitched.cols, stitched.rows);
			imshow(STITCHED, stitched);
			CWnd *start = GetDlgItem(IDC_START);
			start->EnableWindow();
		} else {
			MessageBox("���������ł̃G���[", "�L�����u���[�g");
		}
	}
	else
	{
		MessageBox("�J���������Ȃ����߃L�����u���[�g�ł��܂���");
	}
}


void COpenCVStitchDlg::OnBnClickedStart()
{
	if (2 <= camera.size())
	{
		if (m_save.GetCheck() == BST_CHECKED)
		{
			static TCHAR BASED_CODE szFilter[] = _T("AVI (*.avi)|*.avi|All Files (*.*)|*.*||");

			CFileDialog dlg(FALSE, "avi", "save.avi", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
			if (dlg.DoModal() == IDOK)
			{
				int fourcc = CV_FOURCC('I','Y','U','V');	//CV_FOURCC('M','J','P','G');	// Motion JPEG

				CString filename = dlg.GetFileName();
				if (writer.open((LPCSTR)filename, fourcc, FPS / 2, imageSize) != true)
				{
					MessageBox("Cant save");
				}
			}
			else
			{
				return;
			}
		}
		pTimer = SetTimer(TIMER_STITCH, 30, NULL);	// 30msec
	}
	else
	{
		MessageBox("�J���������Ȃ����߃X�e�b�`�ł��܂���");
	}
}


void COpenCVStitchDlg::OnBnClickedStop()
{
	KillTimer(pTimer);
	destroyAllWindows();
	for (size_t i = 0; i < camera.size(); i++)
	{
	//	camera[i].release();
	}
	if (writer.isOpened())
	{
		writer.release();
	}
}


void COpenCVStitchDlg::OnBnClickedFile()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
}


void COpenCVStitchDlg::OnBnClickedCam1()
{
	m_start.EnableWindow(FALSE);
}


void COpenCVStitchDlg::OnBnClickedCam2()
{
	m_start.EnableWindow(FALSE);
}


void COpenCVStitchDlg::OnBnClickedCam3()
{
	m_start.EnableWindow(FALSE);
}


void COpenCVStitchDlg::OnBnClickedCam4()
{
	m_start.EnableWindow(FALSE);
}
