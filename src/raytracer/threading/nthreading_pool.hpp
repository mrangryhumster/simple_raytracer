#pragma once

#include <thread>
#include <functional>
#include <atomic>
#include <list>

namespace nova
{
	namespace threading
	{	
		
		// thread_pool_t implements the thread pool things.
		// - _desc_type is description class contained within each task and can be accessed from thread
		// - _size_type defines which type will be used to size/count/resize etc methods, use size_t if you are not sure
		template<typename _shared_data_type, typename _size_type>
		class thread_pool_t 
		{
			static_assert(std::is_default_constructible<_shared_data_type>::value, "thread_pool_t : passed _shared_data_type is not default constructable");
		public:
			//--------------------------------
			static const _size_type hardware_concurrency;
			//--------------------------------
			thread_pool_t();
			thread_pool_t(_size_type ThreadsCount);
			~thread_pool_t();
			//--------------------------------
			void     resize(_size_type newThreadCount);
			_size_type threads_count() const;
			_size_type tasks_count() const;
			void clear_tasks();
			//--------------------------------
			_shared_data_type shared(_size_type threadId);
			//--------------------------------
			void process();
			//--------------------------------
			template<class _function, class ..._args>
			void push_task(_shared_data_type desc, _function&& task,_args&& ... args);
			//--------------------------------
		private:

			//--------------------------------
			struct thread_info
			{
				thread_info() :
					thread_shared_data(_shared_data_type()),
					thread_vacant(true)
				{};
				thread_info(_shared_data_type _d) :
					thread_shared_data(_d),
					thread_vacant(true)
				{};
				_shared_data_type thread_shared_data;
			private: //Internal access only
				std::atomic<bool> thread_finished;
				bool thread_vacant;
				friend thread_pool_t;
			};
			//--------------------------------
			struct thread_task
			{
				thread_task(_shared_data_type sh,std::function<void()> fn) :
					shared(sh),
					function(fn) {};
				_shared_data_type shared;
				std::function<void()> function;
			};
			//--------------------------------
			static void __thread_proc(thread_info& info, std::function<void()> function);
			//--------------------------------
			void reallocate(_size_type threadCount);
			void allocate(_size_type threadCount);
			void destroy();
			//--------------------------------
			using TaskList = std::list<thread_task>;
			//--------------------------------
			_size_type	 m_threads_count;
			std::thread* m_thread_pool;
			thread_info* m_thread_info;
			TaskList	 m_task_list;
			_size_type     m_task_count;
			//--------------------------------
		};
	}
}