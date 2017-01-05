#include "Driver.h"

#define UI_ENABLED true

#define COL1 128
#define COL2 300

#define ROW1 8
#define ROW2 64
#define ROW3 128

namespace trickfire {

double Driver::forwards;
double Driver::rotation;

//StandardDriveCommand Driver::standardDrive(&forwards, &rotation);
Command * Driver::driveCommand;

void Driver::OnNonDefaultDriveFinish() {
	StandardDriveCommand stan(&forwards, &rotation);
	driveCommand = &stan;
	driveCommand->Start();
}

void Driver::SfmlWindowThread(Client * client) {
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

void Driver::ClientMessageCallback(Packet& packet) {
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
	case AUTO_PACKET_1:
		driveCommand->Stop();
		TestOverrideCommand comm;
		driveCommand = &comm;
		comm.Start();

		break;
	}
}

void Driver::DrawTrickFireHeader(Font& font, RenderWindow& window) {
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

void Driver::Start() {
	Logger::SetLoggingLevel(Logger::LEVEL_INFO_FINE);

	Client client("127.0.0.1", 25565);
	client.SetMessageCallback(ClientMessageCallback);

#if defined(UI_ENABLED) and UI_ENABLED
	sf::Thread windowThread(&Driver::SfmlWindowThread, &client);
	windowThread.launch();
#endif

	RobotIO::Start();

	StandardDriveCommand stan(&forwards, &rotation);
	driveCommand = &stan;
	driveCommand->Start();

#if defined(UI_ENABLED) and UI_ENABLED
	windowThread.wait();
#else
	client.Join();
#endif
	client.Disconnect();

	RobotIO::Stop();
	Command::StopAllCommands();
}
}
