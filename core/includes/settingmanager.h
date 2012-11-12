#ifndef _SETTING_MANAGER_H_
#define _SETTING_MANAGER_H_

#include "gamelib.h"

#define BOOLSETTING 1
#define PROPERTY 2

struct _setting
{
	char* name;
	int8_t type;
	BOOL enabled;
	void* prop;
	_setting* next;
};
typedef struct _setting settingItem, *pSettingItem;

STATUS settingManager_init();
STATUS settingManager_setBool(char* name, BOOL value);
STATUS settingManager_setProperty(char *name, void* prop);
STATUS settingManager_getBool(char* name, BOOL* value);
STATUS settingManager_getProperty(char* name, void** prop);
STATUS settingManager_release();

#endif /* _SETTING_MANAGER_H_ */