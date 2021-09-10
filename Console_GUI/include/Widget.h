#pragma once

#include "utils.h"
#include <vector>

class Widget {
  // TODO: Base class to represent widget, i.e. text label, button, etc

protected:
  std::string name;
  std::vector<std::shared_ptr<Widget>> children;

private:
  virtual void abstract() = 0;
};

class PositionedWidget : public Widget {
protected:
  UIRect position;
  Align align;

private:
  virtual void abstract() = 0;
};