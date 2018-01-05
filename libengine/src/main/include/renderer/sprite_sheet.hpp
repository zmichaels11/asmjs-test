#pragma once

#include <memory>

#include "renderer/image_view.hpp"
#include "renderer/sprite_sheet_info.hpp"

namespace renderer {
    struct sprite_sheet_res {
        virtual ~sprite_sheet_res();
    };

    class sprite_layer;

    class sprite_sheet {
        sprite_sheet_info _info;
        std::unique_ptr<sprite_sheet_res> _pResources;

        void * getTexture() const;

        friend class sprite_layer;
    public:
        sprite_sheet(const sprite_sheet_info& info);

        virtual ~sprite_sheet() {}

        const sprite_sheet_info& getInfo() const;

        std::size_t getSpriteCount() const;

        const image_view& getSpriteView(int id) const;        
    };
}