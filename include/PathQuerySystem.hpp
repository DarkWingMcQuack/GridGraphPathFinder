#pragma once

#include <Distance.hpp>
#include <GridCell.hpp>
#include <GridGraph.hpp>
#include <Node.hpp>
#include <condition_variable>
#include <deque>
#include <future>
#include <mutex>
#include <variant>
#include <vector>


namespace pathfinding {

template<class DijkstraSolver>
class PathQuerySystem
{
    using ThreadWork =
        std::packaged_task<void(DijkstraSolver&)>;


public:
    PathQuerySystem(graph::GridGraph graph,
                    std::size_t number_of_threads) noexcept
        : graph_(std::move(graph))
    {
        thread_futures_.reserve(number_of_threads);

        for(std::size_t i{0}; i < number_of_threads; i++) {
            thread_futures_.emplace_back(
                std::async(
                    std::launch::async,
                    [&] { threadLoop(); }));
        }
    }

    auto query(const graph::Node& source,
               const graph::Node& target) const noexcept
        -> std::future<graph::Distance>
    {
        // wrap the function object into a packaged task, splitting
        // execution from the return value:
        std::packaged_task<graph::Distance(DijkstraSolver&)>
            p([source, target](auto& solver) {
                return solver.findDistance(source, target);
            });

        auto r = p.get_future(); // get the return value before we hand off the task
        {
            std::unique_lock lock(mtx_);
            work_queue_.emplace_back(std::move(p)); // store the task<R()> as a task<void()>
        }
        condition_.notify_one(); // wake a thread to work on the task

        return r; // return the future result of the task
    }

    auto query(const graph::Node& source,
               const grid::GridCell& targets) const noexcept
        -> std::future<std::vector<graph::Distance>>
    {
        // wrap the function object into a packaged task, splitting
        // execution from the return value:
        std::packaged_task<std::vector<graph::Distance>(DijkstraSolver&)>
            p([source, targets](auto& solver) {
                return solver.findDistances(source, targets);
            });

        auto r = p.get_future(); // get the return value before we hand off the task
        {
            std::unique_lock lock(mtx_);
            work_queue_.emplace_back(std::move(p)); // store the task<R()> as a task<void()>
        }
        condition_.notify_one(); // wake a thread to work on the task

        return r; // return the future result of the task
    }

    auto query(const grid::GridCell& sources,
               const grid::GridCell& targets) const noexcept
        -> std::future<std::vector<std::vector<graph::Distance>>>
    {
        using graph::Distance;

        // wrap the function object into a packaged task, splitting
        // execution from the return value:
        std::packaged_task<std::vector<std::vector<graph::Distance>>(DijkstraSolver&)>
            p([sources, targets, this]([[maybe_unused]] auto&& _) {
                std::vector<std::future<std::vector<Distance>>> distance_futs;
                distance_futs.reserve(sources.size());
                std::transform(std::begin(sources),
                               std::end(sources),
                               std::back_inserter(distance_futs),
                               [&](auto source) {
                                   return query(source, targets);
                               });


                std::vector<std::vector<Distance>> distances;
                distances.reserve(sources.size());
                std::transform(std::make_move_iterator(std::begin(distance_futs)),
                               std::make_move_iterator(std::end(distance_futs)),
                               std::back_inserter(distances),
                               [](auto fut) {
                                   return fut.get();
                               });

                return distances;
            });

        auto r = p.get_future(); // get the return value before we hand off the task
        {
            std::unique_lock lock(mtx_);
            work_queue_.emplace_back(std::move(p)); // store the task<R()> as a task<void()>
        }
        condition_.notify_one(); // wake a thread to work on the task

        return r; // return the future result of the task
    }


    auto getGraph() const noexcept
        -> const graph::GridGraph&
    {
        return graph_;
    }

    ~PathQuerySystem() noexcept
    {
        {
            std::unique_lock<std::mutex> lock{mtx_};
            for(int i{0}; i < thread_futures_.size(); i++) {
                work_queue_.push_back({});
            }
        }
        condition_.notify_all();
        //wait for all, the dtor of future<void> blocks
        thread_futures_.clear();
    }

private:
    auto threadLoop() const noexcept
        -> void
    {
        DijkstraSolver solver{graph_};
        while(true) {
            // pop a task off the queue:
            ThreadWork f;
            {
                // usual thread-safe queue code:
                std::unique_lock lock{mtx_};
                if(work_queue_.empty()) {
                    condition_.wait(lock, [&] { return !work_queue_.empty(); });
                }
                f = std::move(work_queue_.front());
                work_queue_.pop_front();
            }
            // otherwise, run the task:
            if(!f.valid()) {
                return;
            }
            f(solver);
        }
    }

private:
    graph::GridGraph graph_;
    mutable std::mutex mtx_;
    mutable std::condition_variable condition_;
    mutable std::deque<ThreadWork> work_queue_;
    std::vector<std::future<void>> thread_futures_;
}; // namespace pathfinding

} // namespace pathfinding
