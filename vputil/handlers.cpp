#include "handlers.h"

namespace vputil {

Handlers::Handlers(vp_instance_t vpInstance) : vpInstance(vpInstance) {
    vp_user_data_set(vpInstance, this);
}

} //namespace vputil
