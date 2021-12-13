#pragma once

#include "widgets/Graphic.h"
#include "Statistics.h"
#include "WorldState.h"

// This graphic is using data from statistic and doesn't use it's own std::list<double> values
// It has SHARED data
class SharedGraphic : public Graphic {
protected:
    void updateData(double num) override;
    WorldState& state;

public:
    SharedGraphic(const std::string &name, std::string OY, std::string OX,
                   UISize sizeOfGraph, WorldState& state)
        : Graphic(name,OY,OX, sizeOfGraph, [](){ return 0.0; }), Widget(name),
          state(state)
    {
    }
};
