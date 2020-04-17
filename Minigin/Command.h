#pragma once
#include <iostream>
#include "ServiceLocator.h"
class Command
{
protected:
	void (*Function)();

public:
	virtual ~Command() = default;
	virtual bool Execute() = 0;
	virtual void SetFunction(void (*function)())
	{
		Function = function;
	}
};

class NullCommand : public Command
{
public:
	bool Execute() override { return true; }
};

class QuitCommand : public Command
{
public:
	bool Execute() override { return false; }
};
