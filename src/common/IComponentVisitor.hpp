#pragma once

class Effects;
class EffectsList;
class EntityStatus;
class Hitbox;
class MovementPattern;
class PlayerInput;
class SceneViewRenderer;
class Sprite;
class StatusModEvent;
class TeamTag;
class UpdateEntityEvent;
class UpdateEntityPolling;

class IComponentVisitor
{
public:
	virtual ~IComponentVisitor() = default;
	// virtual void visit(const AnimatedSprite* animatedSprite) = 0;
	virtual void visit(const Effects* effects) = 0;
	virtual void visit(const EffectsList* effectsList) = 0;
	virtual void visit(const EntityStatus* entityStatus) = 0;
	virtual void visit(const Hitbox* hitbox) = 0;
	virtual void visit(const MovementPattern* movementPattern) = 0;
	virtual void visit(const PlayerInput* playerInput) = 0;
	virtual void visit(const SceneViewRenderer* sceneViewRenderer) = 0;
	virtual void visit(const Sprite* sprite) = 0;
	virtual void visit(const StatusModEvent* statusModEvent) = 0;
	virtual void visit(const TeamTag* teamTag) = 0;
	virtual void visit(const UpdateEntityEvent* updateEntityEvent) = 0;
	virtual void visit(const UpdateEntityPolling* updateEntityPolling) = 0;
};