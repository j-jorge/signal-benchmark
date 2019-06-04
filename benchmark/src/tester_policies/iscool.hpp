#ifndef TESTER_POLICIES_ISCOOL_HPP
#define TESTER_POLICIES_ISCOOL_HPP

#include <iscool/signals/signal.h>
#include <iscool/signals/signal.impl.tpp>

struct iscool_tester_policy
{
    struct event_sender
    {
        public:
            using signal = iscool::signals::signal<void(int)>;

        public:
            template<class Slot>
            auto connect(Slot&& s)
            {
                return signal_.connect(std::forward<Slot>(s));
            }

            void send(const int i)
            {
                signal_(i);
            }

        private:
            signal signal_;
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
            {
                sender.connect(std::ref(slot_));
            }

            int get_total() const
            {
                return slot_.i;
            }

        private:
            slot slot_;
    };

    static const char* get_name()
    {
        return "iscool::signals";
    }
};

#endif
