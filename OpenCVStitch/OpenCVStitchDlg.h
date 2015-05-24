
// OpenCVStitchDlg.h : �w�b�_�[ �t�@�C��
//

#pragma once
#include "afxwin.h"

#include <opencv2/core/core.hpp>
#include <opencv2/stitching/stitcher.hpp>
#include <cuda_runtime_api.h>

#include "../WebCam.h"
#include "../MyStitcher.h"

using namespace cv;

#define AVERAGE_SIZE	10

// COpenCVStitchDlg �_�C�A���O
class COpenCVStitchDlg : public CDialogEx
{
// �R���X�g���N�V����
public:
	COpenCVStitchDlg(CWnd* pParent = NULL);	// �W���R���X�g���N�^�[

// �_�C�A���O �f�[�^
	enum { IDD = IDD_OPENCVSTITCH_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �T�|�[�g


// ����
protected:
	HICON m_hIcon;

	//StitchImage stitcher;	// Customised cv::Stitcher
	MyStitcher stitcher;

	vector<WebCam> camera;
	VideoWriter writer;
	Size imageSize;
	UINT_PTR pTimer;
	bool fileOpened;
	double value[AVERAGE_SIZE];
	int iValue;

	void UpdateValue(double value);

	void DoOpen();
	void DoStitch();
	void DoPreview();

	// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CButton m_cam1;
	CButton m_cam2;
	CButton m_cam3;
	CButton m_cam4;
	CButton m_qvga;
	CButton m_save;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedPreview();
	afx_msg void OnBnClickedCaribrate();
	afx_msg void OnBnClickedStop();
	afx_msg void OnBnClickedStart();
	afx_msg void OnBnClickedFile();
	afx_msg void OnBnClickedCam1();
	afx_msg void OnBnClickedCam2();
	afx_msg void OnBnClickedCam3();
	afx_msg void OnBnClickedCam4();
	CButton m_start;
	CStatic m_status;
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio3();
	afx_msg void OnBnClickedRadio4();
	CButton m_NoSeamFinder;
	CStatic m_gpu;
};
