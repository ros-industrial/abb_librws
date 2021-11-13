#pragma once

#include <abb_librws/v1_0/rws_client.h>

#include <abb_librws/rws.h>
#include <abb_librws/common/rw/panel.h>

#include <Poco/DOM/DOMParser.h>

#include <string>


namespace abb :: rws :: v1_0 :: rw
{
    using namespace rws::rw;


    class PanelService
    {
    public:
        explicit PanelService(RWSClient& client);

        /**
         * \brief A method for retrieving the controller state.
         *
         * \return RWSResult containing the result.
         *
         * \throw \a RWSError if something goes wrong.
         */
        ControllerState getControllerState();


        /**
         * @brief Set controller state
         *
         * @param state new controller state
         *
         * @throw @a RWSError if something goes wrong.
         */
        void setControllerState(ControllerState state);


        /**
         * \brief A method for retrieving the operation mode of the controller.
         *
         * \return RWSResult containing the result.
         *
         * \throw \a RWSError if something goes wrong.
         */
        OperationMode getOperationMode();


        /**
         * \brief A method for retrieving the robot controller's speed ratio for RAPID motions (e.g. MoveJ and MoveL).
         *
         * \return RWSResult containing the result.
         *
         * \throw \a RWSError if something goes wrong.
         */
        unsigned getSpeedRatio();


        /**
         * \brief A method for setting the robot controller's speed ratio for RAPID motions (e.g. MoveJ and MoveL).
         *
         * Note: The ratio must be an integer in the range [0, 100] (ie: inclusive).
         *
         * \param ratio specifying the new ratio.
         *
         * \return RWSResult containing the result.
         *
         * \throw std::out_of_range if argument is out of range.
         * \throw \a RWSError if something goes wrong.
         */
        void setSpeedRatio(unsigned int ratio);

    private:

        RWSClient& client_;
        Poco::XML::DOMParser parser_;
    };
}