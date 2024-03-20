#include "EventSystem.hpp"

EventSystem::EventSystem()
{}

constexpr std::string_view EventSystem::name()
{
	return "EventSystem";
}

void EventSystem::update(entt::registry& reg, const float& dt, entt::entity ent)
{
	auto view = reg.group<CollisionEvent>();
	for (auto event : view)
	{
		entt::entity source = reg.get<CollisionEvent>(event).sourceID;
		entt::entity receiver = reg.get<CollisionEvent>(event).receiverID;


	}
}

void EventSystem::resolveEffects()
{

}
