#pragma once

#include "utils.h"
#include <vector>
#include "EventSequence.h"

class Widget {
  // TODO: Base class to represent widget, i.e. text label, button, etc

protected:
  std::string name;
  std::weak_ptr<Widget> parent;
  std::shared_ptr<Widget> canvas;
  std::vector<std::shared_ptr<Widget>> children;

private:
  virtual void abstract() = 0;
};

class PositionedWidget : public virtual Widget {
protected:
  UIRect position;
  Align align;
};

class HoverableWidget: public virtual PositionedWidget {
public:
  virtual EventSequence onHoverStart() = 0;
  virtual EventSequence onHoverEnd() = 0;
};