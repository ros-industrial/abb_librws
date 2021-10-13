#include <abb_librws/rws_rapid.h>

namespace abb :: rws
{
    struct MockRAPIDAtomic
    :   public RAPIDAtomicTemplate<std::string>
    {
        MockRAPIDAtomic(const std::string& value = "") 
        :   RAPIDAtomicTemplate(value) {}

        std::string getType() const override
        {
            return "mock_string";
        }
        
        void parseString(const std::string& value_string) override
        {
            value = value_string;
        }

        std::string constructString() const override
        {
            return "" + value;
        }
    };
}