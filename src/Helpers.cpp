#include "Helpers.h"

namespace Helpers {

    std::vector<RawProcess*>* GetProcessesByName(const std::string &name) {
        String ^ processName = gcnew String(name.c_str());
        array < Diagnostics::Process ^>^processesArray = Diagnostics::Process::GetProcessesByName(processName);
        std::vector <RawProcess*>* processesVector = new std::vector<RawProcess*>();

        for (unsigned int i = 0; i < processesArray->Length; i++) {
            std::string mainWindowTitle = msclr::interop::marshal_as<std::string>(processesArray[i]->MainWindowTitle);
            RawProcess* result = new RawProcess({
                    processesArray[i]->Handle.ToInt32(),
                    processesArray[i]->Id,
                    mainWindowTitle,
                    processesArray[i]->MainWindowHandle.ToInt32(),
            });
            processesVector->push_back(result);
        }
        return processesVector;
    }

    RawProcess* GetCurrentProcess() {
        Diagnostics::Process ^currentProcess = Diagnostics::Process::GetCurrentProcess();
        std::string mainWindowTitle = msclr::interop::marshal_as<std::string>(currentProcess->MainWindowTitle);
        RawProcess* result = new RawProcess({
                currentProcess->Handle.ToInt32(),
                currentProcess->Id,
                mainWindowTitle,
                currentProcess->MainWindowHandle.ToInt32(),
        });

        return result;
    }

    RawProcess* GetProcessById(const int id) {
        try {
            Diagnostics::Process ^currentProcess = Diagnostics::Process::GetProcessById(id);

            std::string mainWindowTitle = msclr::interop::marshal_as<std::string>(currentProcess->MainWindowTitle);
            RawProcess* result = new RawProcess({
                    currentProcess->Handle.ToInt32(),
                    currentProcess->Id,
                    mainWindowTitle,
                    currentProcess->MainWindowHandle.ToInt32(),
            });

            return result;
        } catch (ArgumentException^error) {
            throw std::invalid_argument("Cannot find process for id " + std::to_string(id));
        }
    }
}//
// Created by leoma on 09/03/2018.
//

