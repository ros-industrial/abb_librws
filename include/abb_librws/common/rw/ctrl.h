#pragma once

#include <abb_librws/rws.h>

#include <string>


namespace abb :: rws :: rw
{
     /**
     * @brief Controller restart type.
     *
     * The documentation strings are taken from https://developercenter.robotstudio.com/api/rwsApi/ctrl_restart_page.html
     */
    enum class RestartMode
    {
        restart, /*!< The controller will be restarted. The state is saved and any changed system parameter settings will be activated after the restart. */
        shutdown, /*!< The main computer will be shut down. Should be used if the controller UPS is broken. */
        xstart, /*!< The controller will be restarted and the Boot Application will be started. The current system is saved and deactivated (the controller is non-functional, for advanced maintenance only). */
        istart, /*!< The controller will be restarted. The current system parameter settings and RAPID programs will be discarded, and the original system installation settings will be used. */
        pstart, /*!< The controller will be restarted. The current RAPID programs and data will be discarded but not the system parameter settings. */
        bstart /*!<  The controller will be restarted. The last automatically saved system state will be loaded. */
    };

    std::ostream& operator<<(std::ostream& os, RestartMode mode);
}