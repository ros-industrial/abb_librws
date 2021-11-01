#pragma once

#include <abb_librws/v1_0/rws_client.h>

#include <abb_librws/rws.h>
#include <abb_librws/common/rw/rapid.h>

#include <Poco/DOM/DOMParser.h>

#include <string>


namespace abb :: rws :: v1_0 :: rw
{
    using namespace rws::rw;


    class RAPIDService
    {
    public:
        explicit RAPIDService(RWSClient& client);

        /**
         * \brief A method for retrieving the execution state of RAPID.
         *
         * https://developercenter.robotstudio.com/api/rwsApi/rapid_execution_get_page.html
         *
         * \return Current RAPID execution state.
         *
         * \throw \a RWSError if something goes wrong.
         */
        RAPIDExecutionInfo getRAPIDExecution();

        /**
         * \brief A method for starting RAPID execution in the robot controller.
         *
         * There can be a delay between the function returns and when the RAPID program enters the "running" state.
         *
         * \throw \a std::runtime_error if something goes wrong.
         */
        void startRAPIDExecution();

        /**
         * \brief A method for stopping RAPID execution in the robot controller.
         *
         * https://developercenter.robotstudio.com/api/rwsApi/rapid_execution_stop_page.html
         *
         * There can be a delay between the function returns and when the RAPID program enters the "stopped" state.
         *
         * \param stopmode stop mode
         * \param usetsp which tasks to stop (?)
         *
         * \throw \a std::runtime_error if something goes wrong.
         */
        void stopRAPIDExecution(StopMode stopmode = StopMode::stop, UseTsp usetsp = UseTsp::normal);

        /**
         * \brief A method for reseting the RAPID program pointer in the robot controller.
         *
         * \throw \a std::runtime_error if something goes wrong.
         */
        void resetRAPIDProgramPointer();

        /**
         * \brief A method for retrieving information about the RAPID modules of a RAPID task defined in the robot controller.
         *
         * \return \a std::vector<RAPIDModuleInfo> containing the RAPID modules information.
         *
         * \throw \a std::runtime_error if something goes wrong.
         */
        std::vector<RAPIDModuleInfo> getRAPIDModulesInfo(const std::string& task);

        /**
         * \brief A method for retrieving information about the RAPID tasks defined in the robot controller.
         *
         * \return \a std::vector<RAPIDTaskInfo> containing the RAPID tasks information.
         *
         * \throw \a std::runtime_error if something goes wrong.
         */
        std::vector<RAPIDTaskInfo> getRAPIDTasks();

        /**
         * \brief A method for retrieving the data of a RAPID symbol.
         *
         * \param resource specifying the RAPID task, module and symbol names for the RAPID resource.
         *
         * \return RWSResult containing the result.
         *
         * \throw \a RWSError if something goes wrong.
         */
        void getRAPIDSymbolData(RAPIDResource const& resource, RAPIDSymbolDataAbstract& data);

        /**
         * \brief A method for retrieving the data of a RAPID symbol in raw text format.
         *
         * See the corresponding "setRAPIDSymbolData(...)" method for examples of RAPID symbols in raw text format.
         *
         * \param task name of the RAPID task containing the RAPID symbol.
         * \param module name of the RAPID module containing the RAPID symbol.
         * \param name name of the RAPID symbol.
         *
         * \return String containing the data.
         *
         * \throw \a RWSError if something goes wrong.
         */
        std::string getRAPIDSymbolData(RAPIDResource const& resource);

        /**
         * \brief A method for setting the data of a RAPID symbol (based on the provided struct representing the RAPID data).
         *
         * \param resource specifying the RAPID task, module and symbol names for the RAPID resource.
         * \param data for the RAPID symbol's new data.
         *
         * \throw \a RWSError if something goes wrong.
         */
        void setRAPIDSymbolData(const RAPIDResource& resource, const RAPIDSymbolDataAbstract& data);

        /**
         * \brief A method for setting the data of a RAPID symbol.
         *
         * \param resource specifying the RAPID task, module and symbol names for the RAPID resource.
         * \param data the RAPID symbol's new data.
         *
         * \throw \a RWSError if something goes wrong.
         */
        void setRAPIDSymbolData(const RAPIDResource& resource, const std::string& data);

        /**
         * \brief A method for loading a module to the robot controller.
         *
         * \param task specifying the RAPID task.
         * \param resource specifying the file's directory and name.
         * \param replace indicating if the actual module into the controller must be replaced by the new one or not.
         *
         * \throw \a std::exception if something goes wrong.
         */
        void loadModuleIntoTask(const std::string& task, const FileResource& resource, const bool replace = false);

        /**
         * \brief A method for unloading a module to the robot controller.
         *
         * \param task specifying the RAPID task.
         * \param resource specifying the file's directory and name.
         *
         * \throw \a std::exception if something goes wrong.
         */
        void unloadModuleFromTask(const std::string& task, const FileResource& resource);

    private:
        /**
         * \brief A method for retrieving the properties of a RAPID symbol.
         *
         * \param resource specifying the RAPID task, module and symbol names for the RAPID resource.
         *
         * \return RWSResult containing the result.
         *
         * \throw \a RWSError if something goes wrong.
         */
        RWSResult getRAPIDSymbolProperties(RAPIDResource const& resource);

        /**
         * \brief Method for generating a RAPID data resource URI path.
         *
         * \param resource specifying the RAPID task, module and symbol names for the RAPID resource.
         *
         * \return std::string containing the path.
         */
        static std::string generateRAPIDDataPath(const RAPIDResource& resource);

        /**
         * \brief Method for generating a RAPID properties resource URI path.
         *
         * \param resource specifying the RAPID task, module and symbol names for the RAPID resource.
         *
         * \return std::string containing the path.
         */
        static std::string generateRAPIDPropertiesPath(const RAPIDResource& resource);

        /**
         * \brief Method for generating a task resource URI path.
         *
         * \param task for the task name.
         *
         * \return std::string containing the path.
         */
        static std::string generateRAPIDTasksPath(const std::string& task);

        RWSClient& client_;
        Poco::XML::DOMParser parser_;
    };
}