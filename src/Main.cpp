#include "Main.h"

#define GUI_ENABLED true

#define COL1 128
#define COL2 300

#define ROW1 8
#define ROW2 64
#define ROW3 128

namespace trickfire {
double Main::forwards;
double Main::rotation;

Command * comm;
TestCommand2 tc2;

void Main::Start() {
	RobotIO::Start();

	Client client("127.0.0.1", 25565);
	client.SetMessageCallback(Main::OnClientMessageReceived);

	TestCommand tcomm;
	comm = &tcomm;
	comm->Start();

#if defined(GUI_ENABLED) and GUI_ENABLED
	sf::Thread windowThread(SfmlWindowThread);
	windowThread.launch();
	windowThread.wait();
#else
	client.Join();
#endif

	comm->Stop();

	client.Disconnect();
	RobotIO::Stop();
}

void Main::OnClientMessageReceived(Packet& packet) {
	int type = -1;
	packet >> type;
	switch (type) {
	case DRIVE_PACKET:
		packet >> forwards >> rotation;
		RobotIO::SimpleArcade(forwards, rotation);

		cout << "Driving: " << forwards << ", " << rotation << endl;
		break;
	case AUTO_PACKET_1:
		comm->Stop();
		cout << "Starting auto command" << endl;
		comm = &tc2;
		comm->Start();
		break;
	}
}

void Main::SfmlWindowThread() {
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
}

void Main::DrawTrickFireHeader(Font& font, RenderWindow& window) {
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
}
