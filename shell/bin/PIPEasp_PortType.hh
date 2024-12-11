// This C++ header file was generated by the TTCN-3 compiler
// of the TTCN-3 Test Executor version 11.0.0
// for hyconsoft_rnd (hyconsoft_rnd@hyconsoft) on Wed Dec 11 13:55:00 2024

// Copyright (c) 2000-2024 Ericsson Telecom AB

// Do not edit this file unless you know what you are doing.

#ifndef PIPEasp__PortType_HH
#define PIPEasp__PortType_HH

#ifdef TITAN_RUNTIME_2
#error Generated code does not match with used runtime.\
 Code was generated without -R option but -DTITAN_RUNTIME_2 was used.
#endif

/* Header file includes */

#include <TTCN3.hh>
#include "PIPEasp_Types.hh"

#if TTCN3_VERSION != 110000
#error Version mismatch detected.\
 Please check the version of the TTCN-3 compiler and the base library.\
 Run make clean and rebuild the project if the version of the compiler changed recently.
#endif

#ifndef LINUX
#error This file should be compiled on LINUX
#endif

#undef PIPEasp__PortType_HH
#endif

namespace PIPEasp__PortType {

/* Forward declarations of classes */

class PIPEasp__PT_BASE;
class PIPEasp__PT;

} /* end of namespace */

#ifndef PIPEasp__PortType_HH
#define PIPEasp__PortType_HH

