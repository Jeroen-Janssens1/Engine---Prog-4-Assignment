#pragma once
#include "SubjectComponent.h"
class InputSubject final :
	public SubjectComponent
{
public:
	InputSubject(GameObject* pOwner);
	virtual void Update() override;
	virtual void Render() const override {};

private:
	

};

