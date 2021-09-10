#pragma once

#include <concepts>
#include <queue>

template <typename T, typename I>
concept QueueLike = requires(T t, I item) {
  t.push(std::move(item));
};

class GUIEvent;

class EventSequence {
private:
  std::queue<std::unique_ptr<GUIEvent>> internal;

public:
  template <std::derived_from<GUIEvent> T>
  void andThen(T event) {
	internal.push(std::make_unique(std::move(event)));
  }

  template <QueueLike<std::unique_ptr<GUIEvent>> T>
  void moveTo(T& destination) {
	std::unique_ptr<GUIEvent> item;
	while(!internal.empty()) {
	  item = std::move(internal.back());
	  internal.pop();
	  destination.push(std::move(item));
	}
  }

  static EventSequence empty() {
		return {};
  }
};