#pragma once

class InputMgr
{
	DECLARE_SINGLETON(InputMgr)

private:
	explicit	InputMgr();
	virtual		~InputMgr();

public:
	virtual void Free(void);
};

