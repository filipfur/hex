#pragma once

#include <unordered_map>

namespace hex
{
    using State_ID = uint32_t;

    class State
    {
    public:
        virtual void entry(class State_Machine* sm, bool first) = 0;

        virtual void tick(class State_Machine* sm, float dt) = 0;

        State_ID id() const
        {
            return _id;
        }

        operator State_ID() const
        {
            return _id;
        }

        friend class State_Machine;

    private:
        State_ID _id;
        bool _initialized{false};
    };

    class State_Machine
    {
    public:
        virtual ~State_Machine() noexcept
        {

        }

        void add_state(State& state)
        {
            state._id = _nextId++;
            _states.emplace(state._id, state);
        }

        void transition_to(State_ID state_id, float timer=0.0f)
        {
            _delayed_transition = std::make_pair(state_id, timer);
        }

        void tick(float dt)
        {
            if(_delayed_transition.has_value())
            {
                _delayed_transition.value().second -= dt;
                if(_delayed_transition.value().second <= 0)
                {
                    set_state(_delayed_transition.value().first);
                    _delayed_transition = std::nullopt;
                }
            }
            auto it = _states.find(_current_state);
            if(it == _states.end())
            {
                return;
            }
            if(_entry)
            {
                bool first{false};
                if(!it->second._initialized)
                {
                    first = true;
                    it->second._initialized = true;
                }
                it->second.entry(this, first);
                _entry = false;
                _time_since_entry = 0.0f;
            }
            it->second.tick(this, dt);
            _time += dt;
            _time_since_entry += dt;
        }

        State_ID current_state() const
        {
            return _current_state;
        }

        void terminate()
        {
            _terminated = true;
        }

        bool terminated() const
        {
            return _terminated;
        }

        float time()
        {
            return _time;
        }

        float time_since_entry()
        {
            return _time_since_entry;
        }

    private:
        void set_state(State_ID state_id)
        {
            auto it = _states.find(state_id);
            if(it == _states.end())
            {
                return;
            }
            if(_current_state != state_id)
            {
                _current_state = state_id;
            }
            _entry = true;
        }

        std::unordered_map<State_ID, State&> _states;
        State_ID _current_state{0};
        float _time{0.0f};
        float _time_since_entry{0.0f};
        bool _entry{true};
        std::optional<std::pair<State_ID, float>> _delayed_transition{std::nullopt};
        bool _terminated{false};
        State_ID _nextId{0};
    };
}