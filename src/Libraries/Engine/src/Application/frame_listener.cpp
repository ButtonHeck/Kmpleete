#include "Kmpleete/Application/frame_listener.h"
#include "Kmpleete/Application/frame_listener_manager.h"
#include "Kmpleete/Base/exception.h"
#include "Kmpleete/Log/log.h"


namespace Kmpleete
{
    FrameListener::FrameListener(FrameListenerManager& frameListenerManager, const StringID& sid, UInt8 priority)
        : _sid(sid)
        , _priority(priority)
        , _frameListenerManager(frameListenerManager)
        , _active(true)
    {
        if (not _frameListenerManager.AddFrameListener(this))
        {
            KMP_LOG_ERROR("failed to attach frame listener '{}' to manager", sid);
            throw RuntimeError("FrameListener: failed to attach to frame listener manager");
        }
    }
    //--------------------------------------------------------------------------

    FrameListener::~FrameListener()
    {
        _frameListenerManager.RemoveFrameListener(this);
    }
    //--------------------------------------------------------------------------

    StringID FrameListener::GetSID() const noexcept
    {
        return _sid;
    }
    //--------------------------------------------------------------------------

    UInt8 FrameListener::GetPriority() const noexcept
    {
        return _priority;
    }
    //--------------------------------------------------------------------------

    bool FrameListener::IsActive() const noexcept
    {
        return _active;
    }
    //--------------------------------------------------------------------------

    void FrameListener::SetActive(bool active)
    {
        if (active == _active)
        {
            return;
        }

        PushCommand(FrameListenerCommand{ .code = active ? FrameListenerCommandCode::Activate : FrameListenerCommandCode::Deactivate, .sid = _sid });
    }
    //--------------------------------------------------------------------------

    void FrameListener::OnEvent(Events::Event& event)
    {
        _eventDispatcher.Dispatch(event);
    }
    //--------------------------------------------------------------------------

    void FrameListener::PushCommand(FrameListenerCommand&& command) noexcept
    {
        _frameListenerManager.PushCommand(std::move(command));
    }
    //--------------------------------------------------------------------------

    void FrameListener::_SetActive(bool active)
    {
        if (active == _active)
        {
            return;
        }

        _active = active;

        _active ? OnActivated() : OnDeactivated();
    }
    //--------------------------------------------------------------------------
}