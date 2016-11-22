#include "Client.h"
#include "Logger.h"
#include "DrawingUtil.h"
#include "PacketUtil.h"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <pthread.h>

#define COL1 128
#define COL2 300

#define ROW1 8
#define ROW2 64
#define ROW3 128

using namespace trickfire;
using namespace sf;
using namespace std;

double forwards = 0.5;
double rotation = -0.25;

void DrawTrickFireHeader(Font& font, RenderWindow& window) {
	Text header;
	header.setFont(font);
	header.setCharacterSize(60);
	header.setColor(Color::Green);
	header.setStyle(Text::Italic);
	header.setString("TrickFire Robot Client");
	header.setOrigin(0, 2 * header.getLocalBounds().height);
	header.setRotation(90);
	window.draw(header);
}

void * WindowThread(void * cl) {
	Client * client = (Client *) cl;

	Font wlmCarton;
	if (!wlmCarton.loadFromFile("wlm_carton.ttf")) {
		cerr << "Error loading font" << endl;
	}

	RenderWindow window(VideoMode(500, 768), "TrickFire Robotics - Client");

	while (window.isOpen()) {
		window.clear(Color::Black);

		Event event;

		while (window.pollEvent(event)) {
			if (event.type == Event::Closed) {
				window.close();
			}
		}

		DrawTrickFireHeader(wlmCarton, window);

		Color background(64, 64, 64);

		Vector2f forwLabelSize = DrawingUtil::DrawGenericHeader("Rot.",
				Vector2f(COL1, ROW1), false, wlmCarton, Color::Green, window);
		DrawingUtil::DrawCenteredAxisBar(rotation,
				Vector2f(COL1 + (forwLabelSize.x / 2) - 20, ROW2),
				Vector2f(40, 264), Vector2f(4, 4), background, Color::Green,
				window);

		Vector2f rotLabelSize = DrawingUtil::DrawGenericHeader("Forw.",
				Vector2f(COL2, ROW1), false, wlmCarton, Color::Green, window);
		DrawingUtil::DrawCenteredAxisBar(forwards,
				Vector2f(COL2 + (rotLabelSize.x / 2) - 20, ROW2),
				Vector2f(40, 264), Vector2f(4, 4), background, Color::Green,
				window);

		window.display();
	}

	return NULL;
}

void ClientMessageCallback(Packet& packet) {
	int type;
	packet >> type;

	switch (type) {
	case DRIVE_PACKET:
		double drive;
		double rotate;
		packet >> drive >> rotate;

		forwards = drive;
		rotation = rotate;

		break;
	}
}

int main() {
	Logger::SetLoggingLevel(Logger::LEVEL_INFO_VERY_FINE);

	Client client("127.0.0.1", 25565);
	client.SetMessageCallback(ClientMessageCallback);

	pthread_t windowThread;
	pthread_create(&windowThread, NULL, WindowThread, (void *) &client);

	pthread_join(windowThread, NULL);

	client.Disconnect();

	return 0;
}
