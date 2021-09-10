#pragma once

#include <tbb/concurrent_queue.h>
#include "GUIEvent.h"

class Frontend {
  //TODO: here we need to specialize a facade to contact with gui

protected:
	tbb::concurrent_queue<std::unique_ptr<GUIEvent>> todo;
};