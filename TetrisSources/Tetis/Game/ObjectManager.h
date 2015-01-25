#ifndef __OBJECTMANAGER_H__
#define __OBJECTMANAGER_H__

class GameObject;
class IRenderer;

class ObjectManager
	{
	private:
		std::vector<std::unique_ptr<GameObject>> m_objects;
		
		std::vector<GameObject*> m_dead_pool;
		std::map<std::size_t, std::vector<GameObject*>> m_grouped_objects;

		int m_next_id;

	public:
		ObjectManager();
		~ObjectManager();

		void Update(float i_elapsed_time);

		template <typename T>
		GameObject& Create()
			{
			static_assert(std::is_base_of<GameObject, T>::value,
				"`T` must be derived from `GameObject`");

			auto p_unique(std::unique_ptr<T>(new T(m_next_id++)));
			auto p_ptr = p_unique.get();

			m_objects.emplace_back(std::move(p_unique));
			m_grouped_objects[typeid(T).hash_code()].emplace_back(p_ptr);

			return *p_ptr;
			}

		template <typename T, typename Arg0>
		GameObject& Create(const Arg0& i_arg)
			{
			static_assert(std::is_base_of<GameObject, T>::value,
				"`T` must be derived from `GameObject`");

			auto p_unique(std::unique_ptr<T>(new T(m_next_id++, i_arg)));
			auto p_ptr = p_unique.get();

			m_objects.emplace_back(std::move(p_unique));
			m_grouped_objects[typeid(T).hash_code()].emplace_back(p_ptr);

			return *p_ptr;
			}

		template <typename T, typename Arg0, typename Arg1>
		GameObject& Create(const Arg0& i_arg, const Arg1& i_arg1)
			{
			static_assert(std::is_base_of<GameObject, T>::value,
				"`T` must be derived from `GameObject`");

			auto p_unique(std::unique_ptr<T>(new T(m_next_id++, i_arg, i_arg1)));
			auto p_ptr = p_unique.get();

			m_objects.emplace_back(std::move(p_unique));
			m_grouped_objects[typeid(T).hash_code()].emplace_back(p_ptr);

			return *p_ptr;
			}

		void Draw(IRenderer& i_renderer);

		void Refresh();

		void clear()
			{
			m_grouped_objects.clear();
			m_objects.clear();
			}

		template<typename T> 
		std::vector<T*>& GetAll()
			{
			return m_grouped_objects[typeid(T).hash_code()];
			}

		template<typename T, typename TFunc>
		void forEach(const TFunc& i_function)
			{
			auto& objects = GetAll<T>();
			for (auto p_object : objects)
				i_function(*reinterpret_cast<T*>(p_object));
			}
	};

#endif