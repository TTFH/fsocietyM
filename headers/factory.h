#ifndef FACTORY_H
#define FACTORY_H

#include "encrypter.h"

class Factory {
public:
  IController* getIController();
};

#endif
