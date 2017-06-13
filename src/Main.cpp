#include "Main.h"

#define COL1 128
#define COL2 300
#define COL3 500

#define ROW1 8
#define ROW2 64
#define ROW3 ROW2 + 264 + 16

# define PI 3.14159265358979323846

namespace trickfire {

Command * drivebase;
StandardDriveCommand standardDrive;

Command * coalMiner;
CoalMinerDigCommand cmDig;
CoalMinerEmptyCommand cmEmpty;

Command * coalMinerS1Move;
CoalMinerS1MoveCommand cmS1Move;
//CoalMinerS1LowerCommand cmS1Lower;

Command * coalMinerS2Move;
CoalMinerS2MoveCommand cmS2Move;
//CoalMinerS2LowerCommand cmS2Lower;

Command * binMove;
BinToDumpCommand binToDump;
BinToDumpCommand binToDumpAuto(3);
BinToFillCommand binToFill;
BinToFillCommand binToFillAuto(3);

Command * conveyor;
ConveyorDumpCommand conveyorDump;

CameraSendCommand * cam0 = nullptr;
CameraSendCommand * cam1 = nullptr;

double zero = 0;

void Main::Start() {
	Logger::SetLoggingLevel(Logger::LEVEL_INFO);
	RobotIO::Start();

	Client client(SERVER_IP, 25565);
	sf::Mutex mut_client;
	client.SetMessageCallback(Main::OnClientMessageReceived);

	drivebase = &standardDrive;
	standardDrive.Start();

#if OPENCV
#if CAMERA
	CameraSendCommand camera0Command(&client, &mut_client, 0);
	cam0 = &camera0Command;
	camera0Command.Start();

	//CameraSendCommand camera1Command(&client, &mut_client, 1);
	//cam1 = &camera1Command;
	//camera1Command.Start();
#endif
#if KINECT
	Freenect::Freenect freenect;

	TFFreenect& temp = freenect.createDevice<TFFreenect>(0);
	temp.startVideo();
	temp.stopVideo();
	freenect.deleteDevice(0);

	TFFreenect& kinect = freenect.createDevice<TFFreenect>(0);
	kinect.setLed(LED_GREEN);
	KinectSendCommand kinectCommand(&client, &mut_client, kinect);
	kinectCommand.Start();
	kinect.setTiltDegrees(0);
	kinect.updateState();
#endif
#endif

#if defined(GUI_ENABLED) and GUI_ENABLED
	sf::Thread windowThread(SfmlWindowThread);
	windowThread.launch();
	windowThread.wait();
#else
	client.Join();
#endif

	drivebase->Stop();
	Command::KillAll();

	client.Disconnect();
	RobotIO::Stop();

#if KINECT
	kinect.stopVideo();
	kinect.stopDepth();
	kinect.setLed(LED_RED);
#endif
}

void Main::ResumeStandardDrive() {
	if (drivebase != &standardDrive) {
		drivebase->Stop();
		drivebase = &standardDrive;
		drivebase->Start();
	}
}

void Main::OnClientMessageReceived(Packet& packet) {
	int type = -1;
	packet >> type;
	switch (type) {
	case DRIVE_PACKET:
		Logger::Log(Logger::LEVEL_INFO_VERY_FINE, "Received drive packet");
		double left;
		double right;
		bool fl, rl, fr, rr;
		packet >> left >> right >> fl >> rl >> fr >> rr;
		//std::cout << "Wheel bools: " << fl << " " << rl << " " << fr << " " << rr << std::endl;
		standardDrive.SetVals(left, right, fl, rl, fr, rr);
		break;
	case MINER_MOVE_S1_PACKET:
		int move_s1_dir;
		double s1L, s1R;
		packet >> move_s1_dir >> s1L >> s1R;
		switch (move_s1_dir) {
		case 0:
			Logger::Log(Logger::LEVEL_INFO_FINE,
					"Received miner S1 move packet (stop)");
			if (coalMinerS1Move != nullptr) {
				coalMinerS1Move->Stop();
			}
			coalMinerS1Move = nullptr;
			break;
		case 1:
		case -1:
			Logger::Log(Logger::LEVEL_INFO_FINE,
					"Received miner S1 move packet");
			std::cout << "Sides: L " << s1L << " R " << s1R << std::endl;
			if (coalMinerS1Move != nullptr) {
				coalMinerS1Move->Stop();
			}
			coalMinerS1Move = &cmS1Move;
			cmS1Move.left = s1L;
			cmS1Move.right = s1R;
			coalMinerS1Move->Start();
			break;
		}
		break;
	case MINER_MOVE_S2_PACKET:
			int move_s2_dir;
			double s2L, s2R;
			packet >> move_s2_dir >> s2L >> s2R;
			switch (move_s2_dir) {
			case 0:
				Logger::Log(Logger::LEVEL_INFO_FINE,
						"Received miner S2 move packet (stop)");
				if (coalMinerS2Move != nullptr) {
					coalMinerS2Move->Stop();
				}
				coalMinerS2Move = nullptr;
				break;
			case 1:
			case -1:
				Logger::Log(Logger::LEVEL_INFO_FINE,
						"Received miner S2 move packet");
				if (coalMinerS2Move != nullptr) {
					coalMinerS2Move->Stop();
				}
				coalMinerS2Move = &cmS2Move;
				cmS2Move.left = s2L;
				cmS2Move.right = s2R;
				coalMinerS2Move->Start();
				break;
			}
			break;
	case MINER_SPIN_PACKET:
		int spin_dir;
		packet >> spin_dir;
		switch (spin_dir) {
		case 0:
			Logger::Log(Logger::LEVEL_INFO_FINE,
					"Received miner spin packet (stop)");
			if (coalMiner != nullptr) {
				coalMiner->Stop();
			}
			coalMiner = nullptr;
			break;
		case 1:
			Logger::Log(Logger::LEVEL_INFO_FINE,
					"Received miner spin packet (dig)");
			if (coalMiner != nullptr) {
				coalMiner->Stop();
			}
			coalMiner = &cmDig;
			coalMiner->Start();
			break;
		case -1:
			Logger::Log(Logger::LEVEL_INFO_FINE,
					"Received miner spin packet (empty)");
			if (coalMiner != nullptr) {
				coalMiner->Stop();
			}
			coalMiner = &cmEmpty;
			coalMiner->Start();
			break;
		}
		break;
	case BIN_SLIDE_PACKET:
		int slide_dir;
		packet >> slide_dir;
		switch (slide_dir) {
		case 0:
			Logger::Log(Logger::LEVEL_INFO_FINE,
					"Received bin slide packet (stop)");
			if (binMove != nullptr) {
				binMove->Stop();
			}
			binMove = nullptr;
			break;
		case -1:
		case -2:
			Logger::Log(Logger::LEVEL_INFO_FINE,
					"Received bin slide packet (slide to fill)");
			if (binMove != nullptr) {
				binMove->Stop();
			}
			binMove = (slide_dir == -2) ? &binToFillAuto : &binToFill;
			binMove->Start();
			break;
		case 1:
		case 2:
			Logger::Log(Logger::LEVEL_INFO_FINE,
					"Received bin slide packet (slide to dump)");
			if (binMove != nullptr) {
				binMove->Stop();
			}
			binMove = (slide_dir == 2) ? &binToDumpAuto : &binToDump;
			binMove->Start();
		}
		break;
	case CONVEYOR_PACKET:
		int dir;
		packet >> dir;
		if (dir != 0) {
			Logger::Log(Logger::LEVEL_INFO_FINE,
					"Received conveyor packet (dump)");
			if (conveyor != nullptr) {
				conveyor->Stop();
			}
			conveyor = &conveyorDump;
			conveyorDump.speed = dir;
			conveyor->Start();
			break;
		} else {
			Logger::Log(Logger::LEVEL_INFO_FINE,
					"Received conveyor packet (stop)");
			if (conveyor != nullptr) {
				conveyor->Stop();
			}
			conveyor = nullptr;
		}
		break;
	case CONVEYOR_PACKET + 1:
		bool camOn;
		packet >> camOn;

		cam0->mut_Transmit.lock();
		cam0->transmit = camOn;
		cam0->mut_Transmit.unlock();
		break;
	/*case CONVEYOR_PACKET + 1:
		double val;
		packet >> val;
		cmS1Lower.mod = val;
		cmS1Move.mod = val;
		break;*/
	default:
		Logger::Log(Logger::LEVEL_INFO_FINE,
				"Received unknown packet (type " + to_string(type) + ")");
		break;
	}
}

void Main::SfmlWindowThread() {
	Font wlmCarton;
	if (!wlmCarton.loadFromFile("wlm_carton.ttf")) {
		Logger::Log(Logger::LEVEL_ERROR,
				"Error loading font: will continue but text will be invisible");
	}

	RenderWindow window(VideoMode(1366, 768), "TrickFire Robotics - Client");

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

		Vector2f forwLabelSize = DrawingUtil::DrawGenericHeader("Left",
				Vector2f(COL1, ROW1), false, wlmCarton, Color::Green, window);
		DrawingUtil::DrawCenteredAxisBar(DisplayVariables::GetLeft(),
				Vector2f(COL1 + (forwLabelSize.x / 2) - 20, ROW2),
				Vector2f(40, 264), Vector2f(4, 4), background, Color::Green,
				window);

		Vector2f rotLabelSize = DrawingUtil::DrawGenericHeader("Right",
				Vector2f(COL2, ROW1), false, wlmCarton, Color::Green, window);
		DrawingUtil::DrawCenteredAxisBar(DisplayVariables::GetRight(),
				Vector2f(COL2 + (rotLabelSize.x / 2) - 20, ROW2),
				Vector2f(40, 264), Vector2f(4, 4), background, Color::Green,
				window);

		float subsystemY = ROW1;

		Vector2f driveOutLabelSize = DrawingUtil::DrawGenericHeader(
				"Drive System: ", Vector2f(COL3, subsystemY), false, wlmCarton,
				Color::Green, window);
		Vector2f driveLabelSize = DrawingUtil::DrawGenericText(
				drivebase->GetCommandName(),
				Vector2f(COL3 + driveOutLabelSize.x, subsystemY + 48 - 36),
				false, wlmCarton, Color::White, window);

		subsystemY += 56;

		Vector2f cmOutLabelSize = DrawingUtil::DrawGenericHeader("CM Spin: ",
				Vector2f(COL3, subsystemY), false, wlmCarton, Color::Green,
				window);
		std::string cmText = "";
		if (coalMiner != nullptr && coalMiner->IsRunning()) {
			cmText = coalMiner->GetCommandName();
		}
		Vector2f cmLabelSize = DrawingUtil::DrawGenericText(cmText,
				Vector2f(COL3 + cmOutLabelSize.x, subsystemY + 48 - 36), false,
				wlmCarton, Color::White, window);

		subsystemY += 56;

		Vector2f cmMoveS1OutLabelSize = DrawingUtil::DrawGenericHeader(
				"CM Move S1: ", Vector2f(COL3, subsystemY), false, wlmCarton,
				Color::Green, window);
		std::string cmMoveS1Text = "";
		if (coalMinerS1Move != nullptr && coalMinerS1Move->IsRunning()) {
			cmMoveS1Text = coalMinerS1Move->GetCommandName();
		}
		Vector2f cmMoveS1LabelSize = DrawingUtil::DrawGenericText(cmMoveS1Text,
				Vector2f(COL3 + cmMoveS1OutLabelSize.x, subsystemY + 48 - 36),
				false, wlmCarton, Color::White, window);

		subsystemY += 56;

		Vector2f cmMoveS2OutLabelSize = DrawingUtil::DrawGenericHeader(
				"CM Move S2: ", Vector2f(COL3, subsystemY), false, wlmCarton,
				Color::Green, window);
		std::string cmMoveS2Text = "";
		if (coalMinerS2Move != nullptr && coalMinerS2Move->IsRunning()) {
			cmMoveS2Text = coalMinerS2Move->GetCommandName();
		}
		Vector2f cmMoveS2LabelSize = DrawingUtil::DrawGenericText(cmMoveS2Text,
				Vector2f(COL3 + cmMoveS2OutLabelSize.x, subsystemY + 48 - 36),
				false, wlmCarton, Color::White, window);

		subsystemY += 56;

		Vector2f binOutLabelSize = DrawingUtil::DrawGenericHeader("Bin Move: ",
				Vector2f(COL3, subsystemY), false, wlmCarton, Color::Green,
				window);
		std::string binMoveText = "";
		if (binMove != nullptr && binMove->IsRunning()) {
			binMoveText = binMove->GetCommandName();
		}
		Vector2f binLabelSize = DrawingUtil::DrawGenericText(binMoveText,
				Vector2f(COL3 + binOutLabelSize.x, subsystemY + 48 - 36), false,
				wlmCarton, Color::White, window);

		subsystemY += 56;

		Vector2f conveyorOutLabelSize = DrawingUtil::DrawGenericHeader(
				"Conveyor: ", Vector2f(COL3, subsystemY), false, wlmCarton,
				Color::Green, window);
		std::string conveyorText = "";
		if (conveyor != nullptr && conveyor->IsRunning()) {
			conveyorText = conveyor->GetCommandName();
		}
		Vector2f conveyorLabelSize = DrawingUtil::DrawGenericText(conveyorText,
				Vector2f(COL3 + conveyorOutLabelSize.x, subsystemY + 48 - 36), false,
				wlmCarton, Color::White, window);

		sf::VertexArray line = sf::VertexArray(sf::Lines, 2);
		line[0].position = Vector2f(250, (768 * 0.75));

		// double radYaw = (RobotIO::GetGyroYaw() - gyroDrive.GetZero()) * PI / 180;
		double radYaw = (RobotIO::GetGyroYaw() * PI / 180);
		line[1].position = Vector2f(line[0].position.x + (100 * cos(radYaw)),
				line[0].position.y + (100 * sin(radYaw)));
		line[0].color = Color::Green;
		line[1].color = Color::Green;

		window.draw(line);

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
