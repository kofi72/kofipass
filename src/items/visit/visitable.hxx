#pragma once

#ifndef VISITABLE_HXX_INCLUDED
#define VISITABLE_HXX_INCLUDED

#include "../display/display.hxx"

struct visitable
{
  virtual void accept_visit(display_visitor& visitor) = 0;
};

#endif // VISITABLE_HXX_INCLUDED
