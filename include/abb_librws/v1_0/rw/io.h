#pragma once

#include <abb_librws/v1_0/rws_client.h>
#include <abb_librws/common/rw/io.h>


namespace abb :: rws :: v1_0 :: rw :: io
{
    using namespace abb::rws::rw::io;


    /**
     * \brief IO signal subscription resource.
     */
    struct IOSignalSubscribableResource: public SubscribableResource
    {
        /**
         * \brief A constructor.
         *
         * \param name name of the IO signal.
         */
        explicit IOSignalSubscribableResource(std::string const& name)
        :   name(name)
        {
        }


        std::string getURI() const override;

        void processEvent(Poco::XML::Element const& li_element, SubscriptionCallback& callback) const override;


    private:
        /**
         * \brief IO signal name.
         */
        std::string name;
    };


    /**
     * \brief Get values of all IO signals.
     *
     * \param client RWS client.
     *
     * \return Mapping from IO signal names to values.
     */
    IOSignalInfo getIOSignals(RWSClient& client);

    /**
     * \brief A function for retrieving the value of an IO signal.
     *
     * \param client RWS client
     * \param iosignal for the name of the IO signal.
     *
     * \return std::string containing the IO signal's value (empty if not found).
     */
    std::string getIOSignal(RWSClient& client, const std::string& iosignal);


     /**
     * \brief A function for setting the value of an IO signal.
     *
     * \param client RWS client.
     * \param iosignal for the IO signal's name.
     * \param value for the IO signal's new value.
     *
     * \throw \a RWSError if something goes wrong.
     */
    void setIOSignal(RWSClient& client, const std::string& iosignal, const std::string& value);


    /// @brief Get value of a digital signal
    ///
    /// @param client RWS client
    /// @param signal_name Name of the signal
    ///
    /// @return Value of the requested digital signal
    ///
    bool getDigitalSignal(RWSClient& client, std::string const& signal_name);


    /// @brief Get value of an analog signal
    ///
    /// @param client RWS client
    /// @param signal_name Name of the signal
    ///
    /// @return Value of the requested analog signal
    ///
    float getAnalogSignal(RWSClient& client, std::string const& signal_name);


    /// @brief Get value of a group signal
    ///
    /// @param client RWS client
    /// @param signal_name Name of the signal
    ///
    /// @return Value of the requested group signal
    ///
    std::uint32_t getGroupSignal(RWSClient& client, std::string const& signal_name);


    /// @brief Set value of a digital signal
    ///
    /// @param client RWS client
    /// @param signal_name Name of the signal
    /// @param value New value of the signal
    ///
    void setDigitalSignal(RWSClient& client, std::string const& signal_name, bool value);


    /// @brief Set value of an analog signal
    ///
    /// @param client RWS client
    /// @param signal_name Name of the signal
    /// @param value New value of the signal
    ///
    void setAnalogSignal(RWSClient& client, std::string const& signal_name, float value);


    /// @brief Set value of a group signal
    ///
    /// @param client RWS client
    /// @param signal_name Name of the signal
    /// @param value New value of the signal
    ///
    void setGroupSignal(RWSClient& client, std::string const& signal_name, std::uint32_t value);
}