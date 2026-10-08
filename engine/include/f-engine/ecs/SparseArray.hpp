#pragma once

#include <algorithm>
#include <optional>
#include <vector>

namespace fengine {
    namespace ecs {
        template <typename Component>
        class SparseArray {
            public:
                using value_type = std::optional<Component>;
                using reference_type = value_type &;
                using const_reference_type = value_type const &;
                using container_t = std::vector<value_type>;
                using size_type = typename container_t::size_type;
                using iterator = typename container_t::iterator;
                using const_iterator = typename container_t::const_iterator;

                SparseArray() : _data() {
                    _data.reserve(5);
                };

                SparseArray(SparseArray const &arr) {// Copy constructor
                    _data = arr._data;
                };

                SparseArray(SparseArray &&arr) noexcept {// Move constructor
                    _data = std::move(arr._data);
                };

                ~SparseArray() {};

                SparseArray &operator=(SparseArray const &arr) {// Copy assignment operator
                    _data = arr._data;
                    return *this;
                };

                SparseArray &operator=(SparseArray &&arr) noexcept {// Move assignment operator
                    _data = std::move(arr._data);
                    return *this;
                };

                reference_type operator[](size_t idx) {
                    if (idx >= _data.capacity()) {
                        _data.resize((_data.capacity() + 1 + idx) * 2, std::nullopt);
                    }
                    return _data[idx];
                };

                const_reference_type operator[](size_t idx) const {
                    if (idx >= _data.capacity()) {
                        _data.resize((_data.capacity() + 1 + idx) * 2, std::nullopt);
                    }
                    return _data[idx];
                };

                iterator begin() {
                    return _data.begin();
                };

                const_iterator begin() const {
                    return _data.begin();
                };

                const_iterator cbegin() const {
                    return _data.cbegin();
                };

                iterator end() {
                    return _data.end();
                };

                const_iterator end() const {
                    return _data.end();
                };

                const_iterator cend() const {
                    return _data.cend();
                };

                size_type size() const {
                    return _data.size();
                };

                reference_type insert_at(size_type pos, Component const &component) {
                    if (pos >= _data.capacity()) {
                        _data.resize((_data.capacity() + 1 + pos) * 2, std::nullopt);
                    }
                    _data[pos] = component;
                    return _data[pos];
                };

                reference_type insert_at(size_type pos, Component &&component) {
                    if (pos >= _data.capacity()) {
                        _data.resize((_data.capacity() + 1 + pos) * 2, std::nullopt);
                    }
                    _data[pos] = std::move(component);
                    return _data[pos];
                };

                template <class... Params>
                reference_type emplace_at(size_type pos, Params &&...params) {
                    if (pos >= _data.capacity()) {
                        _data.resize((_data.capacity() + 1 + pos) * 2, std::nullopt);
                    }

                    value_type opt = std::make_optional<Component>(params...);

                    _data[pos].emplace(opt);
                    return _data[pos];
                };

                void erase(size_type pos) {
                    value_type &val = _data.at(pos);
                    if (val.has_value()) {
                        val.reset();
                        val = std::nullopt;
                    }
                };

                size_type get_index(value_type const &val) const {
                    iterator it = std::find(begin(), end(), val);
                    return std::distance(begin(), it);
                };

            private:
                container_t _data;
        };
    }
}
