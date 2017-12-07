#pragma once

#include <cstddef>

#include <string>

#include "engine/gui/text_filter.hpp"
#include "engine/gui/widget.hpp"

namespace engine {
    namespace gui {        
        template<text_filter filter = text_filter::DEFAULT>
        class text_field : public virtual widget {
            char * _buffer;
            std::size_t _bufferSize;
            int _filled;
            
        public:            
            text_field(std::size_t bufferSize);

            virtual ~text_field();

            virtual std::string getText() const;

            virtual void setText(const std::string& text);

            virtual void build(void * ctx);

            virtual std::size_t getLength() const;
        };

        template<text_filter filter>
        text_field<filter>::text_field(std::size_t bufferSize) {
            _buffer = new char[bufferSize];
            _bufferSize = bufferSize;
            _filled = 0;
        }

        template<text_filter filter>
        text_field<filter>::~text_field() {
            delete[] _buffer;
        }

        template<text_filter filter>
        std::string text_field<filter>::getText() const {            
            return std::string(_buffer, _filled);
        }

        template<text_filter filter>
        void text_field<filter>::setText(const std::string& text) {            
            text.copy(_buffer, _filled);
        }

        template<text_filter filter>
        std::size_t text_field<filter>::getLength() const {
            return _filled;
        }

        template<>
        void text_field<text_filter::DEFAULT>::build(void * ctx);

        template<>
        void text_field<text_filter::ASCII>::build(void * ctx);

        template<>
        void text_field<text_filter::FLOAT>::build(void * ctx);

        template<>
        void text_field<text_filter::DECIMAL>::build(void * ctx);

        template<>
        void text_field<text_filter::HEX>::build(void * ctx);

        template<>
        void text_field<text_filter::OCT>::build(void * ctx);

        template<>
        void text_field<text_filter::BINARY>::build(void * ctx);
    }
}