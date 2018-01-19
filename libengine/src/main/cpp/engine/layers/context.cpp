#include "engine/layers/context.hpp"

#include <memory>

#include "engine/layers/base_resources.hpp"
#include "engine/layers/bitmap_font.hpp"
#include "engine/layers/sprite_sheet.hpp"

namespace engine {
    namespace layers {
        namespace {
            struct context_resources : public base_resources {
                context_info _info;
                std::unique_ptr<sprite_sheet[]> _pSpriteSheets;
                std::unique_ptr<bitmap_font[]> _pFonts;

                context_resources(const context_info& info);

                virtual ~context_resources() {}
            };
        }

        context::context(const context_info& info) noexcept {
            _pResources = std::make_unique<context_resources> (info);
        }

        const sprite_sheet * context::getSpriteSheet(int id) const noexcept {
            auto res = dynamic_cast<context_resources * > (_pResources.get());

            return res->_pSpriteSheets.get() + id;
        }

        const bitmap_font * context::getFont(int id) const noexcept {
            auto res = dynamic_cast<context_resources * > (_pResources.get());
            
            return res->_pFonts.get() + id;
        }

        const context_info& context::getInfo() const noexcept {
            auto res = dynamic_cast<context_resources * > (_pResources.get());

            return res->_info;
        }

        namespace {
            context_resources::context_resources(const context_info& info) {
                _info = info;
                _pSpriteSheets = std::make_unique<sprite_sheet[]> (info.nSpriteInfos);
                _pFonts = std::make_unique<bitmap_font[]> (info.nFontInfos);

                for (decltype(info.nSpriteInfos) i = 0; i < info.nSpriteInfos; i++) {
                    _pSpriteSheets[i] = sprite_sheet(info.pSpriteInfos[i]);
                }

                for (decltype(info.nFontInfos) i = 0; i < info.nFontInfos; i++) {
                    _pFonts[i] = bitmap_font(info.pFontInfos[i]);
                }
            }
        }
    }
}