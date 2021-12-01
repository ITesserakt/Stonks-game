#pragma once

#include <exception>
#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <range/v3/all.hpp>
#include <shared_mutex>
#include <vector>

template <typename T, typename ID>
class ViewableContainer {
private:
    std::map<ID, std::unique_ptr<T>> container;
    mutable std::shared_mutex containerLock;

public:
    ViewableContainer(const ViewableContainer &) = delete;
    ViewableContainer &operator=(const ViewableContainer &) = delete;
    ViewableContainer() = default;

    virtual std::unique_ptr<T> takeItem(ID itemId) {
        std::unique_lock guard{containerLock};
        auto it = container.find(itemId);
        if (it == container.end() || it->second == nullptr)
            throw std::runtime_error("Could not take item");
        auto result = std::move(it->second);
        container.erase(it);
        return result;
    }

    std::unique_ptr<T> askItem(ID itemId) {
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
    virtual const T &viewItem(ID itemID) const {
        std::shared_lock guard{containerLock};
        auto it = container.find(itemID);
        if (it == container.end() || it->second == nullptr)
            throw std::runtime_error("Could not view item");
        return *it->second;
    }

    // фокусируемся на предмете
    std::optional<T> focusItem(ID itemID) const {
        std::shared_lock guard{containerLock};
        auto it = container.find(itemID);
        if (it == container.end() || it->second == nullptr)
            return {};
        return *it->second;
    }

    std::map<ID, T> view() const {
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