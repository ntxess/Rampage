#pragma once

#include "IComponentVisitor.hpp"

class EditorSpriteVisitor : public IComponentVisitor
{
public:
	// void visit(const AnimatedSprite* animatedSprite);
	void visit(const Effects* effects);
	void visit(const EffectsList* effectsList);
	void visit(const EntityStatus* entityStatus);
	void visit(const Hitbox* hitbox);
	void visit(const MovementPattern* movementPattern);
	void visit(const PlayerInput* playerInput);
	void visit(const SceneViewRenderer* sceneViewRenderer);
	void visit(const Sprite* sprite);
	void visit(const StatusModEvent* statusModEvent);
	void visit(const TeamTag* teamTag);
	void visit(const UpdateEntityEvent* updateEntityEvent);
	void visit(const UpdateEntityPolling* updateEntityPolling);
};

