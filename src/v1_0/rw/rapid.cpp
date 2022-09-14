#include <abb_librws/v1_0/rw/rapid.h>

#include <abb_librws/parsing.h>
#include <abb_librws/system_constants.h>


namespace abb :: rws :: v1_0 :: rw :: rapid
{
    /**
     * \brief A method for retrieving the properties of a RAPID symbol.
     *
     * \param resource specifying the RAPID task, module and symbol names for the RAPID resource.
     *
     * \return RWSResult containing the result.
     *
     * \throw \a RWSError if something goes wrong.
     */
    static RWSResult getRAPIDSymbolProperties(RWSClient& client, RAPIDResource const& resource);

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


    RAPIDExecutionInfo getRAPIDExecution(RWSClient& client)
    {
        RAPIDExecutionInfo result;

        std::string const uri = Resources::RW_RAPID_EXECUTION;
        RWSResult xml_content = parseXml(client.httpGet(uri).content());

        Poco::XML::Node const * li_node = xml_content->getNodeByPath("html/body/div/ul/li");
        if (!li_node)
            BOOST_THROW_EXCEPTION(ProtocolError {"Cannot parse RWS response: can't find XML path html/body/div/ul/li"});

        std::string const ctrlexecstate = xmlFindTextContent(li_node, XMLAttributes::CLASS_CTRLEXECSTATE);
        if (ctrlexecstate.empty())
            BOOST_THROW_EXCEPTION(ProtocolError {"Can't find a node with class=\"ctrlexecstate\""});

        std::string const cycle = xmlFindTextContent(li_node, XMLAttribute {"class", "cycle"});
        if (cycle.empty())
            BOOST_THROW_EXCEPTION(ProtocolError {"Can't find a node with class=\"cycle\""});

        return RAPIDExecutionInfo {
            makeRAPIDExecutionState(ctrlexecstate),
            makeRAPIDRunMode(cycle)
        };
    }


    void startRAPIDExecution(RWSClient& client)
    {
        std::string const uri = Resources::RW_RAPID_EXECUTION + "?" + Queries::ACTION_START;
        std::string const content = "regain=continue&execmode=continue&cycle=forever&condition=none&stopatbp=disabled&alltaskbytsp=false";
        client.httpPost(uri, content);
    }


    void stopRAPIDExecution(RWSClient& client, StopMode stopmode, UseTsp usetsp)
    {
        std::stringstream content;
        content << "stopmode=" << stopmode << "&usetsp=" << usetsp;
        client.httpPost("/rw/rapid/execution?action=stop", content.str());
    }


    void resetRAPIDProgramPointer(RWSClient& client)
    {
        client.httpPost(Resources::RW_RAPID_EXECUTION + "?" + Queries::ACTION_RESETPP);
    }


    void getRAPIDSymbolData(RWSClient& client, RAPIDResource const& resource, RAPIDSymbolDataAbstract& data)
    {
        RWSResult const temp_result = getRAPIDSymbolProperties(client, resource);
        std::string const data_type = xmlFindTextContent(temp_result, XMLAttributes::CLASS_DATTYP);

        if (data.getType() == data_type)
            data.parseString(getRAPIDSymbolData(client, resource));
        else
            BOOST_THROW_EXCEPTION(std::invalid_argument {"Argument type does not match the RAPID variable type"});
    }


    void setRAPIDSymbolData(RWSClient& client, const RAPIDResource& resource, const RAPIDSymbolDataAbstract& data)
    {
        setRAPIDSymbolData(client, resource, data.constructString());
    }


    std::string getRAPIDSymbolData(RWSClient& client, RAPIDResource const& resource)
    {
        std::string const uri = generateRAPIDDataPath(resource);
        RWSResult xml_content = parseXml(client.httpGet(uri).content());
        std::string value = xmlFindTextContent(xml_content, XMLAttributes::CLASS_VALUE);

        if (value.empty())
            BOOST_THROW_EXCEPTION(std::logic_error {"RAPID value string was empty"});

        return value;
    }


    void setRAPIDSymbolData(RWSClient& client, const RAPIDResource& resource, const std::string& data)
    {
        std::string uri = generateRAPIDDataPath(resource) + "?" + Queries::ACTION_SET;
        std::string content = Identifiers::VALUE + "=" + data;

        client.httpPost(uri, content);
    }


    static RWSResult getRAPIDSymbolProperties(RWSClient& client, RAPIDResource const& resource)
    {
        std::string const uri = generateRAPIDPropertiesPath(resource);
        return parseXml(client.httpGet(uri).content());
    }


    static std::string generateRAPIDDataPath(const RAPIDResource& resource)
    {
        return "/rw/rapid/symbol/data/RAPID/" + resource.task + "/" + resource.module + "/" + resource.name;
    }


