#include "StdAfx.h"

#include "MainPlayer.h"
#include "GamePlugin.h"
#include "Helpers/ComponentHelper.h"

#include <CryCore/StaticInstanceList.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CryNetwork/Rmi.h>

using Cry::DefaultComponents::CCharacterControllerComponent;
using Cry::DefaultComponents::CAdvancedAnimationComponent;
using Cry::Entity::EEvent;

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
	m_pCharacterController = m_pEntity->GetOrCreateComponent<CCharacterControllerComponent>();
	m_pCharacterController->SetTransformMatrix(Matrix34::Create(Vec3(1.f), IDENTITY, Vec3(0, 0, 1.f)));

	m_pAnimationComponent = m_pEntity->GetOrCreateComponent<CAdvancedAnimationComponent>();
	m_pAnimationComponent->SetMannequinAnimationDatabaseFile("Animations/Mannequin/ADB/Test/FirstPerson.adb");
	m_pAnimationComponent->SetCharacterFile("Objects/Characters/TestChar/testchar.cdf");
	m_pAnimationComponent->SetControllerDefinitionFile("Animations/Mannequin/ADB/Test/ControllerDefinition.xml");
	m_pAnimationComponent->SetDefaultScopeContextName("ThirdPersonCharacter");
	m_pAnimationComponent->SetDefaultFragmentName("Idle");
	m_pAnimationComponent->SetAnimationDrivenMotion(true);
	m_pAnimationComponent->LoadFromDisk();

	m_idleFragmentId = m_pAnimationComponent->GetFragmentId("Idle");

	m_pEntity->GetNetEntity()->BindToNetwork();
}

void CMainPlayerComponent::InitializeLocalPlayer() {
	m_pCameraComponent = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CCameraComponent>();
	m_pAudioListenerComponent = m_pEntity->GetOrCreateComponent<Cry::Audio::DefaultComponents::CListenerComponent>();

	m_pInputComponent = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CInputComponent>();
}

void CMainPlayerComponent::UpdateAnimation(float frameTime) {
	auto desiredFragmendId = m_idleFragmentId;

	if (desiredFragmendId != m_activeFragmendId) {
		m_activeFragmendId = desiredFragmendId;
		m_pAnimationComponent->QueueFragmentWithId(m_activeFragmendId);
	}
}

void CMainPlayerComponent::UpdateCamera(float frameTime) {
	Quat lookOrientation;
	Ang3 _ypr = CCamera::CreateAnglesYPR(Matrix33(lookOrientation));
	_ypr.y = 0;
	_ypr.z = 0;
	const Quat correctedOrientation = Quat(CCamera::CreateOrientationYPR(_ypr));

	Helper::Camera::UpdateThirdPerson(lookOrientation, -1.5f, 2.0f, m_pCameraComponent, m_pAudioListenerComponent);
}

Cry::Entity::EventFlags CMainPlayerComponent::GetEventMask() const
{
	return
		Cry::Entity::EEvent::BecomeLocalPlayer |
		Cry::Entity::EEvent::Update |
		Cry::Entity::EEvent::Reset;
}

void CMainPlayerComponent::ProcessEvent(const SEntityEvent& event)
{
	switch (event.event)
	{
	case EEvent::BecomeLocalPlayer: {
		InitializeLocalPlayer();

		m_isAlive = true;
		m_pAnimationComponent->ResetCharacter();
		m_pCharacterController->Physicalize();

		break;
	}
	case EEvent::Update: {
		if (!m_isAlive)
			return;

		const auto frameTime = event.fParam[0];

		UpdateAnimation(frameTime);
		UpdateCamera(frameTime);
		break;
	}
	case EEvent::Reset: {
		m_isAlive = event.nParam[0] != 0;
		break;
	}
	}
}


