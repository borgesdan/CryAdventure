#pragma once

#include <CrySchematyc/Reflection/TypeDesc.h>
#include <DefaultComponents/Cameras/CameraComponent.h>
#include <DefaultComponents/Audio/ListenerComponent.h>

struct ComponentHelper {
	/// <summary>
	/// Atualiza uma câmera em terceira pessoa.
	/// </summary>
	/// <param name="lookOrientation">Valor da orientação da visão.</param>
	/// <param name="viewOffsetForward">Offset the player along the forward axis (normally back).</param>
	/// <param name="viewOffsetUp">Offset the player along the upwards</param>
	/// <param name="cameraComponent">O componente de câmera a ser atualizado.</param>
	/// <param name="listenerComponent">O componente de áudio a ser atualizado.</param>
	/// <returns>Retorna a transformação local</returns>
	static Matrix34 UpdateThirdPersonCamera(Quat& lookOrientation,
		float viewOffsetForward = -1.5f, float viewOffsetUp = 2.f,
		Cry::DefaultComponents::CCameraComponent *cameraComponent = nullptr, 
		Cry::Audio::DefaultComponents::CListenerComponent *listenerComponent = nullptr);
};
