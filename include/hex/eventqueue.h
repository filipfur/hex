#pragma once

#include <queue>

namespace hex
{
    using Event_ID = uint32_t;

    struct Event
    {
        Event_ID id;

        Event() { id = nextId++; }

        virtual ~Event() noexcept { }

        virtual Event_ID type() const = 0;

    private:
        inline static Event_ID nextId{0};
    };

    class Event_Queue
    {
    public:
        void push(std::unique_ptr<Event> event)
        {
            _queue.push(std::move(event));
        }

        std::unique_ptr<Event> pull()
        {
            auto event = std::move(_queue.front());
            _queue.pop();
            return event;
        }

    private:
        std::queue<std::unique_ptr<Event>> _queue;
    };
}