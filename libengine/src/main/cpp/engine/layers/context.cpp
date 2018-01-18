#include "engine/layers/context.hpp"

#include <memory>

#include "engine/layers/base_resources.hpp"

namespace engine {
    namespace layers {
        namespace {
            struct context_resources : public base_resources {
                context_info _info;
                std::unique_ptr<sprite_sheet[]> _spriteSheets;

                context_resources(const context_info& info);

                virtual ~context_resources() {}
            };
        }

        context::context(const context_info& info) noexcept {
            _pResources = std::make_unique<context_resources> (info);
        }

        const engine::layers::sprite_sheet * context::getSpriteSheet(int id) const noexcept {
            auto res = dynamic_cast<context_resources * > (_pResources.get());

            return res->_spriteSheets.get() + id;
        }

        const context_info& context::getInfo() const noexcept {
            auto res = dynamic_cast<context_resources * > (_pResources.get());

            return res->_info;
        }

        namespace {
            context_resources::context_resources(const context_info& info) {
                _info = info;
                _spriteSheets = std::make_unique<sprite_sheet[]> (info.nSpriteInfos);

                for (decltype(info.nSpriteInfos) i = 0; i < info.nSpriteInfos; i++) {
                    _spriteSheets[i] = sprite_sheet(info.pSpriteInfos[i]);
                }
            }
        }
    }
}