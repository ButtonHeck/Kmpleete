#if not defined (KMP_CONFIG_TYPE_PRODUCTION)

#include "Kmpleete/Core/assertion.h"
#include "Kmpleete/Log/log.h"


namespace Kmpleete
{
    Assertion::Assertion(const String& expression, const String& file, const String& function, int line, Consequence consequence /*= Consequence::Terminate*/)
        : _file(file)
        , _function(function)
        , _line(line)
        , _consequence(consequence)
    {
        Utils::ToSStream(_stream, "Assertion failed! \"", expression, "\"");
    }
    //--------------------------------------------------------------------------

    Assertion::~Assertion()
    {
        if (_consequence == Consequence::Terminate)
        {
            KMP_LOG_CRITICAL("[file {}, function {}, line {}]: {}", _file, _function, _line, _stream.str());
            std::terminate();
        }
        else
        {
            KMP_LOG_ERROR("[file {}, function {}, line {}]: {}", _file, _function, _line, _stream.str());
        }
    }
    //--------------------------------------------------------------------------

    Assertion& Assertion::Message(const String& message)
    {
        Utils::ToSStream(_stream, "\n\tMessage: ", message);
        return *this;
    }
    //--------------------------------------------------------------------------
}
#endif