#pragma once

#include "IComponentVisitor.hpp"
#include "../components/Component.hpp"
#include <entt/entity/entity.hpp>
#include <imgui.h>
#include <imgui-SFML.h>

class EditorComponentVisitor : public IComponentVisitor
{
public:
	// void visit(const AnimatedSprite* animatedSprite);
	void visit(Effects* effects, entt::entity entityID) override;
	void visit(EffectsList* effectsList, entt::entity entityID) override;
	void visit(EntityStatus* entityStatus, entt::entity entityID) override;
	void visit(Hitbox* hitbox, entt::entity entityID) override;
	void visit(MovementPattern* movementPattern, entt::entity entityID) override;
	void visit(PlayerInput* playerInput, entt::entity entityID) override;
	void visit(SceneViewRenderer* sceneViewRenderer, entt::entity entityID) override;
	void visit(Sprite* sprite, entt::entity entityID) override;
	void visit(StatusModEvent* statusModEvent, entt::entity entityID) override;
	void visit(TeamTag* teamTag, entt::entity entityID) override;
	void visit(UpdateEntityEvent* updateEntityEvent, entt::entity entityID) override;
	void visit(UpdateEntityPolling* updateEntityPolling, entt::entity entityID) override;
};

