#include "Main.h"
#include "Logger.h"

int main() {
	trickfire::Logger::Log(trickfire::Logger::LEVEL_INFO_FINE, "Starting whole program");
	trickfire::Main::Start();
}
