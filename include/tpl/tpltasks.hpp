#ifndef TPL_TASKS_H_
#define TPL_TASKS_H_

#include <stdexcept>
#include <memory>
#include <type_traits>

namespace tpl
{
    enum task_status
    {
        /// <summary>
        ///     The tasks queued to the <c>task_group</c> object have not completed.  Note that this value is not presently returned by
        ///     the Concurrency Runtime.
        /// </summary>
        not_complete,

        /// <summary>
        ///     The tasks queued to the <c>task_group</c> or <c>structured_task_group</c> object completed successfully.
        /// </summary>
        completed,

        /// <summary>
        ///     The <c>task_group</c> or <c>structured_task_group</c> object was canceled.  One or more tasks may not have executed.
        /// </summary>
        canceled
    };

    class cancellation_token
    {
    public:

    };

    struct task_impl_base
    {
        enum TaskInternalState
        {
            // Tracks the state of the task, rather than the task collection on which the task is scheduled
            Created,
            Started,
            PendingCancel,
            Completed,
            Canceled
        };

        task_impl_base()
        {
        }

        virtual ~task_impl_base()
        {
        }

        task_status wait()
        {
            
        }

    };

    template<typename ReturnType>
    struct task_impl : public task_impl_base
    {

    };

    template<typename ReturnType>
    struct task_ptr
    {
        typedef std::shared_ptr<task_impl<ReturnType>> PtrType;
        /*
        static PtrType make(CancellationTokenState * ct, scheduler_ptr scheduler_arg)
        {
            return std::make_shared<task_impl<ReturnType>>(ct, scheduler_arg);
        }
        */
    };

    template<typename ReturnType>
    class task
    {
    public:
        task() : impl_(nullptr)
        {
        }

        task(const task& other) : impl_(other.impl_) {}

        task(task&& other): impl_(std::move(other.impl_)) {}

        task& operator=(const task& other)
        {
            if (this != &other)
            {
                impl_ = other.impl_;
            }
            return *this;
        }

        task& operator=(task&& other)
        {
            if (this != &other)
            {
                impl_ = std::move(other.impl_);
            }
            return *this;
        }

        template<typename FunctionType>
        task(FunctionType fn)
        {

        }

        task_status wait()
        {
            if (impl_)
            {
                return impl_.wait();
            }
            else
            {
                throw std::runtime_error("Task implement no found");
            }
        }

        ReturnType get()
        {
            return ReturnType{};
        }

        typename task_ptr<ReturnType>::PtrType impl_;
    };

    template<typename ReturnType>
    task<ReturnType> create_task(const task<ReturnType>& t)
    {
        task<ReturnType> created_task(t);
        return created_task;
    }

    namespace details
    {
        template<typename ParamType>
        struct TaskTypeFromParam
        {
#if __cplusplus <= 201103L
            typedef std::result_of<ParamType> type;
#else
            typedef std::invoke_result<ParamType> type;
#endif
        };
    }

    template<typename ParamType>
    task<typename details::TaskTypeFromParam<ParamType>::type> create_task(ParamType param) // , task_options _TaskOptions = task_options())
    {
        return task<typename details::TaskTypeFromParam<ParamType>::type>();
    }
}

#endif // TPL_TASKS_H_