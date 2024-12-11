// This C++ header file was generated by the TTCN-3 compiler
// of the TTCN-3 Test Executor version 11.0.0
// for hyconsoft_rnd (hyconsoft_rnd@hyconsoft) on Wed Dec 11 13:55:00 2024

// Copyright (c) 2000-2024 Ericsson Telecom AB

// Do not edit this file unless you know what you are doing.

#ifndef PipeTest_HH
#define PipeTest_HH

#ifdef TITAN_RUNTIME_2
#error Generated code does not match with used runtime.\
 Code was generated without -R option but -DTITAN_RUNTIME_2 was used.
#endif

/* Header file includes */

#include <TTCN3.hh>
#include "PIPEasp_PortType.hh"
#include "PIPEasp_Templates.hh"

#if TTCN3_VERSION != 110000
#error Version mismatch detected.\
 Please check the version of the TTCN-3 compiler and the base library.\
 Run make clean and rebuild the project if the version of the compiler changed recently.
#endif

#ifndef LINUX
#error This file should be compiled on LINUX
#endif

namespace PipeTest {

/* Type definitions */

typedef COMPONENT PIPE__CT;
typedef COMPONENT_template PIPE__CT_template;

/* Function prototypes */

extern alt_status handle__default_instance();
extern void handle__default();
extern Default_Base *activate_handle__default();
extern verdicttype testcase_TC__basic(boolean has_timer, double timer_value);
extern verdicttype testcase_TC__executeSomeUnixCommands(boolean has_timer, double timer_value);
extern void WindowNotice(const CHARSTRING& pl__notice);
extern void start_WindowNotice(const COMPONENT& component_reference, const CHARSTRING& pl__notice);
extern void WindowQuestionString(const CHARSTRING& pl__question, CHARSTRING& pl__answer);
extern void start_WindowQuestionString(const COMPONENT& component_reference, const CHARSTRING& pl__question, CHARSTRING& pl__answer);
extern void WindowQuestionYesNo(const CHARSTRING& pl__question, BOOLEAN& pl__answer);
extern void start_WindowQuestionYesNo(const COMPONENT& component_reference, const CHARSTRING& pl__question, BOOLEAN& pl__answer);
extern verdicttype testcase_TC__TrySomeWindows(boolean has_timer, double timer_value);
extern verdicttype testcase_TC__simple(boolean has_timer, double timer_value);
extern verdicttype testcase_TC__unmap(boolean has_timer, double timer_value);
extern void sleep(const FLOAT& seconds);
extern verdicttype testcase_TC__SW__Config__01(boolean has_timer, double timer_value);
extern verdicttype testcase_TC__SW__Config__02(boolean has_timer, double timer_value);
extern verdicttype testcase_TC__Authorization__01(boolean has_timer, double timer_value);
extern verdicttype testcase_TC__Authorization__02(boolean has_timer, double timer_value);
extern verdicttype testcase_TC__Package__01(boolean has_timer, double timer_value);
extern verdicttype testcase_TC__Package__02(boolean has_timer, double timer_value);
extern verdicttype testcase_TC__Package__03(boolean has_timer, double timer_value);
extern verdicttype testcase_TC__Node__01(boolean has_timer, double timer_value);
extern verdicttype testcase_TC__Node__02(boolean has_timer, double timer_value);
extern verdicttype testcase_TC__individual__01(boolean has_timer, double timer_value);
extern verdicttype testcase_TC__individual__02(boolean has_timer, double timer_value);
extern verdicttype testcase_TC__Network__01(boolean has_timer, double timer_value);
extern verdicttype testcase_TC__Network__02(boolean has_timer, double timer_value);
extern verdicttype testcase_TC__Injection__01(boolean has_timer, double timer_value);
extern verdicttype testcase_TC__VehicleMessage__01(boolean has_timer, double timer_value);
extern verdicttype testcase_TC__LogVerification__01(boolean has_timer, double timer_value);
extern verdicttype testcase_TC__LogVerification__02(boolean has_timer, double timer_value);
extern verdicttype testcase_TC__Firewall__01(boolean has_timer, double timer_value);
extern verdicttype testcase_TC__Firewall__02(boolean has_timer, double timer_value);
extern verdicttype testcase_TC__Firewall__03(boolean has_timer, double timer_value);
extern verdicttype testcase_TC__CAN__Injection(boolean has_timer, double timer_value);
extern void module_control_part();

/* Global variable declarations */

extern const TTCN_Typedescriptor_t& PIPE__CT_descr_;
extern PIPEasp__PortType::PIPEasp__PT PIPE__CT_component_PIPE__PCO;
extern PIPEasp__PortType::PIPEasp__PT PIPE__CT_component_PIPE__PCO1;
extern PIPEasp__Types::ASP__PExecute PIPE__CT_component_v__ASP__PExecute;
extern PIPEasp__Types::ASP__PResult PIPE__CT_component_v__ASP__PResult;
extern PIPEasp__Types::ASP__PExecuteBinary PIPE__CT_component_v__ASP__PExecuteBinary;
extern PIPEasp__Types::ASP__PResultBinary PIPE__CT_component_v__ASP__PResultBinary;
extern PIPEasp__Types::ASP__PExecuteBackground PIPE__CT_component_v__ASP__PExecuteBackground;
extern PIPEasp__Types::ASP__PStdin PIPE__CT_component_v__ASP__PStdin;
extern PIPEasp__Types::ASP__PStdout PIPE__CT_component_v__ASP__PStdout;
extern PIPEasp__Types::ASP__PStderr PIPE__CT_component_v__ASP__PStderr;
extern PIPEasp__Types::ASP__PStdinBinary PIPE__CT_component_v__ASP__PStdinBinary;
extern PIPEasp__Types::ASP__PStdoutBinary PIPE__CT_component_v__ASP__PStdoutBinary;
extern PIPEasp__Types::ASP__PStderrBinary PIPE__CT_component_v__ASP__PStderrBinary;
extern PIPEasp__Types::ASP__PKill PIPE__CT_component_v__ASP__PKill;
extern PIPEasp__Types::ASP__PExit PIPE__CT_component_v__ASP__PExit;
extern PIPEasp__Types::ASP__PLineMode PIPE__CT_component_v__ASP__PLineMode;
extern PIPEasp__Types::ASP__PError PIPE__CT_component_v__ASP__PError;
extern TTCN_Module module_object;

} /* end of namespace */

#endif