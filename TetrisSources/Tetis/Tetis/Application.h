#ifndef __TETRISAPPLICATION_H__
#define __TETRISAPPLICATION_H__

#include <GameCore/CoreDelegate.h>
#include "Game/GameManager.h"
#include "Game/ISolver.h"
#include "Game/IRandomizer.h"

namespace TetrisGame
{

	class ApplicationDelegateImpl : public SDK::CoreDelegate
	{
	private:
		std::unique_ptr<GameManager> mp_manager;
	public:
		virtual ~ApplicationDelegateImpl() {}

		virtual void OnCreate() override;
		virtual void OnTerminate() override;

		virtual void OnCollapse() override
		{
		}
		virtual void OnActivate() override
		{
		}

		virtual void Update(float i_elapsed_time) override;
		virtual void Draw() override;

	};

} // TetrisGame

#endif