#include "thread"
#include <Frontend.h>
#include <iostream>

class Button: public virtual HoverableWidget {
public:
  EventSequence onHoverStart() override {
	std::cout << "Hover" << std::endl;
	return EventSequence::empty();
  }
  EventSequence onHoverEnd() override {
	std::cout << "End hover" << std::endl;
	return EventSequence::empty();
  }
private:
  void abstract() override {

  }
};

int main() {
  auto f = Frontend();
  auto button = Button();

  button.onHoverEnd().moveTo(f);
}
