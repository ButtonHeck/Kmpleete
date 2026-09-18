#pragma once

#include "Kmpleete/Base/kmpleete_api.h"
#include "Kmpleete/Base/pointers.h"
#include "Kmpleete/Base/types_aliases.h"
#include "Kmpleete/Base/string_id.h"
#include "Kmpleete/Assets/texture_asset.h"
#include "Kmpleete/Graphics/graphics_base.h"
#include "Kmpleete/Graphics/graphics_backend.h"
#include "Kmpleete/Log/log_class_macro.h"
#include "Kmpleete/Profile/profiler.h"


namespace Kmpleete
{
    namespace Graphics
    {
        class Image;
    }


    namespace Assets
    {
        //! Manager of texture assets, responsible for managing lifetime of contained asset objects,
        //! adding/deleting texture assets.
        //! If this manager has successfully been created - then there is the asset with StringID = 0 that holds
        //! "Error" texture (little pink/black square)
        //! @see Assets::TextureAsset
        class KMP_API TextureAssetManager
        {
            KMP_LOG_CLASSNAME(TextureAssetManager)
            KMP_DISABLE_COPY_MOVE(TextureAssetManager)
            KMP_PROFILE_CONSTRUCTOR_DECLARE()

        public:
            static constexpr StringID ErrorTextureSID = 0;

            explicit TextureAssetManager(Graphics::GraphicsBackend& graphicsBackend);
            ~TextureAssetManager() = default;

            bool CreateAsset(StringID textureSid, const Filepath& filepath, TextureSubTypeMaskBits subTypeMask, bool flipVertically = false);
            bool CreateAsset(StringID textureSid, const Graphics::Image& image, TextureSubTypeMaskBits subTypeMask);

            KMP_NODISCARD const Assets::TextureAsset& GetAsset(StringID textureSid) const;
            KMP_NODISCARD Assets::TextureAsset& GetAsset(StringID textureSid);

            void RemoveAssets(const Vector<StringID>& sids);
            KMP_NODISCARD bool RemoveAsset(StringID sid);

            KMP_NODISCARD UInt64 GetAssetsCount() const noexcept;

        private:
            KMP_NODISCARD bool _CreateErrorTextureAsset();
            KMP_NODISCARD bool _TextureSidIsValid(StringID textureSid);

        private:
            Graphics::GraphicsBackend& _graphicsBackend;
            StringIDHashMap<UPtr<Assets::TextureAsset>> _textures;
        };
        //--------------------------------------------------------------------------
    }
}