#pragma once

#include "Kmpleete/Base/Kmpleete_api.h"
#include "Kmpleete/Base/nullability.h"
#include "Kmpleete/Base/pointers.h"
#include "Kmpleete/Base/types_aliases.h"
#include "Kmpleete/Base/optional.h"
#include "Kmpleete/Core/settings_document.h"
#include "Kmpleete/Profile/profiler_fwd.h"
#include "Kmpleete/Log/log_class_macro.h"


namespace Kmpleete
{
    //! Manager of settings document objects, responsible for managing lifetime of contained documents,
    //! creating/deleting document objects, merging, saving and loading documents from/to a file.
    //! @see SettingsDocument
    class KMP_API SettingsManager
    {
        KMP_LOG_CLASSNAME(SettingsManager)
        KMP_PROFILE_CONSTRUCTOR_DECLARE()
        KMP_DISABLE_COPY_MOVE(SettingsManager)

    public:
        explicit SettingsManager(const Filepath& filepath);
        ~SettingsManager() = default;

        KMP_NODISCARD OptionalRef<SettingsDocument> PutSettingsDocument(const String& name);
        KMP_NODISCARD OptionalRef<SettingsDocument> GetSettingsDocument(const String& name) const;

        bool LoadSettings();
        bool SaveSettings() const;

        void SetFilepath(const Filepath& filepath) noexcept;
        KMP_NODISCARD const Filepath& GetFilepath() const noexcept;

    private:
        Filepath _filepath;
        HashMap<String, UPtr<SettingsDocument>> _namedSettingsDocuments;
    };
    //--------------------------------------------------------------------------
}