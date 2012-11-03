#ifndef _INPUT_DEVICE_PI_H_
#define _INPUT_DEVICE_PI_H_

#include <stdint.h>
enum _vd_key
{
	VD_A,
	VD_B,
	VD_C,
	VD_D,
	VD_E,
	VD_F,
	VD_G,
	VD_H,
	VD_I,
	VD_J,
	VD_K,
	VD_L,
	VD_M,
	VD_N,
	VD_O,
	VD_P,
	VD_Q,
	VD_R,
	VD_S,
	VD_T,
	VD_U,
	VD_V,
	VD_W,
	VD_X,
	VD_Y,
	VD_Z,
	VD_F1,
	VD_F2,
	VD_F3,
	VD_F4,
	VD_F5,
	VD_F6,
	VD_F7,
	VD_F8,
	VD_F9,
	VD_F10,
	VD_F11,
	VD_F12,
	VD_LCONTROL,
	VD_LMETA,
	VD_LSHIFT,
	VD_LWIN,
	VD_SPACE,
	VD_RMETA,
	VD_RWIN,
	VD_RCONTROL,
	VD_RSHIFT,
	VD_RETURN,
	VD_LBRACKET,
	VD_RBRACKET,
	VD_COMMA,
	VD_DOT,
	VD_SLASH,
	VD_SEMICOLON,
	VD_UP,
	VD_DOWN,
	VD_LEFT,
	VD_RIGHT,
	VD_HOME,
	VD_END,
	VD_PAGEUP,
	VD_PAGEDOWN,
	VD_INSERT,
	VD_DELETE,
	VD_BACKSPACE,
	VD_ESCAPE,
	VD_UNDERLINE,
	VD_EQUAL,
	VD_BACKSLASH,
	//TODO: numpad
};
typedef enum _vd_key VD_KEY;

enum _vd_mouse
{
	VD_MOUSE0,
	VD_MOUSE1,
	VD_MOUSE2,
	VD_MOUSE3,
	VD_MOUSE4,
	VD_MOUSE5,
	VD_MOUSE6,
	VD_MOUSE7,
};
typedef enum _vd_mouse VD_MOUSE;

struct _input_device
{
	void (*getInputState) (void);
	int32_t (*keyDown) (VD_KEY);
	int32_t (*buttonDown) (VD_MOUSE);
	int32_t (*mouse_X) (void);
	int32_t (*mouse_Y) (void);
};
typedef struct _input_device inputDevice, *pInputDevice;

#endif /* _INPUT_DEVICE_PI_H_ */