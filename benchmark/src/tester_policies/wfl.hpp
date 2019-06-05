#ifndef TESTER_POLICIES_WFL_HPP
#define TESTER_POLICIES_WFL_HPP

#include <wfl/shared_function.hpp>
#include <wfl/weak_function.hpp>

#include <vector>

struct wfl_tester_policy
{
    struct event_sender
    {
        public:
            event_sender()
            {
                callbacks_.reserve( 10000000 );
            }
      
            template<class Slot>
            auto connect(Slot&& s)
            {
                callbacks_.emplace_back( std::forward<Slot>(s) );
            }

            void send(int i)
            {
                for ( const auto& f : callbacks_ )
                    f(i);
            }

        private:
            std::vector< wfl::weak_function< void( int ) > > callbacks_;
    };

    struct event_receiver
    {
        private:
            struct slot
            {
                void operator()(int value)
                {
                    i += value;
                }

                int i = 0;
            };

        public:
            event_receiver(event_sender& sender)
              : callback_(std::ref(slot_))
            {
                sender.connect(callback_);
            }

            int get_total() const
            {
                return slot_.i;
            }

        private:
            slot slot_;
            const wfl::shared_function< void( int ) > callback_;
    };

    static const char* get_name()
    {
        return "wfl";
    }
};

#endif
