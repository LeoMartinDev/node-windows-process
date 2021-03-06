#include "Helpers.h"

namespace Helpers
{

std::vector<RawProcess *> *GetProcessesByName(const std::string &name)
{
    try
    {
        String ^ processName = gcnew String(name.c_str());
        array<Diagnostics::Process ^> ^ processesArray = Diagnostics::Process::GetProcessesByName(processName);
        std::vector<RawProcess *> *processesVector = new std::vector<RawProcess *>();

        for (unsigned int i = 0; i < processesArray->Length; i++)
        {
            try
            {
                std::string mainWindowTitle = msclr::interop::marshal_as<std::string>(processesArray[i]->MainWindowTitle);
                RawProcess *result = new RawProcess({
                    processesArray[i]->Handle.ToInt32(),
                    processesArray[i]->Id,
                    mainWindowTitle,
                    processesArray[i]->MainWindowHandle.ToInt32(),
                });
                processesVector->push_back(result);
            }
            catch (InvalidOperationException ^ error)
            {
            }
        }
        return processesVector;
    }
    catch (ArgumentException ^ error)
    {
        throw std::invalid_argument("Cannot find processes");
    }
    catch (InvalidOperationException ^ error)
    {
        throw std::range_error("Process has been closed during access!");
    }
}

RawProcess *GetCurrentProcess()
{
    Diagnostics::Process ^ currentProcess = Diagnostics::Process::GetCurrentProcess();
    std::string mainWindowTitle = msclr::interop::marshal_as<std::string>(currentProcess->MainWindowTitle);
    RawProcess *result = new RawProcess({
        currentProcess->Handle.ToInt32(),
        currentProcess->Id,
        mainWindowTitle,
        currentProcess->MainWindowHandle.ToInt32(),
    });

    return result;
}

RawProcess *GetProcessById(const int id)
{
    try
    {
        Diagnostics::Process ^ currentProcess = Diagnostics::Process::GetProcessById(id);

        std::string mainWindowTitle = msclr::interop::marshal_as<std::string>(currentProcess->MainWindowTitle);
        RawProcess *result = new RawProcess({
            currentProcess->Handle.ToInt32(),
            currentProcess->Id,
            mainWindowTitle,
            currentProcess->MainWindowHandle.ToInt32(),
        });

        return result;
    }
    catch (ArgumentException ^ error)
    {
        throw std::invalid_argument("Cannot find process for id " + std::to_string(id));
    }
    catch (InvalidOperationException ^ error)
    {
        throw std::range_error("Process has been closed during access!");
    }
}
} // namespace Helpers
// Created by leoma on 09/03/2018.
//
