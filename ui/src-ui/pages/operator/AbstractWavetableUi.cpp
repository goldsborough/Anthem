#include "AbstractWavetableUi.hpp"



AbstractWavetableUi::AbstractWavetableUi(QWidget* parent,
										 int frequency)
: Plot(parent),
  y_(new QVector<double>(Global::wavetableLength)),
  frequency_(frequency)
