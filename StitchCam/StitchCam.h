
// StitchCam.h : PROJECT_NAME �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C���ł��B
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH �ɑ΂��Ă��̃t�@�C�����C���N���[�h����O�� 'stdafx.h' ���C���N���[�h���Ă�������"
#endif

#include "resource.h"		// ���C�� �V���{��


// CStitchCamApp:
// ���̃N���X�̎����ɂ��ẮAStitchCam.cpp ���Q�Ƃ��Ă��������B
//

class CStitchCamApp : public CWinApp
{
public:
	CStitchCamApp();

// �I�[�o�[���C�h
public:
	virtual BOOL InitInstance();

// ����

	DECLARE_MESSAGE_MAP()
};

extern CStitchCamApp theApp;