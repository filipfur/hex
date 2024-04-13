#include <cassert>
#include <unordered_map>
#include <vector>
#include <string>

#include "hex/extensions.h"
#include "hex/statemachine.h"
#include "hex/eventqueue.h"
#include "hex/iterations.h"

enum TestState
{
    IDLE,
    WALKING,
    JUMPING
};

struct MetaState : public hex::State
{
    virtual void entry(hex::State_Machine* sm, bool first) override
    {
        if(first)
        {
            ++initCounter;
        }
        ++entryCounter;
    }

    virtual void tick(hex::State_Machine* sm, float dt) override
    {
        timeSpent += dt;
    }

    size_t initCounter{0};
    size_t entryCounter{0};
    float timeSpent{0.0f};
};

struct Idle : public MetaState
{
};

struct Walking : public MetaState
{
    virtual void entry(hex::State_Machine* sm, bool first) override
    {
        MetaState::entry(sm, first);
        if(first)
        {
            sm->transition_to(JUMPING, 7.0f);
        }
    }

    virtual void tick(hex::State_Machine* sm, float dt) override
    {
        MetaState::tick(sm, dt);
        ++x;
    }

    int x{0};
};

struct Jumping : public MetaState
{
    virtual void entry(hex::State_Machine* sm, bool first) override
    {
        MetaState::entry(sm, first);
        sm->transition_to(WALKING, 1.0f);
    }

    virtual void tick(hex::State_Machine* sm, float dt) override
    {
        MetaState::tick(sm, dt);
        ++y;
    }

    int y{0};
};

enum TestEvents
{
    TEST
};

struct TestEvent : public hex::Event
{
    virtual hex::Event_ID type() const override
    {
        return TEST;
    };
};

int main(int argc, const char* argv[])
{
    assert(static_cast<int>(1.51f) == hex::as<int>(1.51f));

    Idle idle;
    Walking walking;
    Jumping jumping;
    hex::State_Machine state_machine;
    state_machine.add_state(idle);
    state_machine.add_state(walking);
    state_machine.add_state(jumping);

    const float dt{0.1f};
    for(float t{0.0f}; t < 10.0f; t += dt)
    {
        state_machine.tick(dt);
    }

    assert(idle.initCounter == 1);
    assert(idle.entryCounter == 1);
    assert(idle.timeSpent >= 10.0f);

    state_machine.transition_to(walking);

    for(float t{0.0f}; t < 10.0f; t += dt)
    {
        state_machine.tick(dt);
    }

    assert(walking.initCounter == 1);
    assert(walking.entryCounter == 2);
    assert(walking.x == 90);

    assert(jumping.y == 10);
    assert(jumping.initCounter == 1);
    assert(jumping.entryCounter == 1);

    hex::Event_Queue event_queue;

    event_queue.push(std::unique_ptr<TestEvent>(new TestEvent()));
    event_queue.push(std::unique_ptr<TestEvent>(new TestEvent()));
    event_queue.push(std::unique_ptr<TestEvent>(new TestEvent()));


    assert(event_queue.pull()->id == 0);
    assert(event_queue.pull()->id == 1);
    assert(event_queue.pull()->id == 2);

    std::unordered_map<uint32_t, std::string> mmap = {
        {3, std::string{"three"}},
        {0, std::string{"zero"}},
        {7, std::string{"seven"}}
    };

    auto removed = hex::iter_remove_if(mmap, [](auto& entry){
        printf("%d: %s\n", entry.first, entry.second.c_str());
        return entry.first % 2 != 0;
    });

    assert(removed == 2);

    std::vector<int> v = {
        1, 2, 3, 4, 5, 6, 7
    };

    removed += hex::iter_remove_if(v, [](int val) {
        return val % 2 != 0;
    });

    assert(removed == 6);

    printf("#removed: %u\n", removed);

    hex::iter_combine(v, [](int v1, int v2) {
        printf("%dx%d=%d\n", v1, v2, v1 * v2);
    });

    printf("sum: %d %.1f\n", hex::iter_sum(v), hex::iter_sum(v) / hex::as<float>(v.size()));

    return 0;
}