#pragma once

#include <exception>
#include <jsoncons/json.hpp>
#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <range/v3/all.hpp>
#include <shared_mutex>
#include <vector>

template <typename V, typename ID>
class ViewableContainer {
private:
    std::map<ID, std::unique_ptr<V>> container;
    mutable std::shared_mutex containerLock;

    JSONCONS_TYPE_TRAITS_FRIEND;

protected:
    explicit ViewableContainer(decltype(container) &&container) : container(std::move(container)) {}

public:
    ViewableContainer(const ViewableContainer &) = delete;
    ViewableContainer &operator=(const ViewableContainer &) = delete;
    ViewableContainer(ViewableContainer &&v) noexcept : container(std::move(v.container)) {}
    ViewableContainer() = default;

    virtual std::unique_ptr<V> takeItem(ID itemId) {
        std::unique_lock guard{containerLock};
        auto it = container.find(itemId);
        if (it == container.end() || it->second == nullptr)
            throw std::runtime_error("Could not take item");
        auto result = std::move(it->second);
        container.erase(it);
        return result;
    }

    std::unique_ptr<V> askItem(ID itemId) {
        std::unique_lock guard{containerLock};
        auto it = container.find(itemId);
        if (it == container.end() || it->second == nullptr)
            return {};
        auto result = std::move(it->second);
        container.erase(it);
        return result;
    }

    [[nodiscard]] auto getSlots() const {
        std::shared_lock guard{containerLock};
        return container | ranges::views::keys | ranges::to<std::vector<ID>>();
    }

    // We get element that cannot be modified
    virtual const V &viewItem(ID itemID) const {
        std::shared_lock guard{containerLock};
        auto it = container.find(itemID);
        if (it == container.end() || it->second == nullptr)
            throw std::runtime_error("Could not view item");
        return *it->second;
    }

    // фокусируемся на предмете
    std::optional<V> focusItem(ID itemID) const {
        std::shared_lock guard{containerLock};
        auto it = container.find(itemID);
        if (it == container.end() || it->second == nullptr)
            return {};
        return *it->second;
    }

    std::map<ID, V> view() const {
        std::shared_lock guard{containerLock};
        decltype(view()) copy;
        for (const auto &item : container)
            if (item.second != nullptr)
                copy.emplace(item.first, *item.second);
        return copy;
    }

    template <typename U>
    void populate(ID id, U &&value) {
        std::unique_lock guard{containerLock};
        container[id] = std::forward<U>(value);
    }
};

namespace jsoncons {
    template <typename Id, typename V>
    struct json_type_traits<json, ViewableContainer<V, Id>> {
        static bool is(const json &j) {
            if (!j.is_object()) return false;
            return true;
        }

        static ViewableContainer<V, Id> as(const json &j) {
            return ViewableContainer(j.as<std::map<Id, V>>());
        }

        static json to_json(const ViewableContainer<V, Id> &c) {
            return json{c.container};
        }
    };
}// namespace jsoncons
