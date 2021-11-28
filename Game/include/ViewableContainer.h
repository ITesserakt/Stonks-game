#pragma once

#include <map>
#include <memory>
#include <exception>

template<typename T, typename ID>
class ViewableContainer {
protected:
    std::map<ID, std::unique_ptr<T>> container;

public:
    ViewableContainer(const ViewableContainer&) = delete;
    ViewableContainer& operator=(const ViewableContainer&) = delete;
    ViewableContainer() = default;

    virtual std::unique_ptr<T> takeItem(ID itemId) {
        auto it = container.find(itemId);
        if (it == container.end() || it->second == nullptr)
            throw std::runtime_error("Could not take item");
        auto result = std::move(it->second);
        container.erase(it);
        return result;
    }

    [[nodiscard]] auto getSlots() const {
        std::vector<ID> result;
        for (const auto &item : container)
            result.push_back(item.first);
        return result;
    }

    // We get element that cannot be modified
    virtual const T &viewItem(ID itemID) const {
        auto it = container.find(itemID);
        if (it == container.end() || it->second == nullptr)
            throw std::runtime_error("Could not view item");
        return *it->second.get();
    }

    bool hasItem(ID itemId) const {
        auto it = container.find(itemId);
        if (it == container.end() || it->second == nullptr)
            return false;
        return true;
    }
};