#ifndef VPUTIL_EVENTWRAPPER_H__
#define VPUTIL_EVENTWRAPPER_H__

#include <functional>
#include <VPSDK/VP.h>

namespace vputil {
/**
 *  Wraps VP event handlers and callbacks so you can use function objects 
 *  instead of only C callbacks.
 */
class Handlers {
    vp_instance_t vpInstance;
    std::function<void()> events[VP_HIGHEST_EVENT];
    std::function<void(int,int)> callbacks[VP_HIGHEST_CALLBACK];
        
    template<vp_event_t event>
    static void dispatchEvent(vp_instance_t sender) {
        Handlers* obj = static_cast<Handlers*>(vp_user_data(sender));
        obj->events[event]();
    }
    
    template<vp_callback_t callback>
    static void dispatchCallback(vp_instance_t sender, int rc, int reference) {
        Handlers* obj = static_cast<Handlers*>(vp_user_data(sender));
        obj->callbacks[callback](rc, reference);
    }
public:
    /**
     *  \warning Overwrites user data of the specified vp instance
     */
    Handlers(vp_instance_t vpInstance);
    
    /**
     *  Set event handler
     */
    template<vp_event_t event>
    void set(std::function<void()> handler) {
        events[event] = handler;
        vp_event_set(vpInstance, event, dispatchEvent<event>);
    }
    
    /**
     *  Set callback handler
     */
    template<vp_callback_t callback>
    void set(std::function<void(int, int)> handler) {
        callbacks[callback] = handler;
        vp_callback_set(vpInstance, callback, dispatchCallback<callback>);
    }
};
} // namespace vputil

#endif
