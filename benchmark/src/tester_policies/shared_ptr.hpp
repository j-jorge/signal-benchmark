#ifndef TESTER_POLICIES_SHARED_PTR_HPP
#define TESTER_POLICIES_SHARED_PTR_HPP

#include <functional>
#include <memory>
#include <vector>

struct shared_ptr_tester_policy
{
    public:
        using function = std::function< void( int ) >;

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
                callbacks_.emplace_back( s );

                return callbacks_.back();
            }

            void send(int i)
            {
                for ( const auto& p : callbacks_ )
                  {
                    const std::shared_ptr< function > f( p.lock() );

                    if ( f )
                      (*f)(i);
                  }
            }

        private:
            std::vector< std::weak_ptr< function > > callbacks_;
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
              : callback_( std::make_shared< function >( std::ref(slot_) ) )
            {
                sender.connect(callback_);
            }

            int get_total() const
            {
                return slot_.i;
            }

        private:
            slot slot_;
            const std::shared_ptr< function > callback_;
    };

    static const char* get_name()
    {
        return "std::shared_ptr";
    }
};

#endif
