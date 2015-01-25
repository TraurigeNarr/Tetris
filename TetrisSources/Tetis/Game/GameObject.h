#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

class IRenderer;

class GameObject
	{
	private:
		int m_id;
		bool m_destroy;

	public:
		GameObject(int i_id) : m_id(i_id), m_destroy(false) {}
		virtual ~GameObject(){}

		void Destroy() { m_destroy = false; }
		bool IsDestroyed() const { return m_destroy; }
		int GetId() const { return m_id; }

		virtual void Update(float i_elapsed_time) = 0;
		virtual void Draw(IRenderer& i_renderer) = 0;
	};

#endif