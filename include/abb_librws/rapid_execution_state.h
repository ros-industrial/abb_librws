#pragma once


namespace abb :: rws
{
    /**
     * \brief State of RAPID program.
     */
    enum class RAPIDExecutionState : bool
    {
        stopped = false,
        running = true
    };
}