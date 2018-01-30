#include "renderer/sprite_sheet.hpp"

#include <cstddef>
#include <cstdint>
#include <cstring>

#include <algorithm>
#include <memory>
#include <vector>

#include "graphics/texture.hpp"

#include "renderer/image_view.hpp"

#include "util.hpp"

namespace renderer {
    namespace {
        struct sprite_sheet_res_impl : public virtual sprite_sheet_res {
            virtual ~sprite_sheet_res_impl() {}

            graphics::texture texture;
            std::vector<image_view> spriteRefs;
        };
    }

    sprite_sheet::sprite_sheet(const sprite_sheet_info& info) {
        _info = info;

        auto pResources = std::make_unique<sprite_sheet_res_impl>();
        
        pResources->spriteRefs.reserve(info.spriteImageCount);

        std::size_t width = 0;
        std::size_t height = 0;

        for (auto pSpriteInfo = info.pSpriteImages; pSpriteInfo != (info.pSpriteImages + info.spriteImageCount); pSpriteInfo++) {
            width = std::max(width, static_cast<std::size_t> (pSpriteInfo->pImage->getWidth()));
            height = std::max(height, static_cast<std::size_t> (pSpriteInfo->pImage->getHeight()));
        }            

        width = util::bestFitPowerOf2(width);
        height = util::bestFitPowerOf2(height);
        
        std::int32_t imageId = 0;

        auto samplerInfo = graphics::sampler_info::defaults();

        samplerInfo.filters.mag = graphics::mag_filter::NEAREST;
        samplerInfo.filters.min = graphics::min_filter::NEAREST;
        samplerInfo.addressing.wrapS = graphics::address_mode::CLAMP_TO_EDGE;
        samplerInfo.addressing.wrapT = graphics::address_mode::CLAMP_TO_EDGE;

        auto texture = graphics::texture(
            {{width, height, 1}, 
            info.spriteImageCount, 1, 
            samplerInfo, 
            graphics::internal_format::RGBA8});                

        for (auto pSpriteInfo = info.pSpriteImages; pSpriteInfo != (info.pSpriteImages + info.spriteImageCount); pSpriteInfo++) {
            texture.subImage(0, 0, 0, imageId++, pSpriteInfo->pImage);
        }            

        std::swap(pResources->texture, texture);

        _pResources.reset(pResources.release());
    }

    const sprite_sheet_info& sprite_sheet::getInfo() const {
        return _info;
    }

    std::size_t sprite_sheet::getSpriteCount() const {
        auto res = reinterpret_cast<sprite_sheet_res_impl*> (_pResources.get());

        return res->spriteRefs.size();
    }

    const image_view& sprite_sheet::getSpriteView(int id) const {
        auto res = reinterpret_cast<sprite_sheet_res_impl*> (_pResources.get());

        return res->spriteRefs[id];
    }

    void * sprite_sheet::getTexture() const {
        auto res = reinterpret_cast<sprite_sheet_res_impl*> (_pResources.get());

        return reinterpret_cast<void*> (&res->texture);
    }
}