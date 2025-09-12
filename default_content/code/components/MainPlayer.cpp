#include "StdAfx.h"

#include "MainPlayer.h"

#include <CryCore/StaticInstanceList.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>

namespace
{
	static void RegisterMainPlayerComponent(Schematyc::IEnvRegistrar& registrar)
	{
		Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CMainPlayerComponent));
		}
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterMainPlayerComponent);
}

void CMainPlayerComponent::Initialize()
{
}

Cry::Entity::EventFlags CMainPlayerComponent::GetEventMask() const
{
	return Cry::Entity::EventFlags();
}

void CMainPlayerComponent::ProcessEvent(const SEntityEvent& event)
{
}
