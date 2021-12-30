#pragma once

#include <abb_librws/v2_0/rws_client.h>

#include <abb_librws/rws.h>
#include <abb_librws/common/rw/panel.h>

#include <string>


namespace abb :: rws :: v2_0 :: rw
{
    using namespace rws::rw;
}


namespace abb :: rws :: v2_0 :: rw :: panel
{
    /**
     * \brief A function for retrieving the controller state.
     *
     * \param client RWS client
     *
     * \return RWSResult containing the result.
     *
     * \throw \a RWSError if something goes wrong.
     */
    ControllerState getControllerState(RWSClient& client);


    /**
     * \brief Set controller state
     *
     * \param client RWS client
     * \param state new controller state
     *
     * \throw \a RWSError if something goes wrong.
     */
    void setControllerState(RWSClient& client, ControllerState state);


    /**
     * \brief A function for retrieving the operation mode of the controller.
     *
     * \param client RWS client
     *
     * \return RWSResult containing the result.
     *
     * \throw \a RWSError if something goes wrong.
     */
    OperationMode getOperationMode(RWSClient& client);


    /**
     * \brief A function for retrieving the robot controller's speed ratio for RAPID motions (e.g. MoveJ and MoveL).
     *
     * \param client RWS client
     *
     * \return RWSResult containing the result.
     *
     * \throw \a RWSError if something goes wrong.
     */
    unsigned getSpeedRatio(RWSClient& client);


    /**
     * \brief A function for setting the robot controller's speed ratio for RAPID motions (e.g. MoveJ and MoveL).
     *
     * Note: The ratio must be an integer in the range [0, 100] (ie: inclusive).
     *
     * \param client RWS client
     * \param ratio specifying the new ratio.
     *
     * \return RWSResult containing the result.
     *
     * \throw std::out_of_range if argument is out of range.
     * \throw \a RWSError if something goes wrong.
     */
    void setSpeedRatio(RWSClient& client, unsigned int ratio);
}