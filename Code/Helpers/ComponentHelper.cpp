#include "StdAfx.h"
#include "ComponentHelper.h"

Matrix34 ComponentHelper::UpdateThirdPersonCamera(Quat& lookOrientation,
	float viewOffsetForward, float viewOffsetUp,
	Cry::DefaultComponents::CCameraComponent* cameraComponent,
	Cry::Audio::DefaultComponents::CListenerComponent* listenerComponent)
{
	Ang3 ypr = CCamera::CreateAnglesYPR(Matrix33(lookOrientation));

	// Ignore z-axis rotation, that's set by CPlayerAnimations
	ypr.x = 0;

	// Start with changing view rotation to the requested mouse look orientation
	Matrix34 localTransform = IDENTITY;
	localTransform.SetRotation33(CCamera::CreateOrientationYPR(ypr));

	// Offset the player along the forward axis (normally back)
	// Also offset upwards	
	localTransform.SetTranslation(Vec3(0, viewOffsetForward, viewOffsetUp));


	if(cameraComponent)
		cameraComponent->SetTransformMatrix(localTransform);

	if(listenerComponent)
		listenerComponent->SetOffset(localTransform.GetTranslation());

	return localTransform;
}
