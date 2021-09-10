#pragma once

#include "Widget.h"

class GUIEvent {
  // TODO: this abstract class contains all data to process event

  Widget &sender;

  virtual void action() = 0;
};