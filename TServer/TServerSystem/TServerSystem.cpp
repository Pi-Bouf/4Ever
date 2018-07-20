#include "TServerSystem.h"

#pragma region Constructor/Destructor

TServerSystem::TServerSystem(string _serverName, string _serverVersion)
{
	ServerName = _serverName;
	transform(ServerName.begin(), ServerName.end(), ServerName.begin(), (int(*)(int))toupper);
	ServerNameOriginal = _serverName;
	ServerVersion = _serverVersion;
	ConsoleTitle = ServerName + " Server";

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	string iniLocation = ".\\Configurations\\" + ServerNameOriginal + ".ini";
	cSimpleIni.SetUnicode();
	cSimpleIni.LoadFile(iniLocation.c_str());

	SetConsoleTitle(ConsoleTitle.c_str());
	DisplayPresentation();
}

TServerSystem::~TServerSystem()
{
}

#pragma endregion

#pragma region Textual interaction

void TServerSystem::DisplayPresentation()
{
	SetConsoleTextAttribute(hConsole, PRESENTATION_COLOR);
	cout << endl;
	cout << " #################################################################### " << endl;
	cout << " #         .o    .oooooo..o     .                                   # " << endl;
	cout << " #       .d88   d8P'    `Y8   .o8                                   # " << endl;
	cout << " #     .d'888   Y88bo.      .o888oo  .ooooo.  oooo d8b oooo    ooo  # " << endl;
	cout << " #   .d'  888    `\"Y8888o.    888   d88' `88b `888\"\"8P  `88.  .8'   # " << endl;
	cout << " #   88ooo888oo      `\"Y88b   888   888   888  888       `88..8'    # " << endl;
	cout << " #        888   oo     .d8P   888 . 888   888  888        `888'     # " << endl;
	cout << " #       o888o  8\"\"88888P'    \"888\" `Y8bod8P' d888b        .8'      # " << endl;
	SetConsoleTextAttribute(hConsole, SERVER_NAME_COLOR);
	cout << " #       " << ServerName << " SERVER";
	SetConsoleTextAttribute(hConsole, PRESENTATION_COLOR);
	for (int i = 0; i < 39 - ServerName.length(); i++)
	{
		cout << " ";
	}
	cout << ".o..P'       # " << endl;
	SetConsoleTextAttribute(hConsole, VERSION_COLOR);
	cout << " #       VERSION: " << ServerVersion;
	SetConsoleTextAttribute(hConsole, PRESENTATION_COLOR);
	for (int i = 0; i < 37 - ServerVersion.length(); i++)
	{
		cout << " ";
	}
	cout << "`Y8P'        # " << endl;
	cout << " #################################################################### " << endl << endl;

	LogInfo("Welcome on " + ServerName + " SERVER !", 0);
}

tm* TServerSystem::GetLocalTime()
{
	time_t timeNow = time(0);
	tm *ltm = localtime(&timeNow);

	return ltm;
}

void TServerSystem::LogInfo(string text, int spaceBefore)
{
	LogSpace(spaceBefore);
	tm* ltm = GetLocalTime();
	SetConsoleTextAttribute(hConsole, LOG_INFO_COLOR);
	cout << "[" << ltm->tm_hour << ":" << ltm->tm_min << ":" << ltm->tm_sec << "] ==> " << text << endl;
}

void TServerSystem::LogInfo(char* text, int spaceBefore)
{
	LogInfo(string(text), spaceBefore);
}

void TServerSystem::LogEvent(const char *fmt, ...)
{
	char textString[1024 * 5] = { '\0' };

	// -- Empty the buffer properly to ensure no leaks.
	memset(textString, '\0', sizeof(textString));

	va_list args;
	va_start(args, fmt);
	vsnprintf(textString, 1024 * 5, fmt, args);
	va_end(args);
	std::string retStr = textString;

	tm* ltm = GetLocalTime();
	SetConsoleTextAttribute(hConsole, LOG_EVENT_COLOR);
	cout << "[" << ltm->tm_hour << ":" << ltm->tm_min << ":" << ltm->tm_sec << "] (LOG EVENT) ==> " << retStr.c_str() << endl;
}

void TServerSystem::LogError(string text, int spaceBefore)
{
	LogSpace(spaceBefore);
	tm* ltm = GetLocalTime();
	SetConsoleTextAttribute(hConsole, LOG_ERROR_COLOR);
	cout << "[" << ltm->tm_hour << ":" << ltm->tm_min << ":" << ltm->tm_sec << "] ==> " << text << endl;
}

void TServerSystem::LogSpace(int space)
{
	for (int i = 0; i < space; i++)
	{
		cout << endl;
	}
}

void TServerSystem::DisplayIni()
{
	LogInfo("Loaded configuration from " + ServerNameOriginal + ".ini:", 0);
	SetConsoleTextAttribute(hConsole, INI_PRESENTATION_COLOR);
	cout << "  ##########################################" << endl;

	for (int i = 0; i < serverConfig.size(); ++i)
	{
		vector <string> currentConf = serverConfig.at(i);
		SetConsoleTextAttribute(hConsole, INI_PRESENTATION_COLOR);
		cout << "  # + " << currentConf.at(0);
		for (int i = 0; i < 13 - currentConf.at(0).length(); i++)
		{
			cout << " ";
		}
		SetConsoleTextAttribute(hConsole, INI_VALUES_COLOR);
		cout << currentConf.at(1) << endl;
	}

	SetConsoleTextAttribute(hConsole, INI_PRESENTATION_COLOR);
	cout << "  ##########################################" << endl;
}

#pragma endregion

#pragma region Ini system

void TServerSystem::LoadIntFromIni(string confName, int defaultValue, int* ref)
{
	string namespaceIni = ServerNameOriginal + "Config";
	*ref = stoi(cSimpleIni.GetValue(namespaceIni.c_str(), confName.c_str(), to_string(defaultValue).c_str()));
	vector <string> data;
	data.push_back(confName);
	data.push_back(to_string(*ref));

	serverConfig.push_back(data);
}

void TServerSystem::LoadStringFromIni(string confName, string defaultValue, string* ref)
{
	string namespaceIni = ServerNameOriginal + "Config";
	*ref = cSimpleIni.GetValue(namespaceIni.c_str(), confName.c_str(), defaultValue.c_str());
	vector <string> data;
	data.push_back(confName);
	data.push_back(*ref);

	serverConfig.push_back(data);
}

#pragma endregion

#pragma region Starting server

DWORD TServerSystem::OnEnter()
{
	return OnEnter();
}


void TServerSystem::StartServer()
{
	LogInfo("Starting up...", 0);

	DWORD result = OnEnter();
	if (result) {
		StartupError(result);
	}
}

void TServerSystem::StartupError(DWORD result)
{
	LogError("Starup error: ", 2);
	switch (result) {
	case EC_INITSERVICE_DBOPENFAILED: {
		LogError("Can't connect to the database !", 0);
		LogError("1) Check ODBC x32 or x64, if all is configured", 0);
		LogError("2) Bad credentials in your .ini configuration file", 0);
		LogError("3) Check if you SQL Server is running !", 0);

		break;
	}
	default:
		LogError("Unknown error !", 0);
	}

	LogError("Can't start the server...", 2);
}
#pragma endregion