namespace PIPEasp__PortType {

/* Class definitions */

class PIPEasp__PT_BASE : public PORT {
enum msg_selection { MESSAGE_0, MESSAGE_1, MESSAGE_2, MESSAGE_3, MESSAGE_4, MESSAGE_5, MESSAGE_6, MESSAGE_7, MESSAGE_8 };
struct msg_queue_item : public msg_queue_item_base {
msg_selection item_selection;
union {
PIPEasp__Types::ASP__PResult *message_0;
PIPEasp__Types::ASP__PResultBinary *message_1;
PIPEasp__Types::ASP__PStdout *message_2;
PIPEasp__Types::ASP__PStderr *message_3;
PIPEasp__Types::ASP__PStdoutBinary *message_4;
PIPEasp__Types::ASP__PStderrBinary *message_5;
PIPEasp__Types::ASP__PExit *message_6;
PIPEasp__Types::ASP__PError *message_7;
PIPEasp__Types::ASP__Parrallel__Result *message_8;
};
component sender_component;
};

void remove_msg_queue_head();
protected:
void clear_queue();
public:
PIPEasp__PT_BASE(const char *par_port_name);
~PIPEasp__PT_BASE();
void send(const PIPEasp__Types::ASP__PExecute& send_par, const COMPONENT& destination_component, FLOAT* timestamp_redirect = NULL);
void send(const PIPEasp__Types::ASP__PExecute& send_par, FLOAT* timestamp_redirect = NULL);
void send(const PIPEasp__Types::ASP__PExecute_template& send_par, const COMPONENT& destination_component, FLOAT* timestamp_redirect = NULL);
void send(const PIPEasp__Types::ASP__PExecute_template& send_par, FLOAT* timestamp_redirect = NULL);
void send(const PIPEasp__Types::ASP__PExecutePty& send_par, const COMPONENT& destination_component, FLOAT* timestamp_redirect = NULL);
void send(const PIPEasp__Types::ASP__PExecutePty& send_par, FLOAT* timestamp_redirect = NULL);
void send(const PIPEasp__Types::ASP__PExecutePty_template& send_par, const COMPONENT& destination_component, FLOAT* timestamp_redirect = NULL);
void send(const PIPEasp__Types::ASP__PExecutePty_template& send_par, FLOAT* timestamp_redirect = NULL);
void send(const PIPEasp__Types::ASP__PExecuteBinary& send_par, const COMPONENT& destination_component, FLOAT* timestamp_redirect = NULL);
void send(const PIPEasp__Types::ASP__PExecuteBinary& send_par, FLOAT* timestamp_redirect = NULL);
void send(const PIPEasp__Types::ASP__PExecuteBinary_template& send_par, const COMPONENT& destination_component, FLOAT* timestamp_redirect = NULL);
void send(const PIPEasp__Types::ASP__PExecuteBinary_template& send_par, FLOAT* timestamp_redirect = NULL);
void send(const PIPEasp__Types::ASP__PExecuteBinaryPty& send_par, const COMPONENT& destination_component, FLOAT* timestamp_redirect = NULL);
void send(const PIPEasp__Types::ASP__PExecuteBinaryPty& send_par, FLOAT* timestamp_redirect = NULL);
void send(const PIPEasp__Types::ASP__PExecuteBinaryPty_template& send_par, const COMPONENT& destination_component, FLOAT* timestamp_redirect = NULL);
void send(const PIPEasp__Types::ASP__PExecuteBinaryPty_template& send_par, FLOAT* timestamp_redirect = NULL);
void send(const PIPEasp__Types::ASP__PExecuteBackground& send_par, const COMPONENT& destination_component, FLOAT* timestamp_redirect = NULL);
void send(const PIPEasp__Types::ASP__PExecuteBackground& send_par, FLOAT* timestamp_redirect = NULL);
void send(const PIPEasp__Types::ASP__PExecuteBackground_template& send_par, const COMPONENT& destination_component, FLOAT* timestamp_redirect = NULL);
void send(const PIPEasp__Types::ASP__PExecuteBackground_template& send_par, FLOAT* timestamp_redirect = NULL);
void send(const PIPEasp__Types::ASP__PExecuteBackgroundPty& send_par, const COMPONENT& destination_component, FLOAT* timestamp_redirect = NULL);
void send(const PIPEasp__Types::ASP__PExecuteBackgroundPty& send_par, FLOAT* timestamp_redirect = NULL);
void send(const PIPEasp__Types::ASP__PExecuteBackgroundPty_template& send_par, const COMPONENT& destination_component, FLOAT* timestamp_redirect = NULL);
void send(const PIPEasp__Types::ASP__PExecuteBackgroundPty_template& send_par, FLOAT* timestamp_redirect = NULL);
void send(const PIPEasp__Types::ASP__PStdin& send_par, const COMPONENT& destination_component, FLOAT* timestamp_redirect = NULL);
void send(const PIPEasp__Types::ASP__PStdin& send_par, FLOAT* timestamp_redirect = NULL);
void send(const PIPEasp__Types::ASP__PStdin_template& send_par, const COMPONENT& destination_component, FLOAT* timestamp_redirect = NULL);
void send(const PIPEasp__Types::ASP__PStdin_template& send_par, FLOAT* timestamp_redirect = NULL);
void send(const PIPEasp__Types::ASP__PStdinBinary& send_par, const COMPONENT& destination_component, FLOAT* timestamp_redirect = NULL);
void send(const PIPEasp__Types::ASP__PStdinBinary& send_par, FLOAT* timestamp_redirect = NULL);
void send(const PIPEasp__Types::ASP__PStdinBinary_template& send_par, const COMPONENT& destination_component, FLOAT* timestamp_redirect = NULL);
void send(const PIPEasp__Types::ASP__PStdinBinary_template& send_par, FLOAT* timestamp_redirect = NULL);
void send(const PIPEasp__Types::ASP__PKill& send_par, const COMPONENT& destination_component, FLOAT* timestamp_redirect = NULL);
void send(const PIPEasp__Types::ASP__PKill& send_par, FLOAT* timestamp_redirect = NULL);
void send(const PIPEasp__Types::ASP__PKill_template& send_par, const COMPONENT& destination_component, FLOAT* timestamp_redirect = NULL);
void send(const PIPEasp__Types::ASP__PKill_template& send_par, FLOAT* timestamp_redirect = NULL);
void send(const PIPEasp__Types::ASP__PLineMode& send_par, const COMPONENT& destination_component, FLOAT* timestamp_redirect = NULL);
void send(const PIPEasp__Types::ASP__PLineMode& send_par, FLOAT* timestamp_redirect = NULL);
void send(const PIPEasp__Types::ASP__PLineMode_template& send_par, const COMPONENT& destination_component, FLOAT* timestamp_redirect = NULL);
void send(const PIPEasp__Types::ASP__PLineMode_template& send_par, FLOAT* timestamp_redirect = NULL);
void send(const PIPEasp__Types::ASP__PEndOfInput& send_par, const COMPONENT& destination_component, FLOAT* timestamp_redirect = NULL);
void send(const PIPEasp__Types::ASP__PEndOfInput& send_par, FLOAT* timestamp_redirect = NULL);
void send(const PIPEasp__Types::ASP__PEndOfInput_template& send_par, const COMPONENT& destination_component, FLOAT* timestamp_redirect = NULL);
void send(const PIPEasp__Types::ASP__PEndOfInput_template& send_par, FLOAT* timestamp_redirect = NULL);
void send(const PIPEasp__Types::ASP__Parallel__Command& send_par, const COMPONENT& destination_component, FLOAT* timestamp_redirect = NULL);
void send(const PIPEasp__Types::ASP__Parallel__Command& send_par, FLOAT* timestamp_redirect = NULL);
void send(const PIPEasp__Types::ASP__Parallel__Command_template& send_par, const COMPONENT& destination_component, FLOAT* timestamp_redirect = NULL);
void send(const PIPEasp__Types::ASP__Parallel__Command_template& send_par, FLOAT* timestamp_redirect = NULL);
protected:
virtual void outgoing_send(const PIPEasp__Types::ASP__PExecute& send_par) = 0;
virtual void outgoing_send(const PIPEasp__Types::ASP__PExecutePty& send_par) = 0;
virtual void outgoing_send(const PIPEasp__Types::ASP__PExecuteBinary& send_par) = 0;
virtual void outgoing_send(const PIPEasp__Types::ASP__PExecuteBinaryPty& send_par) = 0;
virtual void outgoing_send(const PIPEasp__Types::ASP__PExecuteBackground& send_par) = 0;
virtual void outgoing_send(const PIPEasp__Types::ASP__PExecuteBackgroundPty& send_par) = 0;
virtual void outgoing_send(const PIPEasp__Types::ASP__PStdin& send_par) = 0;
virtual void outgoing_send(const PIPEasp__Types::ASP__PStdinBinary& send_par) = 0;
virtual void outgoing_send(const PIPEasp__Types::ASP__PKill& send_par) = 0;
virtual void outgoing_send(const PIPEasp__Types::ASP__PLineMode& send_par) = 0;
virtual void outgoing_send(const PIPEasp__Types::ASP__PEndOfInput& send_par) = 0;
virtual void outgoing_send(const PIPEasp__Types::ASP__Parallel__Command& send_par) = 0;
public:
alt_status receive(const COMPONENT_template& sender_template, COMPONENT *sender_ptr, FLOAT* timestamp_redirect, Index_Redirect*);
alt_status check_receive(const COMPONENT_template& sender_template, COMPONENT *sender_ptr, FLOAT* timestamp_redirect, Index_Redirect*);
alt_status trigger(const COMPONENT_template& sender_template, COMPONENT *sender_ptr, FLOAT* timestamp_redirect, Index_Redirect*);
alt_status receive(const PIPEasp__Types::ASP__PResult_template& value_template, PIPEasp__Types::ASP__PResult *value_redirect, const COMPONENT_template& sender_template, COMPONENT *sender_ptr, FLOAT* timestamp_redirect, Index_Redirect*);
alt_status check_receive(const PIPEasp__Types::ASP__PResult_template& value_template, PIPEasp__Types::ASP__PResult *value_redirect, const COMPONENT_template& sender_template, COMPONENT *sender_ptr, FLOAT* timestamp_redirect, Index_Redirect*);
alt_status trigger(const PIPEasp__Types::ASP__PResult_template& value_template, PIPEasp__Types::ASP__PResult *value_redirect, const COMPONENT_template& sender_template, COMPONENT *sender_ptr, FLOAT* timestamp_redirect, Index_Redirect*);
alt_status receive(const PIPEasp__Types::ASP__PResultBinary_template& value_template, PIPEasp__Types::ASP__PResultBinary *value_redirect, const COMPONENT_template& sender_template, COMPONENT *sender_ptr, FLOAT* timestamp_redirect, Index_Redirect*);
alt_status check_receive(const PIPEasp__Types::ASP__PResultBinary_template& value_template, PIPEasp__Types::ASP__PResultBinary *value_redirect, const COMPONENT_template& sender_template, COMPONENT *sender_ptr, FLOAT* timestamp_redirect, Index_Redirect*);
alt_status trigger(const PIPEasp__Types::ASP__PResultBinary_template& value_template, PIPEasp__Types::ASP__PResultBinary *value_redirect, const COMPONENT_template& sender_template, COMPONENT *sender_ptr, FLOAT* timestamp_redirect, Index_Redirect*);
alt_status receive(const PIPEasp__Types::ASP__PStdout_template& value_template, PIPEasp__Types::ASP__PStdout *value_redirect, const COMPONENT_template& sender_template, COMPONENT *sender_ptr, FLOAT* timestamp_redirect, Index_Redirect*);
alt_status check_receive(const PIPEasp__Types::ASP__PStdout_template& value_template, PIPEasp__Types::ASP__PStdout *value_redirect, const COMPONENT_template& sender_template, COMPONENT *sender_ptr, FLOAT* timestamp_redirect, Index_Redirect*);
alt_status trigger(const PIPEasp__Types::ASP__PStdout_template& value_template, PIPEasp__Types::ASP__PStdout *value_redirect, const COMPONENT_template& sender_template, COMPONENT *sender_ptr, FLOAT* timestamp_redirect, Index_Redirect*);
alt_status receive(const PIPEasp__Types::ASP__PStderr_template& value_template, PIPEasp__Types::ASP__PStderr *value_redirect, const COMPONENT_template& sender_template, COMPONENT *sender_ptr, FLOAT* timestamp_redirect, Index_Redirect*);
alt_status check_receive(const PIPEasp__Types::ASP__PStderr_template& value_template, PIPEasp__Types::ASP__PStderr *value_redirect, const COMPONENT_template& sender_template, COMPONENT *sender_ptr, FLOAT* timestamp_redirect, Index_Redirect*);
alt_status trigger(const PIPEasp__Types::ASP__PStderr_template& value_template, PIPEasp__Types::ASP__PStderr *value_redirect, const COMPONENT_template& sender_template, COMPONENT *sender_ptr, FLOAT* timestamp_redirect, Index_Redirect*);
alt_status receive(const PIPEasp__Types::ASP__PStdoutBinary_template& value_template, PIPEasp__Types::ASP__PStdoutBinary *value_redirect, const COMPONENT_template& sender_template, COMPONENT *sender_ptr, FLOAT* timestamp_redirect, Index_Redirect*);
alt_status check_receive(const PIPEasp__Types::ASP__PStdoutBinary_template& value_template, PIPEasp__Types::ASP__PStdoutBinary *value_redirect, const COMPONENT_template& sender_template, COMPONENT *sender_ptr, FLOAT* timestamp_redirect, Index_Redirect*);
alt_status trigger(const PIPEasp__Types::ASP__PStdoutBinary_template& value_template, PIPEasp__Types::ASP__PStdoutBinary *value_redirect, const COMPONENT_template& sender_template, COMPONENT *sender_ptr, FLOAT* timestamp_redirect, Index_Redirect*);
alt_status receive(const PIPEasp__Types::ASP__PStderrBinary_template& value_template, PIPEasp__Types::ASP__PStderrBinary *value_redirect, const COMPONENT_template& sender_template, COMPONENT *sender_ptr, FLOAT* timestamp_redirect, Index_Redirect*);
alt_status check_receive(const PIPEasp__Types::ASP__PStderrBinary_template& value_template, PIPEasp__Types::ASP__PStderrBinary *value_redirect, const COMPONENT_template& sender_template, COMPONENT *sender_ptr, FLOAT* timestamp_redirect, Index_Redirect*);
alt_status trigger(const PIPEasp__Types::ASP__PStderrBinary_template& value_template, PIPEasp__Types::ASP__PStderrBinary *value_redirect, const COMPONENT_template& sender_template, COMPONENT *sender_ptr, FLOAT* timestamp_redirect, Index_Redirect*);
alt_status receive(const PIPEasp__Types::ASP__PExit_template& value_template, PIPEasp__Types::ASP__PExit *value_redirect, const COMPONENT_template& sender_template, COMPONENT *sender_ptr, FLOAT* timestamp_redirect, Index_Redirect*);
alt_status check_receive(const PIPEasp__Types::ASP__PExit_template& value_template, PIPEasp__Types::ASP__PExit *value_redirect, const COMPONENT_template& sender_template, COMPONENT *sender_ptr, FLOAT* timestamp_redirect, Index_Redirect*);
alt_status trigger(const PIPEasp__Types::ASP__PExit_template& value_template, PIPEasp__Types::ASP__PExit *value_redirect, const COMPONENT_template& sender_template, COMPONENT *sender_ptr, FLOAT* timestamp_redirect, Index_Redirect*);
alt_status receive(const PIPEasp__Types::ASP__PError_template& value_template, PIPEasp__Types::ASP__PError *value_redirect, const COMPONENT_template& sender_template, COMPONENT *sender_ptr, FLOAT* timestamp_redirect, Index_Redirect*);
alt_status check_receive(const PIPEasp__Types::ASP__PError_template& value_template, PIPEasp__Types::ASP__PError *value_redirect, const COMPONENT_template& sender_template, COMPONENT *sender_ptr, FLOAT* timestamp_redirect, Index_Redirect*);
alt_status trigger(const PIPEasp__Types::ASP__PError_template& value_template, PIPEasp__Types::ASP__PError *value_redirect, const COMPONENT_template& sender_template, COMPONENT *sender_ptr, FLOAT* timestamp_redirect, Index_Redirect*);
alt_status receive(const PIPEasp__Types::ASP__Parrallel__Result_template& value_template, PIPEasp__Types::ASP__Parrallel__Result *value_redirect, const COMPONENT_template& sender_template, COMPONENT *sender_ptr, FLOAT* timestamp_redirect, Index_Redirect*);
alt_status check_receive(const PIPEasp__Types::ASP__Parrallel__Result_template& value_template, PIPEasp__Types::ASP__Parrallel__Result *value_redirect, const COMPONENT_template& sender_template, COMPONENT *sender_ptr, FLOAT* timestamp_redirect, Index_Redirect*);
alt_status trigger(const PIPEasp__Types::ASP__Parrallel__Result_template& value_template, PIPEasp__Types::ASP__Parrallel__Result *value_redirect, const COMPONENT_template& sender_template, COMPONENT *sender_ptr, FLOAT* timestamp_redirect, Index_Redirect*);
private:
void incoming_message(const PIPEasp__Types::ASP__PResult& incoming_par, component sender_component);
void incoming_message(const PIPEasp__Types::ASP__PResultBinary& incoming_par, component sender_component);
void incoming_message(const PIPEasp__Types::ASP__PStdout& incoming_par, component sender_component);
void incoming_message(const PIPEasp__Types::ASP__PStderr& incoming_par, component sender_component);
void incoming_message(const PIPEasp__Types::ASP__PStdoutBinary& incoming_par, component sender_component);
void incoming_message(const PIPEasp__Types::ASP__PStderrBinary& incoming_par, component sender_component);
void incoming_message(const PIPEasp__Types::ASP__PExit& incoming_par, component sender_component);
void incoming_message(const PIPEasp__Types::ASP__PError& incoming_par, component sender_component);
void incoming_message(const PIPEasp__Types::ASP__Parrallel__Result& incoming_par, component sender_component);
protected:
inline void incoming_message(const PIPEasp__Types::ASP__PResult& incoming_par) { incoming_message(incoming_par, SYSTEM_COMPREF); }
inline void incoming_message(const PIPEasp__Types::ASP__PResultBinary& incoming_par) { incoming_message(incoming_par, SYSTEM_COMPREF); }
inline void incoming_message(const PIPEasp__Types::ASP__PStdout& incoming_par) { incoming_message(incoming_par, SYSTEM_COMPREF); }
inline void incoming_message(const PIPEasp__Types::ASP__PStderr& incoming_par) { incoming_message(incoming_par, SYSTEM_COMPREF); }
inline void incoming_message(const PIPEasp__Types::ASP__PStdoutBinary& incoming_par) { incoming_message(incoming_par, SYSTEM_COMPREF); }
inline void incoming_message(const PIPEasp__Types::ASP__PStderrBinary& incoming_par) { incoming_message(incoming_par, SYSTEM_COMPREF); }
inline void incoming_message(const PIPEasp__Types::ASP__PExit& incoming_par) { incoming_message(incoming_par, SYSTEM_COMPREF); }
inline void incoming_message(const PIPEasp__Types::ASP__PError& incoming_par) { incoming_message(incoming_par, SYSTEM_COMPREF); }
inline void incoming_message(const PIPEasp__Types::ASP__Parrallel__Result& incoming_par) { incoming_message(incoming_par, SYSTEM_COMPREF); }
boolean process_message(const char *message_type, Text_Buf& incoming_buf, component sender_component, OCTETSTRING& slider);
};


/* Function prototypes */

extern INTEGER f__PIPE__request__p__id(PIPEasp__PT& pl__port, const BOOLEAN& pl__reusable);
extern BOOLEAN f__PIPE__release__p__id(PIPEasp__PT& pl__port, const INTEGER& pl__pid);

/* Global variable declarations */

extern const BOOLEAN& f__PIPE__request__p__id_pl__reusable_defval;
extern TTCN_Module module_object;

} /* end of namespace */

/* Test port header files */

#include "PIPEasp_PT.hh"

#endif
