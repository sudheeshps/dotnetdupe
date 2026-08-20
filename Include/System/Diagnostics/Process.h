#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/String.h"
#include "System/SmartPointer.h"
#include "System/Array.h"

namespace DotNetDupe {
    namespace System {
        namespace Diagnostics {

            class ProcessStartInfo : public Object {
            public:
                DOTNETDUPE_API ProcessStartInfo();
                DOTNETDUPE_API ProcessStartInfo(const String& sFileName);
                DOTNETDUPE_API ProcessStartInfo(const String& sFileName, const String& sArguments);

                String FileName;
                String Arguments;
                String WorkingDirectory;
                bool CreateNoWindow;
                bool UseShellExecute;
            };

            class Process : public Object {
            public:
                DOTNETDUPE_API Process();
                DOTNETDUPE_API Process(int iId, const String& sProcessName, void* pProcessHandle = nullptr);
                DOTNETDUPE_API virtual ~Process();

                DOTNETDUPE_API ProcessStartInfo GetStartInfo() const { return m_objStartInfo; }
                DOTNETDUPE_API void SetStartInfo(const ProcessStartInfo& objStartInfo) { m_objStartInfo = objStartInfo; }

                DOTNETDUPE_API bool Start();
                DOTNETDUPE_API static SmartPointer<Process> Start(const String& sFileName);
                DOTNETDUPE_API static SmartPointer<Process> Start(const String& sFileName, const String& sArguments);
                DOTNETDUPE_API static SmartPointer<Process> Start(const ProcessStartInfo& objStartInfo);

                DOTNETDUPE_API void WaitForExit();
                DOTNETDUPE_API bool WaitForExit(int iMilliseconds);

                DOTNETDUPE_API int GetExitCode() const { return m_iExitCode; }
                DOTNETDUPE_API bool GetHasExited() const;
                DOTNETDUPE_API int GetId() const { return m_iId; }
                DOTNETDUPE_API String GetProcessName() const { return m_sProcessName; }
                DOTNETDUPE_API static int GetCurrentProcessId();
                DOTNETDUPE_API static SmartPointer<Process> GetCurrentProcess();

                DOTNETDUPE_API static Array<SmartPointer<Process>> GetProcesses();
                DOTNETDUPE_API static SmartPointer<Process> GetProcessById(int iProcessId);
                DOTNETDUPE_API static Array<SmartPointer<Process>> GetProcessesByName(const String& sProcessName);

                DOTNETDUPE_API void Kill();

            private:
                ProcessStartInfo m_objStartInfo;
                int m_iId;
                String m_sProcessName;
                mutable int m_iExitCode;
                mutable bool m_bHasExited;
                void* m_pProcessHandle;

                void Refresh() const;
            };
        }
    }
}
