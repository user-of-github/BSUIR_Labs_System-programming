#include <windows.h> 
#include <vector>
#include <iostream>


struct ProcessWrapper
{
	STARTUPINFO startup_info {};
	PROCESS_INFORMATION process_information {};
};


int main()
{
	std::vector<ProcessWrapper> processes{};

	char choice{};

	while (choice != '0') {
		std::cout << "Print 0 to quit, 1 - to create process, 2 - terminate process\n";

		std::cin >> choice;

		if (choice == '1') {
			std::string executable_module_name{};
			std::cout << "Input executable process name:\n";
			std::cin >> executable_module_name;
			
			STARTUPINFO si{};
			PROCESS_INFORMATION pi{};

			ZeroMemory(&si, sizeof(STARTUPINFO));

			si.cb = sizeof(STARTUPINFO);

			if (!CreateProcess(
				executable_module_name.c_str(), 
				NULL, 
				NULL, 
				NULL, 
				FALSE, 
				CREATE_NEW_CONSOLE, 
				NULL, 
				NULL, 
				&si, 
				&pi))
			{
				std::cout << "Process is not created ! Sorry... Maybe wrong name;\n";
				continue;
			}

			processes.push_back({ si, pi });

			//WaitForSingleObject(pi.hProcess, INFINITE);
			//CloseHandle(pi.hThread);
			//CloseHandle(pi.hProcess);
		}
		else if (choice == '2') {
			std::cout << "Enter process number\n";
			int a{ };
			std::cin >> a;

			if (a >= 0 && a < processes.size()) {
				TerminateProcess(processes.at(a).process_information.hProcess, 1);
			}
			else {
				std::cout << "Invalid value\n";
			}
		}
		else if (choice == '0') {
			break;
		}
	}

 return 0;
}