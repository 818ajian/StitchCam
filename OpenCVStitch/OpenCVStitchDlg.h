
// OpenCVStitchDlg.h : �w�b�_�[ �t�@�C��
//

#pragma once
#include "afxwin.h"

#include <opencv2/core/core.hpp>
#include <opencv2/stitching/stitcher.hpp>
#include <cuda_runtime_api.h>

#include "..\WebCam.h"
#include "StitchImage.h"

using namespace cv;


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

	StitchImage stitcher;	// Customised cv::Stitcher
	vector<WebCam> camera;
	VideoWriter writer;
	Size imageSize;
	UINT_PTR pTimer;
	bool fileOpened;

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
	afx_msg void OnBnClickedPreview();
	afx_msg void OnBnClickedCaribrate();
	afx_msg void OnBnClickedStop();
	afx_msg void OnBnClickedStart();
	afx_msg void OnBnClickedFile();
};
