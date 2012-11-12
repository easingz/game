#include "includes/settingmanager.h"

static pSettingItem settings = NULL;
static pSettingItem getSettingItem(char* name)
{
	pSettingItem p = settings;
	for (;p != NULL; p = p->next)
	{
		if(!strcmp(name, p->name))
			return p;
	}
	return NULL;
}

STATUS settingManager_init()
{
	return OK;
}

STATUS settingManager_setBool(char* name, BOOL value)
{
	pSettingItem p = getSettingItem(name);
	if (p)
	{
		p->enabled = value;
		p->type |= BOOLSETTING;
	}
	else
	{
		p = settings;
		while(p != NULL && p++ ->next != NULL);
		pSettingItem newSetting = (pSettingItem)malloc(sizeof(settingItem));
		int l = strlen(name);
		char* newName = (char*)malloc(l+1);
		memset(newName, 0, l+1);
		strncpy(newName, name, l);
		newName[l] = '\0';
		newSetting->name = newName;
		newSetting->enabled = value;
		newSetting->prop = NULL;
		newSetting->type = BOOLSETTING;
		if(p)
			p->next = newSetting;
		else
			settings = newSetting;
		newSetting->next = NULL;
	}
	return OK;
}

STATUS settingManager_setProperty(char *name, void* prop)
{
	pSettingItem p = getSettingItem(name);
	if (p)
	{
		p->prop = prop;
		p->type |= PROPERTY;
	}
	else
	{
		p = settings;
		while(p != NULL && p++ ->next != NULL);
		pSettingItem newSetting = (pSettingItem)malloc(sizeof(settingItem));
		int l = strlen(name);
		char* newName = (char*)malloc(l+1);
		memset(newName, 0, l+1);
		strncpy(newName, name, l);
		newName[l] = '\0';
		newSetting->name = newName;
		newSetting->enabled = false;
		newSetting->prop = prop;
		newSetting->type = PROPERTY;
		if(p)
			p->next = newSetting;
		else
			settings = newSetting;
		newSetting->next = NULL;
	}
	return OK;
}

STATUS settingManager_getBool(char* name, BOOL* value)
{
	pSettingItem p = getSettingItem(name);
	if(p)
	{
		*value = p->enabled;
		return OK;
	}
	else
	{
		return ERROR;
	}
}

STATUS settingManager_getProperty(char* name, void** prop)
{
	pSettingItem p = getSettingItem(name);
	if(p)
	{
		*prop = p->prop;
		return OK;
	}
	else
	{
		return ERROR;
	}
}
STATUS settingManager_release()
{
	pSettingItem p = settings, pre = settings;
	while(p != NULL)
	{
		free(pre->name);
		free(pre->prop);
		free(pre);
		pre = p;
		p = p->next;
	}
	settings = NULL;
	return OK;
}