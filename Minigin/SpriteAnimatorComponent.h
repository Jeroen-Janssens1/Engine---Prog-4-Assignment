#pragma once
#include "BaseComponent.h"

class Animation;
class Transition;
class State;
class SpriteAnimatorComponent final :
	public BaseComponent
{
public:
	SpriteAnimatorComponent(GameObject* pOwner, std::vector<Animation*>& animations, const std::string& tag = "");
	~SpriteAnimatorComponent();

	virtual void Update() override;
	virtual void Render() const override;
	void OnLoad() override {}

	void ResetAnimator();

	void SetTrigger(const std::string& name);
	void ResetTrigger(const std::string& name);

	void SetBool(const std::string& name, bool value);

	const std::string& GetCurrentStateName() const;

private:
	std::vector<Animation*> m_Animations;
	std::vector<Transition*> m_Transitions;
	State* m_ActiveAnimation;

};

