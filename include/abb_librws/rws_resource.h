#pragma once

#include <abb_librws/v1_0/rws.h>

#include <string>
#include <iosfwd>

namespace abb :: rws
{
  /**
   * \brief IO signal as a resource.
   */
  struct IOSignalResource
  {
    /**
     * \brief A constructor.
     *
     * \param name name of the IO signal.
     */
    explicit IOSignalResource(std::string const& name)
    : name(name)
    {
    }

    /**
     * \brief IO signal name.
     */
    std::string name;
  };


  /**
   * \brief A class for representing a RAPID symbol resource.
   */
  struct RAPIDSymbolResource
  {
    /**
     * \brief A constructor.
     *
     * \param module specifying the name of the RAPID module containing the symbol.
     * \param name specifying the name of the RAPID symbol.
     */
    RAPIDSymbolResource(const std::string& module, const std::string& name)
    :
    module(module),
    name(name)
    {}

    /**
     * \brief The RAPID module name.
     */
    std::string module;

    /**
     * \brief The RAPID symbol name.
     */
    std::string name;
  };


  /**
   * \brief A class for representing a RAPID resource.
   */
  struct RAPIDResource
  {
    /**
     * \brief A constructor.
     *
     * \param task specifying the name of the RAPID task containing the symbol.
     * \param module specifying the name of the RAPID module containing the symbol.
     * \param name specifying the name of the RAPID symbol.
     */
    RAPIDResource(const std::string& task, const std::string& module, const std::string& name)
    :
    task(task),
    module(module),
    name(name)
    {}

    /**
     * \brief A constructor.
     *
     * \param task specifying the name of the RAPID task containing the symbol.
     * \param symbol specifying the names of the RAPID module and the the symbol.
     */
    RAPIDResource(const std::string& task, const RAPIDSymbolResource& symbol)
    :
    task(task),
    module(symbol.module),
    name(symbol.name)
    {}

    /**
     * \brief The RAPID task name.
     */
    std::string task;

    /**
     * \brief The RAPID module name.
     */
    std::string module;

    /**
     * \brief The RAPID symbol name.
     */
    std::string name;
  };


  /**
   * \brief RAPID execution state subscription resource
   */
  struct RAPIDExecutionStateResource
  {
  };


  /**
   * \brief Controller operation mode subscription resource
   */
  struct OperationModeResource
  {
  };


  /**
   * \brief Controller state subscription resource
   */
  struct ControllerStateResource
  {
  };


  /**
   * \brief A class for representing a file resource.
   */
  struct FileResource
  {
    /**
     * \brief A constructor.
     *
     * \param filename specifying the name of the file.
     * \param directory specifying the directory of the file on the robot controller (set to $home by default).
     */
    FileResource(const std::string& filename,
                  const std::string& directory = v1_0::Identifiers::HOME_DIRECTORY)
    :
    filename(filename),
    directory(directory)
    {}

    /**
     * \brief The file's name.
     */
    std::string filename;

    /**
     * \brief The file's directory on the robot controller.
     */
    std::string directory;
  };

  std::ostream& operator<<(std::ostream& os, RAPIDResource const& resource);
} // end of namespace abb::rws