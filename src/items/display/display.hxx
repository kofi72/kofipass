#ifndef DISPLAY_HXX_INCLUDED
#define DISPLAY_HXX_INCLUDED

// Item display visitor

struct display_visitor
{
  virtual void visit(class Folder&) = 0;
  virtual void visit(class SimpleLogin&) = 0;
  virtual void visit(class EncryptedFolder&) = 0;
  virtual void visit(class twofa_login&) = 0;
  //virtual void visit(class customItem&) = 0;
};

#endif // DISPLAY_HXX_INCLUDED