    static std::string generateRAPIDPropertiesPath(const RAPIDResource& resource)
    {
        return "/rw/rapid/symbol/properties/RAPID/" + resource.task + "/" + resource.module + "/"+ resource.name;
    }


    std::vector<RAPIDModuleInfo> getRAPIDModulesInfo(RWSClient& client, const std::string& task)
    {
        std::vector<RAPIDModuleInfo> result;

        std::string const uri = Resources::RW_RAPID_MODULES + "?" + Queries::TASK + task;
        RWSResult const rws_result = parseXml(client.httpGet(uri).content());
        std::vector<Poco::XML::Node*> node_list = xmlFindNodes(rws_result,
                                                                XMLAttributes::CLASS_RAP_MODULE_INFO_LI);

        for (size_t i = 0; i < node_list.size(); ++i)
        {
            std::string name = xmlFindTextContent(node_list.at(i), XMLAttributes::CLASS_NAME);
            std::string type = xmlFindTextContent(node_list.at(i), XMLAttributes::CLASS_TYPE);

            result.push_back(RAPIDModuleInfo(name, type));
        }

        return result;
    }

    std::vector<RAPIDTaskInfo> getRAPIDTasks(RWSClient& client)
    {
        std::vector<RAPIDTaskInfo> result;

        RWSResult const rws_result = parseXml(client.httpGet(Resources::RW_RAPID_TASKS).content());
        std::vector<Poco::XML::Node*> node_list = xmlFindNodes(rws_result, XMLAttributes::CLASS_RAP_TASK_LI);

        for (size_t i = 0; i < node_list.size(); ++i)
        {
            std::string name = xmlFindTextContent(node_list.at(i), XMLAttributes::CLASS_NAME);
            bool is_motion_task = xmlFindTextContent(node_list.at(i), XMLAttributes::CLASS_MOTIONTASK) == SystemConstants::RAPID::RAPID_TRUE;
            bool is_active = xmlFindTextContent(node_list.at(i), XMLAttributes::CLASS_ACTIVE) == "On";
            std::string temp = xmlFindTextContent(node_list.at(i), XMLAttributes::CLASS_EXCSTATE);

            // Assume task state is unknown, update based on contents of 'temp'.
            RAPIDTaskExecutionState execution_state = RAPIDTaskExecutionState::UNKNOWN;

            if(temp == "read")
            {
                execution_state = RAPIDTaskExecutionState::READY;
            }
            else if(temp == "stop")
            {
                execution_state = RAPIDTaskExecutionState::STOPPED;
            }
            else if(temp == "star")
            {
                execution_state = RAPIDTaskExecutionState::STARTED;
            }
            else if(temp == "unin")
            {
                execution_state = RAPIDTaskExecutionState::UNINITIALIZED;
            }

            result.push_back(RAPIDTaskInfo(name, is_motion_task, is_active, execution_state));
        }

        return result;
    }


    void loadModuleIntoTask(RWSClient& client, const std::string& task, const FileResource& resource, const bool replace)
    {
        std::string uri = generateRAPIDTasksPath(task) + "?" + Queries::ACTION_LOAD_MODULE;

        // Path to file should be a direct path, i.e. without "/fileservice/"
        std::string content =
            Identifiers::MODULEPATH + "=" + resource.directory + "/" + resource.filename +
            "&replace=" + ((replace) ? "true" : "false");

        client.httpPost(uri, content);
    }


    void unloadModuleFromTask(RWSClient& client, const std::string& task, const FileResource& resource)
    {
        std::string uri = generateRAPIDTasksPath(task) + "?" + Queries::ACTION_UNLOAD_MODULE;
        std::string content = Identifiers::MODULE + "=" + resource.filename;

        client.httpPost(uri, content);
    }


    static std::string generateRAPIDTasksPath(const std::string& task)
    {
        return Resources::RW_RAPID_TASKS + "/" + task;
    }


    std::string RAPIDExecutionStateSubscribableResource::getURI() const
    {
      return "/rw/rapid/execution;ctrlexecstate";
    }


    void RAPIDExecutionStateSubscribableResource::processEvent(Poco::XML::Element const& li_element, SubscriptionCallback& callback) const
    {
        if (li_element.getAttribute("class") == "rap-ctrlexecstate-ev")
        {
            RAPIDExecutionStateEvent event;
            event.state = rw::makeRAPIDExecutionState(xmlFindTextContent(&li_element, XMLAttribute {"class", "ctrlexecstate"}));
            event.resource = std::make_shared<RAPIDExecutionStateSubscribableResource>();

            callback.processEvent(event);
        }
    }
}