#include "../headers/factory.h"

IController* Factory::getIController() {
  return Encrypter::getInstance();
}
