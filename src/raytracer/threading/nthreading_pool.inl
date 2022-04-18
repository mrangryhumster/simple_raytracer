#include "nthreading_pool.hpp"

namespace nova
{
	namespace threading
	{
		template<typename _shared_data_type, typename _size_type>
		const _size_type thread_pool_t<_shared_data_type, _size_type>::hardware_concurrency = std::thread::hardware_concurrency();

		template<typename _shared_data_type, typename _size_type>
		thread_pool_t<_shared_data_type, _size_type>::thread_pool_t() :
			thread_pool_t(0)
		{
		}

		template<typename _shared_data_type, typename _size_type>
		thread_pool_t<_shared_data_type, _size_type>::thread_pool_t(_size_type threadCount) :
			m_threads_count(0)
		{
			allocate(threadCount);
		}

		template<typename _shared_data_type, typename _size_type>
		thread_pool_t<_shared_data_type, _size_type>::~thread_pool_t()
		{
			if (m_thread_pool != nullptr)
				destroy();
		}

		template<typename _shared_data_type, typename _size_type>
		void thread_pool_t<_shared_data_type, _size_type>::resize(_size_type newThreadCount)
		{
			if (m_task_count > 0)
				return;
			reallocate(newThreadCount);
		}

		template<typename _shared_data_type, typename _size_type>
		inline _size_type thread_pool_t<_shared_data_type, _size_type>::threads_count() const
		{
			return m_threads_count;
		}

		template<typename _shared_data_type, typename _size_type>
		_size_type thread_pool_t<_shared_data_type, _size_type>::tasks_count() const
		{
			return m_task_count;
		}

		template<typename _shared_data_type, typename _size_type>
		void thread_pool_t<_shared_data_type, _size_type>::clear_tasks()
		{
			m_task_count -= static_cast<_size_type>(m_task_list.size());
			m_task_list.clear();
		}


		template<typename _shared_data_type, typename _size_type>
		inline _shared_data_type thread_pool_t<_shared_data_type, _size_type>::shared(_size_type threadId)
		{
			return m_thread_info[threadId].thread_shared_data;
		}

		template<typename _shared_data_type, typename _size_type>
		void thread_pool_t<_shared_data_type, _size_type>::process()
		{
			_size_type threads_running = m_threads_count;
			for (size_t idx = 0; idx < m_threads_count; idx++)
			{
				thread_info& info = m_thread_info[idx];
				if (info.thread_finished)
				{
					if (m_thread_pool[idx].joinable())
						m_thread_pool[idx].join();
					info.thread_finished.store(false);
					info.thread_vacant = true;
				}
				if (info.thread_vacant)
				{
					if (!m_task_list.empty())
					{
						thread_task& thread_task = m_task_list.front();
						info.thread_vacant = false;
						info.thread_shared_data = thread_task.shared;
						m_thread_pool[idx] = std::thread(__thread_proc, std::ref(m_thread_info[idx]), thread_task.function);
						m_task_list.pop_front();
					}
					else
					{
						threads_running--;
					}
				}
			}
			m_task_count = static_cast<_size_type>(m_task_list.size()) + threads_running;
		}

		template<typename _shared_data_type, typename _size_type>
		template<class _function, class ..._args>
		inline void thread_pool_t<_shared_data_type, _size_type>::push_task(_shared_data_type shared, _function&& task, _args && ...args)
		{
			m_task_list.push_back(thread_task(shared, std::bind(std::forward<_function>(task), std::forward<_args>(args)...)));
			m_task_count++;
		}

		template<typename _shared_data_type, typename _size_type>
		void thread_pool_t<_shared_data_type, _size_type>::reallocate(_size_type threadCount)
		{
			if (m_thread_pool != nullptr)
				destroy();
			allocate(threadCount);
		}

		template<typename _shared_data_type, typename _size_type>
		void thread_pool_t<_shared_data_type, _size_type>::allocate(_size_type threadCount)
		{
			m_threads_count = threadCount;
			m_thread_pool = new std::thread[m_threads_count];
			m_thread_info = new thread_info[m_threads_count];

			for (_size_type idx = 0; idx < m_threads_count; idx++)
			{
				m_thread_info[idx].thread_finished = false;
				m_thread_info[idx].thread_vacant = true;
			}
		}

		template<typename _shared_data_type, typename _size_type>
		void thread_pool_t<_shared_data_type, _size_type>::destroy()
		{
			delete[] m_thread_pool;
			delete[] m_thread_info;

			m_threads_count = 0;
			m_thread_pool = nullptr;
			m_thread_info = nullptr;
		}

		template<typename _shared_data_type, typename _size_type>
		void thread_pool_t<_shared_data_type, _size_type>::__thread_proc(thread_info& info, std::function<void()> function)
		{
			function();
			info.thread_finished.store(true);
		}
	}
}