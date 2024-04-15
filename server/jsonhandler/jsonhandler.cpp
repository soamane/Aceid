#include "jsonhandler.h"

JsonHandler* JsonHandler::getInstance() {
	static JsonHandler* Instance = new JsonHandler();
	return Instance;
}
