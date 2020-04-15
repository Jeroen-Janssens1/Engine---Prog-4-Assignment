#pragma once
#include <iostream>
#include "ServiceLocator.h"
class Command
{
public:
	virtual ~Command() = default;
	virtual bool Execute() = 0;
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
