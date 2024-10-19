// study the following code
// from https://github.com/progschj/ThreadPool

#include <condition_variable>
#include <functional>
#include <future>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
#include <stdexcept>
#include <thread>
#include <vector>

class ThreadPool {
public:
    ThreadPool(size_t);

    template <class F, class... Args>
    auto enqueue(F&& f, Args&&... args)
        -> std::future<typename std::result_of<F(Args...)>::type>;

    ~ThreadPool();

private:
    // need to keep track of threads so we can join them
    std::vector<std::thread> workers;
    // the task queue
    std::queue<std::function<void()>> tasks;

    // synchronization
    std::mutex queue_mutex;
    std::condition_variable condition;
    bool stop;
};

// The constructor just launches some amount of workers
inline ThreadPool::ThreadPool(size_t threads)
    : stop(false)
{
    for (size_t i = 0; i < threads; ++i) {
        // 向workers中添加线程，每个线程的任务是执行一个匿名函数，该匿名函数通过[this]
        // 捕获了当前对象(ThreadPool实例)的this指针，因此可以在lambda表达式中访问
        // ThreadPool的成员变量和成员函数
        workers.emplace_back(
            [this] {
                for (;;) {
                    std::function<void()> task;

                    {
                        std::unique_lock<std::mutex> lock(this->queue_mutex);
                        this->condition.wait(lock,
                            [this] { return this->stop || !this->tasks.empty(); });
                        if (this->stop && this->tasks.empty())
                            return;
                        task = std::move(this->tasks.front());
                        this->tasks.pop();
                    }

                    task();
                }
            });
    }
}

// add new work item to the pool
// F表示一个可调用对象的类型，Args是可变参数模板，表示可调用对象的参数
template <class F, class... Args>
auto ThreadPool::enqueue(F&& f, Args&&... args)
    -> std::future<typename std::result_of<F(Args...)>::type>
{
    // std::result_of<F(Args...)>是一个函数模板，用于推导F(Args...)的返回值类型
    // 例如 int foo(int, double), 则std::result_of<decltype(foo)(int, double)>::type是int
    using return_type = typename std::result_of<F(Args...)>::type;

    // 将任务包装成一个std::packaged_task对象，该对象可以被调用，返回值是return_type
    // 允许通过std::future来获取这个任务的返回结果，std::packaged_task和std::future
    // 一起使用，可以在一个线程中执行一个任务，而在另一个线程中获取任务的返回值
    auto task = std::make_shared<std::packaged_task<return_type()>>(
        // std::bind是一个函数模板，用于绑定一个可调用对象和其参数
        // std::forward为了保持参数的引用类型，避免参数被拷贝
        std::bind(std::forward<F>(f), std::forward<Args>(args)...));

    std::future<return_type> res = task->get_future();
    {
        std::unique_lock<std::mutex> lock(queue_mutex);

        // don't allow enqueueing after stopping the pool
        if (stop)
            throw std::runtime_error("enqueue on stopped ThreadPool");

        tasks.emplace([task]() { (*task)(); });
    }
    condition.notify_one();
    return res;
}

// the destructor joins all threads
inline ThreadPool::~ThreadPool()
{
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        stop = true;
    }
    condition.notify_all();
    for (std::thread& worker : workers)
        worker.join();
}

int main()
{
    ThreadPool pool(4);
    std::vector<std::future<int>> results;

    // 即使所有worker都在工作，notify_one()也没有错过任何任务
    for (int i = 0; i < 100; ++i) {
        results.emplace_back(
            pool.enqueue([i] {
                printf("hello %d\n", i);
                std::this_thread::sleep_for(std::chrono::seconds(1));
                printf("world %d\n", i);
                return i * i;
            }));
    }

    pool.~ThreadPool();

    for (auto&& result : results)
        std::cout << result.get() << ' ';
    std::cout << std::endl;
    return 0;
}