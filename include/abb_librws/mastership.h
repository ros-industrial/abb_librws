#pragma once

#include <string>


namespace abb :: rws
{
    /// @brief Provides functions to request and release RWS mastership.
    ///
    /// A concrete class inherited from @a MastershipManager must implementat a mastership counting mechanism:
    /// each call to @a requestMastership increments the counter, and each call to @a releaseMastership releases the counter.
    /// The initial value of the counter is 0 and the mastership is acquired when the counter goes from 0 to 1, and released
    /// when the counter goes from 1 to 0. Each mastership type has its own counter. This allows the mastership to be independently
    /// acquired in different places of the code and held for as long as it is needed in at least one of the places.
    ///
    class MastershipManager
    {
    public:
        /**
         * @brief Request mastership of given type
         *
         * @param type type of requested mastership
         */
        virtual void requestMastership(std::string const& type) = 0;

        /**
         * Release mastership of given type
         *
         * @param type type of mastership to be released
         */
        virtual void releaseMastership(std::string const& type) = 0;
    };


    /// @brief Holds RWS mastership permissions for as long as the object exists.
    ///
    class Mastership
    {
    public:
        /// @brief Requests mastership permissions.
        ///
        /// @param mastership_manager manages requesting and releasing of mastership.
        /// @param type type of requested mastership (e.g. "edit", "motion")
        ///
        explicit Mastership(MastershipManager& mastership_manager, std::string const& type);

        /// @brief Releases mastership permissions.
        ///
        ~Mastership();

    private:
        MastershipManager& mastership_manager_;
        std::string const type_;
    };
}
