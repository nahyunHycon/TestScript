/******************************************************************************
* Copyright (c) 2000-2023 Ericsson Telecom AB
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v2.0
* which accompanies this distribution, and is available at
* https://www.eclipse.org/org/documents/epl-2.0/EPL-2.0.html
*
* Contributors:
*  Jozsef Gyurusi - initial implementation and initial documentation
*  Adam Delic
*  Antal Wuh.Hen.Chang
*  Attila Balasko
*  Gabor Szalai
*  Istvan Sandor
*  Peter Kremer
*  Zoltan Jasz
******************************************************************************/
//
//  File:               PIPEasp_PT.hh
//  Description:        Header file of PIPE testport implementation
//  Rev:                R7D
//  Prodnr:             CNL 113 334
//


#ifndef PIPEasp__PT_HH
#define PIPEasp__PT_HH

#include "PIPEasp_PortType.hh"

namespace PIPEasp__PortType {

class PIPEasp__PT : public PIPEasp__PT_BASE {
public:
	PIPEasp__PT(const char *par_port_name = NULL);
	~PIPEasp__PT();

	void set_parameter(const char *parameter_name,
		const char *parameter_value);

/*	void Event_Handler(const fd_set *read_fds,
		const fd_set *write_fds, const fd_set *error_fds,
		double time_since_last_call);*/
  void Handle_Fd_Event_Readable(int fd);

  struct pipedata_main{
    int state; // 0- free, 1-allocated, 2-wait for release
    bool reuse;
    bool processExecuting; // true if process is executing: disable new processes
    bool binaryMode; // true if result should be returned in as binary data
    bool disableSend; // if true sendStdout/err is disabled
    bool ptyMode;       // pty mode

    TTCN_Buffer *stdout_buffer; // data sent to stdout 
    TTCN_Buffer *stderr_buffer; // data sent to stderr
    int processExitCode;       // exit code of the process
  };
  struct pipedata_thread{
    bool processExecuting; // true if process is executing: disable new processes
    int processStdin;   // fd of stdin of the process
    int processStdout;  // fd of stdout of the process
    int processStderr;  // fd of stderr of the process

    int pid;
    bool ptyMode;       // pty mode
    int processPty;     // pty of the process (in pty mode)
    int processExitCode;       // exit code of the process
    
  };

  int maindata_len;
  int maindata_used;
  int threaddata_len;
  pipedata_main *main_data;
  pipedata_thread *thread_data;
  void setup_pipedata_main(int idx);
  void send_p_id_to_thread(const int p_id);
protected:
	void user_map(const char *system_port);
	void user_unmap(const char *system_port);

  void start_child();

	void user_start();
	void user_stop();

	void outgoing_send(const PIPEasp__Types::ASP__PExecute& send_par);
	void outgoing_send(const PIPEasp__Types::ASP__PExecutePty& send_par);
	void outgoing_send(const PIPEasp__Types::ASP__PExecuteBinary& send_par);
	void outgoing_send(const PIPEasp__Types::ASP__PExecuteBinaryPty& send_par);
	void outgoing_send(const PIPEasp__Types::ASP__PExecuteBackground& send_par);
	void outgoing_send(const PIPEasp__Types::ASP__PExecuteBackgroundPty& send_par);
	void outgoing_send(const PIPEasp__Types::ASP__PStdin& send_par);
	void outgoing_send(const PIPEasp__Types::ASP__PStdinBinary& send_par);
	void outgoing_send(const PIPEasp__Types::ASP__PKill& send_par);
	void outgoing_send(const PIPEasp__Types::ASP__PLineMode& send_par);
	void outgoing_send(const PIPEasp__Types::ASP__PEndOfInput& send_par);
	void outgoing_send(const PIPEasp__Types::ASP__Parallel__Command& send_par);
private:
	void handle_message(const int p_id, const PIPEasp__Types::ASP__PExecute& send_par);
	void handle_message(const int p_id, const PIPEasp__Types::ASP__PExecutePty& send_par);
	void handle_message(const int p_id, const PIPEasp__Types::ASP__PExecuteBinary& send_par);
	void handle_message(const int p_id, const PIPEasp__Types::ASP__PExecuteBinaryPty& send_par);
	void handle_message(const int p_id, const PIPEasp__Types::ASP__PExecuteBackground& send_par);
	void handle_message(const int p_id, const PIPEasp__Types::ASP__PExecuteBackgroundPty& send_par);
	void handle_message(const int p_id, const PIPEasp__Types::ASP__PStdin& send_par);
	void handle_message(const int p_id, const PIPEasp__Types::ASP__PStdinBinary& send_par);
	void handle_message(const int p_id, const PIPEasp__Types::ASP__PKill& send_par);
	void handle_message(const int p_id, const PIPEasp__Types::ASP__PEndOfInput& send_par);


  
  boolean pipe_id_valid(const int p_id){if((p_id>=0)&&(p_id<maindata_len)){return main_data[p_id].state==1;} else {return false;}}
  

        int execCommand(const char* command);
        void handle_childDeath(const int p_id);
        void sendStdout(const int p_id);
        void sendStderr(const int p_id);
        void sendExitCode(const int p_id);
        void sendResult(const int p_id);
        void sendError(const char* error_msg,const int p_id);
        void log(const char *fmt, ...);
        void log_buffer(const unsigned char * buf, size_t buflen);
        int get_len(const TTCN_Buffer& buff);
        int put_msg(TTCN_Buffer& buff, int msg_type, int len, const unsigned char* msg, const int p_id);
        // Msg types
        // 1 - kill server
        // 2 - execute
        // 3 - stdin or pty
        // 4 - stdout
        // 5 - stderr 
        // 6 - execute finished
        // 7 - kill command
        // 8 - end of input
        // 9 - unmap
        // 10 - execute in pty mode
        // 11 - set echo mode for pty
        // 12 - new parallel process id
        
        // Msg struct
        // 1. byte - type
        // 2-3 byte - len
        // 4-5 byte - p_id optional
        // 4- byte - data optional
        void processHandler();
        void handle_childDeath_inprocess(const int p_id);
        void newcmd(const int p_id,const char* command);
        void newcmd_pty(const int p_id,const char* command);
        
        void close_and_remove_fd(int fd);
        void free_thread_data(){ if(thread_data){Free(thread_data);thread_data=NULL;threaddata_len=0;}}
        void init_pipedata(int idx);

private:
  bool lineMode; // true if lineMode is enabled
  bool pty_echo; // true: suppress echo
  bool waitforpidrelease; // true if 
  fd_set working_set;     // fd set for event handler
  int max_fd;
//  int processPid;     // pid of the process currently executing
  int processServerPid;
  int processServerUp;
  int processServerDown;

  TTCN_Buffer process_buffer;

  bool debugAllowed; 

};

}//namespace

#endif
