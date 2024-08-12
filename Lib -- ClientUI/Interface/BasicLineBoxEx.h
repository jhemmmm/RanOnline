//	라인 박스 8조각
//
//	최초 작성자 : 성기엽
//	이후 수정자 : 
//	로그
//		[2003.11.31]
//			@ 작성
//

#pragma	once

#include "BasicLineBox.h"

class	CBasicLineBoxEx : public CBasicLineBox
{
protected:
	enum
	{
		LEFTTOP = BOTTOM + 1,		
		LEFTBOTTOM,
		RIGHTTOP,
		RIGHTBOTTOM
	};

public:
	CBasicLineBoxEx ();
	virtual	~CBasicLineBoxEx ();

public:
	void	CreateBaseBoxStorageCard ( char* szBoxControl );
	void	CreateBaseBoxESCMenu ( char* szBoxControl );
	void	CreateBaseBoxVarTextBox ( char* szBoxControl );
	void	CreateBaseBoxVarTextBoxNew ( char* szBoxControl );
	void	CreateBaseBoxVarTextBoxGrayish ( char* szBoxControl );
	void	CreateBaseBoxLoiBox ( char* szBoxControl );
	void	CreateBaseBoxBangBox ( char* szBoxControl );
	void	CreateBaseBoxLomBox ( char* szBoxControl );
	void	CreateBaseBoxBlueBox ( char* szBoxControl );
	void	CreateBaseBoxDenXamBox ( char* szBoxControl );
	void	CreateBaseBoxMapBox ( char* szBoxControl );
	void	CreateBaseBoxNewBox ( char* szBoxControl );
	void	CreateBaseBoxTargetInfo ( char* szBoxControl );
	void	CreateBaseBoxName ( char* szBoxControl );
	void	CreateBaseBoxNameCD ( char* szBoxControl );
	void	CreateBaseBoxChat ( char* szBoxControl );
	void	CreateBaseBoxSimpleHP ( char* szBoxControl );	
	/*Arvin.BC*/
	void	CreateBaseBoxSkillGreen ( char* szBoxControl );
	void	CreateBaseBoxSkillBlue ( char* szBoxControl );
	void	CreateBaseBoxSkillYellow ( char* szBoxControl );
	void	CreateBaseBoxDialogueWhite ( char* szBoxControl );
	void	CreateBaseBoxDialogueLightGray ( char* szBoxControl );
	void	CreateBaseBoxDialogueDarkGray ( char* szBoxControl );
	void	CreateBaseBoxDialogueWhiteBack ( char* szBoxControl );
	void	CreateBaseBoxWhiteOuter ( char* szBoxControl );
	void	CreateBaseBoxDialogueBlackBack ( char* szBoxControl );
	void	CreateBaseBoxDialogueGrayBack ( char* szBoxControl );
	void	CreateBaseBoxDialogueNewBase ( char* szBoxControl );
	
	

protected:
	void	CreateLineImageEx ( char* szLeftTop, char* szLeftBottom, char* szRightTop, char* szRIghtBottom );

private:
	CUIControl*		m_pLeftTop;
	CUIControl*		m_pLeftBottom;
	CUIControl*		m_pRIghtTop;
	CUIControl*		m_pRightBottom;
};