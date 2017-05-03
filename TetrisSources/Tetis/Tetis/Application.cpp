#include "stdafx.h"

#include "Application.h"

#include "Game/GameManager.h"
#include "Game/IRandomizer.h"
#include "Game/ISolver.h"

#include "SolverLib/SolverImplementation.h"

#include <Math/VectorConstructor.h>

#include <GameCore/Core.h>
#include <GameCore/Render/IRenderer.h>
namespace TetrisGame
{

	void ApplicationDelegateImpl::OnCreate()
	{
		mp_manager.reset(new GameManager(SDK::Core::GetRenderer()->GetTargetRectangle()));
		mp_manager->Initialize();
		mp_manager->SetSolver(std::unique_ptr<ISolver>(new SolverImplementation()));
	}
	void ApplicationDelegateImpl::OnTerminate()
	{

	}

	void ApplicationDelegateImpl::Update(float dt)
	{
		mp_manager->Update(dt);
	}

	void ApplicationDelegateImpl::Draw()
	{
		mp_manager->Draw(*SDK::Core::GetRenderer());
	}

} // TetrisGame