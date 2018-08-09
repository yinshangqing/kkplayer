/*
创建人：袁晓东
日期：2014/02/28;
功能：滚动条
*/
#ifndef	JM_SELFDEFINEDSCROLLBAR_H
#define JM_SELFDEFINEDSCROLLBAR_H

#include "qscrollbar.h"
#include "Common.h"

class SelfdefinedScrollbar :public QScrollBar
{
	Q_OBJECT

public:
	SelfdefinedScrollbar(int Flags);
	~SelfdefinedScrollbar(void);

private:
	void SetHorizontalScrollBarStyle();
	void SetVerticalScrollBarStyle();
};

#endif
