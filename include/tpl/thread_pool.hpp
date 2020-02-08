#ifndef TPL_THREAD_POOL_H_
#define TPL_THREAD_POOL_H_

#include <memory>
#include <vector>
#include <cstdint>
#include <stdexcept>
#include <thread>

namespace tpl
{
    class threadpool_base
    {
    public:
        static threadpool_base& shared_instance();
        static std::unique_ptr<threadpool_base> construct(size_t num_threads)
        {
            const auto result = initialize_shared_threadpool(num_threads);
            if (!result.first)
            {
                throw std::runtime_error("the cpprestsdk threadpool has already been initialized");
            }
        }

        virtual ~threadpool_base() = default;

        static void initialize_with_threads(size_t num_threads);

        template<typename T>
        void schedule(T task)
        {
            service().post(task);
        }

    protected:
        threadpool_base(size_t num_threads) : m_service(static_cast<int>(num_threads)) {}

        asio::io_service m_service;
    };

    struct threadpool final : public threadpool_base
    {
        threadpool(size_t n) : threadpool_base(n), m_work(m_service)
        {
            for (size_t i = 0; i < n; i++)
                add_thread();
        }

        threadpool(const threadpool&) = delete;
        threadpool& operator=(const threadpool&) = delete;

        ~threadpool()
        {
            m_service.stop();
            for (auto iter = m_threads.begin(); iter != m_threads.end(); ++iter)
            {
                (*iter)->join();
            }
        }

        threadpool& get_shared() { return *this; }

    private:
        void add_thread()
        {
            m_threads.push_back(
                std::unique_ptr<std::thread>(new std::thread([&] { thread_start(this); })));
        }

        static void* thread_start(void* arg)
        {
            threadpool* that = reinterpret_cast<threadpool*>(arg);
            that->m_service.run();
            return arg;
        }

        std::vector<std::unique_ptr<std::thread>> m_threads;
        asio::io_service::work m_work;
    };
}

#endif