#include "engine/gui/progress_bar.hpp"

#include "nuklear/nk_ctx.hpp"

namespace engine {
    namespace gui {
        void progress_bar::build(void * ctx) {
            auto nk = reinterpret_cast<nk::nk_ctx*> (ctx);

            nk_prog(&nk->context, _current, _max, 0);
        }

        void progress_bar::setCurrent(unsigned int current) {
            _current = current;
        }

        unsigned int progress_bar::getCurrent() const {
            return _current;
        }

        unsigned int progress_bar::getMax() const {
            return _max;
        }
    }
}