#pragma once

#include <abb_librws/rws.h>

#include <string>


namespace abb :: rws :: rw
{
    /**
     * \brief Robot controller state.
     *
     * The documentation strings are taken from https://developercenter.robotstudio.com/api/RWS?urls.primaryName=Panel%20Service
     */
    enum class ControllerState
    {
        // The robot is starting up. It will shift to state motors off when it has started.
        init,
        // The robot is in a standby state where there is no power to the robot's motors.
        // The state has to be shifted to motors on before the robot can move.
        motorOff,
        // The robot is ready to move, either by jogging or by running programs.
        motorOn,
        // The robot is stopped because the safety runchain is opened. For instance, a door to the robot's cell might be open.
        guardStop,
        // The robot is stopped because emergency stop was activated.
        emergencyStop,
        // The robot is ready to leave emergency stop state. The emergency stop is no longer activated, but the state transition isn't yet confirmed.
        emergencyStopReset,
        // The robot is in a system failure state. Restart required.
        sysFail
    };


    std::ostream& operator<<(std::ostream& os, ControllerState state);


    /**
     * \brief Create \a ControllerState from string.
     *
     * \param str source string
     *
     * \return \a ControllerState matching the value of \a str
     *
     * \throw \a std::invalid_argument if \a str is not from the set of valid strings.
     */
    ControllerState makeControllerState(std::string const& str);


    /**
     * @brief Robot operation mode.
     *
     * The documentation strings are taken from https://developercenter.robotstudio.com/api/rwsApi/panel_opmode_get_page.html
     */
    enum class OperationMode
    {
        // State init
        init,
        // State change request for automatic mode
        autoCh,
        // State change request for manual mode & full speed
        manFCh,
        // State manual mode & reduced speed
        manR,
        // State manual mode & full speed
        manF,
        // State automatic mode
        automatic,
        // Undefined
        undef
    };


    std::ostream& operator<<(std::ostream& os, OperationMode mode);


    /**
     * \brief Create \a OperationMode from string.
     *
     * \param str source string
     *
     * \return \a OperationMode matching the value of \a str
     *
     * \throw \a std::invalid_argument if \a str is not from the set of valid strings.
     */
    OperationMode makeOperationMode(std::string const& str);
}
