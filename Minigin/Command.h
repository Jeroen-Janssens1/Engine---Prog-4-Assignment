#pragma once
#include <iostream>
#include "ServiceLocator.h"
class Command
{
public:
	virtual ~Command() = default;
	virtual bool Execute() = 0;
};

// empty command
class NullCommand : public Command
{
public:
	bool Execute() override { return true; }
};

// empty command that returns false
class QuitCommand : public Command
{
public:
	bool Execute() override { return false; }
};
