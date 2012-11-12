#ifndef _SETTING_DEFINE_H_
#define _SETTING_DEFINE_H_

typedef	enum{
	SQUARE,
	ARROW,
	HAND,
}CURSORSHAPE;

struct _cursorProp
{
	//now only surport single color cursor theme
	//TODO use alpha to get transparent effect
	int ThemeColor;
	CURSORSHAPE shape;
};
typedef struct _cursorProp cursorProp, *pCursorProp;
#define CURSORPROPLEN sizeof(struct _cursorProp)
#endif /* _SETTING_DEFINE_H_ */