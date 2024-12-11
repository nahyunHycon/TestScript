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
//  File:               PIPEasp_PT.cc
//  Description:        Source code of PIPE testport implementation
//  Rev:                R7D
//  Prodnr:             CNL 113 334
//


#include "PIPEasp_PT.hh"
#include <signal.h> //kill
#include <unistd.h> //pipe
#include <errno.h>  //errno
#include <ctype.h>  //isspace
#include <sys/select.h>  //FD_ZERO
#include <stdio.h>      // sys_errlist
#include <sys/types.h>  //wait
#include <sys/socket.h>  //wait
#include <sys/wait.h>   //wait
#include <termios.h>

#ifndef PIPE_BUF_SIZE
#define PIPE_BUF_SIZE 65534
#endif

#if defined LINUX || defined FREEBSD || defined WIN32
#include <pty.h>
#endif

#if defined SOLARIS || defined SOLARIS8
#include <strings.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/stream.h>
#include <sys/stropts.h>
/////////////////////////////////////
int forkpty_solaris (int *amaster, char *name, struct termios
         *termp, struct winsize *winp)
{
    int fdm, fds;
    char *slavename;
    pid_t pid;
    fdm = open("/dev/ptmx", O_RDWR);  /* open master */
    grantpt(fdm);                     /* change permission of slave */
    unlockpt(fdm);                    /* unlock slave */
    slavename = ptsname(fdm);         /* get name of slave */
    if (name) strcpy(name, slavename);
    *amaster = fdm;
    if ((pid = fork()) < 0) {
    return pid;         /* error */
    }
    else if (pid != 0) {        /* parent */
    return pid;
    }
    else {            /* child */
    pid_t pgid;
    /* create a new session */
    pgid = setsid();
    if (pgid == -1) {
        perror("forkpty_solaris() setsid failed");
        return -1;
    }
    fds = open(slavename, O_RDWR);    /* open slave */
    ioctl(fds, I_PUSH, "ptem");       /* push ptem */
    ioctl(fds, I_PUSH, "ldterm");    /* push ldterm */
    dup2(fds, 0);
    dup2(fds, 1);
    dup2(fds, 2);
    ioctl(fds, TIOCSPGRP, &pgid);
    /* magic */
    if (termp)
        ioctl(fds, TCSETS, termp);
    if (winp)
        ioctl(fds, TIOCSWINSZ, winp);
    return pid;
    }
}

/////////////////////////////////////
#define forkpty forkpty_solaris
#endif
namespace PIPEasp__Types {
  
BOOLEAN f__PIPE__WIFEXITED(const INTEGER& code){
  int c=(int)code;
  return WIFEXITED(c);
}
INTEGER f__PIPE__WEXITSTATUS(const INTEGER& code){
  int c=(int)code;
  return WEXITSTATUS(c);
}
;
BOOLEAN f__PIPE__WIFSIGNALED(const INTEGER& code){
  int c=(int)code;
  return WIFSIGNALED(c);
}

INTEGER f__PIPE__WTERMSIG(const INTEGER& code){
  int c=(int)code;
  return WTERMSIG(c);
}

}
namespace PIPEasp__PortType {

PIPEasp__PT::PIPEasp__PT(const char *par_port_name)
	: PIPEasp__PT_BASE(par_port_name)
        , maindata_len(0)
        , maindata_used(0)
        , threaddata_len(0)
        , main_data(NULL)
        , thread_data(NULL)
        , lineMode(true)
//        , processPid(-1)     // pid of the process currently executing
        , debugAllowed(false) // debug disabled by default
{
  FD_ZERO(&working_set);
  process_buffer.clear();
  processServerPid=-1;
  pty_echo=false;
  waitforpidrelease=false;
  processServerDown=-1;
  processServerUp=-1;
  max_fd=-1;
}

PIPEasp__PT::~PIPEasp__PT()
{
//printf("processServerPid %d\r\n",processServerPid);
  if(processServerPid>0){
//    char kill_msg[5];
//    kill_msg[0]=1;
//    kill_msg[1]=0;
//    kill_msg[2]=0;
//    kill_msg[3]=0;
//    kill_msg[4]=0;
    
//    write(processServerDown,kill_msg,3);
    close(processServerDown);

    int status=0;
    wait(&status);
  }
  
  if(main_data){
    for(int i=0;i<maindata_len;i++) {
      if((main_data+i)->stdout_buffer){delete (main_data+i)->stdout_buffer;}; 
      if((main_data+i)->stderr_buffer){delete (main_data+i)->stderr_buffer;};
    }
    Free(main_data);
  }
  if(thread_data){
    Free(thread_data);
  }
}

void PIPEasp__PT::set_parameter(const char *parameter_name,
	const char *parameter_value)
{
  if (!strcasecmp(parameter_name, "debug")) {
    if (!strcasecmp(parameter_value,"YES")) {
      debugAllowed = true;
    }
  }
  if (!strcasecmp(parameter_name, "suppress_pty_echo")) {
    if (!strcasecmp(parameter_value,"YES")) {
      pty_echo = true;
    }
  }
  if (!strcasecmp(parameter_name, "auto_pid_release")) {
    if (!strcasecmp(parameter_value,"no")) {
      waitforpidrelease = true;
    } else {
      waitforpidrelease = false;
    }
  }
}

/*void PIPEasp__PT::Event_Handler(const fd_set *read_fds,
	const fd_set *write_fds, const fd_set *error_fds,
	double time_since_last_call)*/
void PIPEasp__PT::Handle_Fd_Event_Readable(int /*fd*/)
{
  log("PIPEasp__PT::Event_Handler called");
  
  long nBytes;
  int r;

  nBytes = PIPE_BUF_SIZE;
  unsigned char* buffer;
  size_t end_len = nBytes;
  process_buffer.get_end(buffer, end_len);
  r = read(processServerUp,buffer,(int)nBytes);
  log("read returned %d",r);
  if (r <= 0) {
      log("ttcn_pipe_port: read problem from server process. %d, %d, %s\n", r, errno, strerror(errno));
      close(processServerUp);
      close(processServerDown);
      Uninstall_Handler();
      int status=0;
      wait(&status);
      processServerPid=-1;
  }
  else {
    process_buffer.increase_length(r);
    int msg_len;
    int buff_len=process_buffer.get_len();
    const unsigned char* buff_data=process_buffer.get_data();

//     log("process buffer length %d",buff_len);
//      if(buff_len>=3) {
//    	  const unsigned char* data=process_buffer.get_data();
//    	  log("buffer data %02x %02x %02x", data[0],data[1],data[2]);
//      }
    
    log_buffer(buff_data,buff_len);

    while((msg_len=get_len(process_buffer))!=-1){
      const unsigned char* msg=process_buffer.get_data();
      int p_id;
      switch(msg[0]){
        case 4: // stdout
          p_id=((int)msg[5]<<8) + (int)msg[6];
          if (!((main_data+p_id)->processExecuting)) {
            TTCN_warning("Unexpected message from stdout, no command is executing");
          } else {
            log("Incoming stdout message received from process: %d",msg_len);
          }
          
          (main_data+p_id)->stdout_buffer->put_s(msg_len-2,msg+7);
          sendStdout(p_id);
          break;
        case 5: // stderr
          p_id=((int)msg[5]<<8) + (int)msg[6];
          if (!((main_data+p_id)->processExecuting)) {
            TTCN_warning("Unexpected message from stderr, no command is executing");
          } else {
            log("Incoming stderr message received from process");
          }
          (main_data+p_id)->stderr_buffer->put_s(msg_len-2,msg+7);
          sendStderr(p_id);
          break;
        case 6: // end of process
          p_id=((int)msg[5]<<8) + (int)msg[6];
          (main_data+p_id)->processExitCode=((int)msg[7]<<24) + ((int)msg[8]<<16) + ((int)msg[9]<<8) + (int)msg[10];
          handle_childDeath(p_id);
          break;
        default:
          break;
      }
      process_buffer.set_pos(5+msg_len);
      process_buffer.cut();
      buff_len=process_buffer.get_len();
      buff_data=process_buffer.get_data();

//     log("process buffer length %d",buff_len);
//      if(buff_len>=3) {
//    	  const unsigned char* data=process_buffer.get_data();
//    	  log("buffer data %02x %02x %02x", data[0],data[1],data[2]);
//      }

      log_buffer(buff_data,buff_len);

		}
	  }
	}

void PIPEasp__PT::user_map(const char *system_port)
{
 log("%s mapping, child pid: %d",system_port, processServerPid);
    Handler_Add_Fd_Read(processServerUp);
    maindata_len=1;
    maindata_used=1;
    main_data=(pipedata_main*)Malloc(sizeof(pipedata_main));
    setup_pipedata_main(0); 
    main_data[0].reuse=true;
    send_p_id_to_thread(0);
  unsigned char msg[6];
  msg[0]=11;
  msg[1]=0;
  msg[2]=0;
  msg[3]=0;
  msg[4]=1;
  msg[5]= pty_echo;
  write(processServerDown,msg,6);
}

void PIPEasp__PT::setup_pipedata_main(int idx){
    main_data[idx].state=1; // 0- free, 1-allocated
    main_data[idx].reuse=false;
    main_data[idx].processExecuting=false; // true if process is executing: disable new processes
    main_data[idx].binaryMode=false; // true if result should be returned in as binary data
    main_data[idx].disableSend=false; // if true sendStdout/err is disabled
    main_data[idx].ptyMode=false;       // pty mode

    main_data[idx].stdout_buffer=new TTCN_Buffer; // data sent to stdout 
    main_data[idx].stderr_buffer=new TTCN_Buffer; // data sent to stderr
    main_data[idx].processExitCode=-1;       // exit code of the process

}

void PIPEasp__PT::send_p_id_to_thread(const int p_id){
  unsigned char msg[7];
  msg[0]=12;
  msg[1]=0;
  msg[2]=0;
  msg[3]=0;
  msg[4]=2;
  msg[5]= (p_id >> 8) & 0xFF;
  msg[6]= p_id & 0xFF;
  write(processServerDown,msg,7);

}

void PIPEasp__PT::user_unmap(const char *system_port)
{
    log("user_unmap started %s",system_port);
//printf("****user_unmap started %s",system_port);
    Uninstall_Handler();
    log("Sending unmap to processServer");
    TTCN_Buffer out_buff;
    out_buff.clear();
    process_buffer.clear(); 

    int write_len=put_msg(out_buff,9,0,NULL,-1); // Sending unmap to processServer
    log("Sending unmap message to the fork server %d",write_len);
    int res=write(processServerDown,out_buff.get_data(),write_len);
    log("Message sent %d",res);

    if(main_data){
      for(int i=0;i<maindata_len;i++) {
        if((main_data+i)->stdout_buffer){delete (main_data+i)->stdout_buffer;}; 
        if((main_data+i)->stderr_buffer){delete (main_data+i)->stderr_buffer;};
      }
      Free(main_data);
      main_data=NULL;
    }

    log("user_unmap finsihed");

}
void PIPEasp__PT::user_start()
{
start_child();
}

void PIPEasp__PT::start_child()
{
	  if(processServerPid != -1) return;
	  int pipefd[2];
	  int pipefd2[2];
	  if (pipe(pipefd) != 0) {
	      return;
	  }
	  if (pipe(pipefd2) != 0) {
	      return;
	  }
	  processServerPid=fork();
	  if (processServerPid < 0) {
	    //
	    // Error
	    //

	    // close the pipes
	    close(pipefd[0]);
	    close(pipefd[1]);
	    close(pipefd2[0]);
	    close(pipefd2[1]);
	    processServerUp=-1;
	    processServerDown=-1;
	  }
	  else if (processServerPid == 0) {

	      //
	      // Child process
	      //

	      // close the parent end of the pipes
	      processServerUp=pipefd2[1];
	      processServerDown=pipefd[0];

// This is a hack
// All fds (except the pipe) should be closed in order to avoid some "funny" communication lost between the mctr and PTC
// There is no easy way to get the list of the open fds
// The start_child called when the test port is started first time
// which is right after the component initialization/first test case start
// fd 0-3 are the standard io fds
// there are only 2 or 3 fd opened by the TITAN run time (they should be closed)
// So it is assummed the max value of the fd below 42
for(int i=4;i<42;i++){
  if((i!=processServerUp) && (i!=processServerDown)){
    close(i);
  }
}
	      close(pipefd[1]);
	      close(pipefd2[0]);
	      processHandler();
	//printf("exit\r\n");
	//throw TC_Error();
	      _exit(0); // end of child process
	  }
	  else {

	    //
	    // Parent process
	    //

//	    TTCN_warning("Process started with pid: %d\r\n", processServerPid);
	    // close child end of the pipes
	      processServerUp=pipefd2[0];
	      processServerDown=pipefd[1];
	    close(pipefd[0]);
	    close(pipefd2[1]);
	 }


}

void PIPEasp__PT::user_stop()
{
}

/*************************************
*  Specific outgoing_send functions
*************************************/
void PIPEasp__PT::handle_message(const int p_id, const PIPEasp__Types::ASP__PExecute& send_par) {

  if(!pipe_id_valid(p_id)){
    sendError("Pipe Test Port: Pipe execution id (p_id) is not valid",p_id);
    if(debugAllowed)
    {
    TTCN_Logger::begin_event(TTCN_DEBUG);
    TTCN_Logger::log_event("PIPE test port (%s): Pipe execution id (p_id=%d) is not valid: ", get_name(),p_id);
    send_par.log();
    TTCN_Logger::end_event();
    }
    return;
  }

  if ((main_data+p_id)->processExecuting) {
    sendError("Pipe Test Port: Command already executing",p_id);
    if(debugAllowed)
    {
    TTCN_Logger::begin_event(TTCN_DEBUG);
    TTCN_Logger::log_event("PIPE test port (%s): Command already executing (p_id=%d). Following ASP is ignored: ", get_name(),p_id);
    send_par.log();
    TTCN_Logger::end_event();
    }
    return;
  }
  PIPEasp__Types::ASP__PExecuteBackground message_PExecuteBackground;
  // starting command
  message_PExecuteBackground.command() = send_par.command();
  handle_message(p_id,message_PExecuteBackground);
  // sending input
  PIPEasp__Types::ASP__PStdin message_PStdin;
  message_PStdin.stdin_() = send_par.stdin_();
  handle_message(p_id,message_PStdin);
  main_data[p_id].disableSend = true;

  // closing stdin pipe:
  handle_message(p_id,PIPEasp__Types::ASP__PEndOfInput());
  
}

void PIPEasp__PT::outgoing_send(const PIPEasp__Types::ASP__PExecute& send_par) {
  log("PIPEasp__PT::outgoing_send_PExecute called");

  handle_message(0,send_par);
  
  log("PIPEasp__PT::outgoing_send_PExecute exited");
}

void PIPEasp__PT::handle_message(const int p_id,const PIPEasp__Types::ASP__PExecutePty& send_par) {

  if(!pipe_id_valid(p_id)){
    sendError("Pipe Test Port: Pipe execution id (p_id) is not valid",p_id);
    if(debugAllowed)
    {
    TTCN_Logger::begin_event(TTCN_DEBUG);
    TTCN_Logger::log_event("PIPE test port (%s): Pipe execution id (p_id=%d) is not valid: ", get_name(),p_id);
    send_par.log();
    TTCN_Logger::end_event();
    }
    return;
  }

  if ((main_data+p_id)->processExecuting) {
    sendError("Pipe Test Port: Command already executing",p_id);
    if(debugAllowed)
    {
    TTCN_Logger::begin_event(TTCN_DEBUG);
    TTCN_Logger::log_event("PIPE test port (%s): Command already executing (p_id=%d). Following ASP is ignored: ", get_name(),p_id);
    send_par.log();
    TTCN_Logger::end_event();
    }
    return;
  }

  PIPEasp__Types::ASP__PExecuteBackgroundPty message_PExecuteBackgroundPty;
  // starting command
  message_PExecuteBackgroundPty.command() = send_par.command();
  handle_message(p_id,message_PExecuteBackgroundPty);
  // sending input
  PIPEasp__Types::ASP__PStdin message_PStdin;
  message_PStdin.stdin_() = send_par.stdin_();
  handle_message(p_id,message_PStdin);
  (main_data+p_id)->disableSend = true;

  // closing stdin pipe:
  handle_message(p_id,PIPEasp__Types::ASP__PEndOfInput());
}

void PIPEasp__PT::outgoing_send(const PIPEasp__Types::ASP__PExecutePty& send_par) {
  log("PIPEasp__PT::outgoing_send_PExecutePty called");

  handle_message(0,send_par);

  log("PIPEasp__PT::outgoing_send_PExecutePty exited");
}

void PIPEasp__PT::handle_message(const int p_id,const PIPEasp__Types::ASP__PExecuteBinary& send_par) {

  if(!pipe_id_valid(p_id)){
    sendError("Pipe Test Port: Pipe execution id (p_id) is not valid",p_id);
    if(debugAllowed)
    {
    TTCN_Logger::begin_event(TTCN_DEBUG);
    TTCN_Logger::log_event("PIPE test port (%s): Pipe execution id (p_id=%d) is not valid: ", get_name(),p_id);
    send_par.log();
    TTCN_Logger::end_event();
    }
    return;
  }

  if (main_data[p_id].processExecuting) {
    sendError("Pipe Test Port: Command already executing",p_id);
    if(debugAllowed)
    {
    TTCN_Logger::begin_event(TTCN_DEBUG);
    TTCN_Logger::log_event("PIPE test port (%s): Command already executing (p_id=%d). Following ASP is ignored: ", get_name(),p_id);
    send_par.log();
    TTCN_Logger::end_event();
    }
    return;
  }

  PIPEasp__Types::ASP__PExecuteBackground message_PExecuteBackground;
  // starting command
  message_PExecuteBackground.command() = send_par.command();
  handle_message(p_id,message_PExecuteBackground);
  // sending input
  PIPEasp__Types::ASP__PStdinBinary message_PStdinBinary;
  message_PStdinBinary.stdin_() = send_par.stdin_();
  handle_message(p_id,message_PStdinBinary);
  main_data[p_id].disableSend = true;
  
  // closing stdin pipe:
  handle_message(p_id,PIPEasp__Types::ASP__PEndOfInput());
  
  log("PIPEasp__PT::outgoing_send_PExecuteBinary exited");
}

void PIPEasp__PT::outgoing_send(const PIPEasp__Types::ASP__PExecuteBinary& send_par) {
  log("PIPEasp__PT::outgoing_send_PExecuteBinary called");

  handle_message(0,send_par);
  
  log("PIPEasp__PT::outgoing_send_PExecuteBinary exited");
}

void PIPEasp__PT::handle_message(const int p_id,const PIPEasp__Types::ASP__PExecuteBinaryPty& send_par) {

  if(!pipe_id_valid(p_id)){
    sendError("Pipe Test Port: Pipe execution id (p_id) is not valid",p_id);
    if(debugAllowed)
    {
    TTCN_Logger::begin_event(TTCN_DEBUG);
    TTCN_Logger::log_event("PIPE test port (%s): Pipe execution id (p_id=%d) is not valid: ", get_name(),p_id);
    send_par.log();
    TTCN_Logger::end_event();
    }
    return;
  }

  if (main_data[p_id].processExecuting) {
    sendError("Pipe Test Port: Command already executing",p_id);
    if(debugAllowed)
    {
    TTCN_Logger::begin_event(TTCN_DEBUG);
    TTCN_Logger::log_event("PIPE test port (%s): Command already executing (p_id=%d). Following ASP is ignored: ", get_name(),p_id);
    send_par.log();
    TTCN_Logger::end_event();
    }
    return;
  }

  PIPEasp__Types::ASP__PExecuteBackgroundPty message_PExecuteBackgroundPty;
  // starting command
  message_PExecuteBackgroundPty.command() = send_par.command();
  handle_message(p_id,message_PExecuteBackgroundPty);
  // sending input
  PIPEasp__Types::ASP__PStdinBinary message_PStdinBinary;
  message_PStdinBinary.stdin_() = send_par.stdin_();
  handle_message(p_id,message_PStdinBinary);
  main_data[p_id].disableSend = true;

  // closing stdin pipe:
  handle_message(p_id,PIPEasp__Types::ASP__PEndOfInput());

}

void PIPEasp__PT::outgoing_send(const PIPEasp__Types::ASP__PExecuteBinaryPty& send_par) {
  log("PIPEasp__PT::outgoing_send_PExecuteBinaryPty called");

  handle_message(0,send_par);

  log("PIPEasp__PT::outgoing_send_PExecuteBinaryPty exited");
}

void PIPEasp__PT::handle_message(const int p_id,const PIPEasp__Types::ASP__PExecuteBackground& send_par) {

  if(!pipe_id_valid(p_id)){
    sendError("Pipe Test Port: Pipe execution id (p_id) is not valid",p_id);
    if(debugAllowed)
    {
    TTCN_Logger::begin_event(TTCN_DEBUG);
    TTCN_Logger::log_event("PIPE test port (%s): Pipe execution id (p_id=%d) is not valid: ", get_name(),p_id);
    send_par.log();
    TTCN_Logger::end_event();
    }
    return;
  }

  if (main_data[p_id].processExecuting) {
    sendError("Pipe Test Port: Command already executing",p_id);
    if(debugAllowed)
    {
    TTCN_Logger::begin_event(TTCN_DEBUG);
    TTCN_Logger::log_event("PIPE test port (%s): Command already executing (p_id=%d). Following ASP is ignored: ", get_name(),p_id);
    send_par.log();
    TTCN_Logger::end_event();
    }
    return;
  }

  TTCN_Buffer out_buff;
  out_buff.clear();
  int write_len=put_msg(out_buff,2,send_par.command().lengthof(),(const unsigned char*)(const char*)send_par.command(),p_id);
  write(processServerDown,out_buff.get_data(),write_len);
  main_data[p_id].processExecuting=true;
  log("PIPEasp__PT::outgoing_send_PExecuteBackground exited");
}

void PIPEasp__PT::outgoing_send(const PIPEasp__Types::ASP__PExecuteBackground& send_par) {
  log("PIPEasp__PT::outgoing_send_PExecuteBackground called");
  
  handle_message(0,send_par);

  log("PIPEasp__PT::outgoing_send_PExecuteBackground exited");
}

void PIPEasp__PT::handle_message(const int p_id,const PIPEasp__Types::ASP__PExecuteBackgroundPty& send_par) {

  if(!pipe_id_valid(p_id)){
    sendError("Pipe Test Port: Pipe execution id (p_id) is not valid",p_id);
    if(debugAllowed)
    {
    TTCN_Logger::begin_event(TTCN_DEBUG);
    TTCN_Logger::log_event("PIPE test port (%s): Pipe execution id (p_id=%d) is not valid: ", get_name(),p_id);
    send_par.log();
    TTCN_Logger::end_event();
    }
    return;
  }

  if (main_data[p_id].processExecuting) {
    sendError("Pipe Test Port: Command already executing",p_id);
    if(debugAllowed)
    {
    TTCN_Logger::begin_event(TTCN_DEBUG);
    TTCN_Logger::log_event("PIPE test port (%s): Command already executing (p_id=%d). Following ASP is ignored: ", get_name(),p_id);
    send_par.log();
    TTCN_Logger::end_event();
    }
    return;
  }

  TTCN_Buffer out_buff;
  out_buff.clear();
  int write_len=put_msg(out_buff,10,send_par.command().lengthof(),(const unsigned char*)(const char*)send_par.command(),p_id);
  write(processServerDown,out_buff.get_data(),write_len);
  main_data[p_id].processExecuting=true;
  log("PIPEasp__PT::outgoing_send_PExecuteBackgroundPty exited");
}

void PIPEasp__PT::outgoing_send(const PIPEasp__Types::ASP__PExecuteBackgroundPty& send_par) {
  log("PIPEasp__PT::outgoing_send_PExecuteBackgroundPty called");
  
  handle_message(0,send_par);

  log("PIPEasp__PT::outgoing_send_PExecuteBackgroundPty exited");
}

void PIPEasp__PT::handle_message(const int p_id, const PIPEasp__Types::ASP__PStdin& send_par) {
  if(!pipe_id_valid(p_id)){
    sendError("Pipe Test Port: Pipe execution id (p_id) is not valid",p_id);
    if(debugAllowed)
    {
    TTCN_Logger::begin_event(TTCN_DEBUG);
    TTCN_Logger::log_event("PIPE test port (%s): Pipe execution id (p_id=%d) is not valid: ", get_name(),p_id);
    send_par.log();
    TTCN_Logger::end_event();
    }
    return;
  }

  if (!main_data[p_id].processExecuting) {
    sendError("Pipe Test Port: No command executing",p_id);
    if(debugAllowed)
    {
    TTCN_Logger::begin_event(TTCN_DEBUG);
    TTCN_Logger::log_event("PIPE test port (%s): No command executing (p_id=%d). Following ASP is ignored: ", get_name(),p_id);
    send_par.log();
    TTCN_Logger::end_event();
    }
    return;
  }


  if (main_data[p_id].disableSend) {
    sendError("ipe Test Port: PStdinBinary is not sent: current process is not started with PExecuteBackground!",p_id);
    if(debugAllowed)
    {
    TTCN_Logger::begin_event(TTCN_DEBUG);
    TTCN_Logger::log_event("PIPE test port (%s): ipe Test Port: PStdinBinary is not sent: current process is not started with PExecuteBackground! (p_id=%d). Following ASP is ignored: ", get_name(),p_id);
    send_par.log();
    TTCN_Logger::end_event();
    }
    return;
  }

  log("will now write to stdin: '%s'",
		 (const char*)(send_par.stdin_()+((lineMode)?"\n":"")));

  TTCN_Buffer out_buff;
  out_buff.clear();
  int write_len=put_msg(out_buff,3,send_par.stdin_().lengthof()+((lineMode)?1:0),
    (const unsigned char*)(const char*)(send_par.stdin_()+((lineMode)?"\n":"")),p_id);
  write(processServerDown,out_buff.get_data(),write_len);
  
}

void PIPEasp__PT::outgoing_send(const PIPEasp__Types::ASP__PStdin& send_par) {
  log("PIPEasp__PT::outgoing_send_PStdin called");

  handle_message(0,send_par);
  
  log("PIPEasp__PT::outgoing_send_PStdin exited");
}

void PIPEasp__PT::handle_message(const int p_id, const PIPEasp__Types::ASP__PStdinBinary& send_par) {

  if(!pipe_id_valid(p_id)){
    sendError("Pipe Test Port: Pipe execution id (p_id) is not valid",p_id);
    if(debugAllowed)
    {
    TTCN_Logger::begin_event(TTCN_DEBUG);
    TTCN_Logger::log_event("PIPE test port (%s): Pipe execution id (p_id=%d) is not valid: ", get_name(),p_id);
    send_par.log();
    TTCN_Logger::end_event();
    }
    return;
  }

  if (!main_data[p_id].processExecuting) {
    sendError("Pipe Test Port: No command executing",p_id);
    if(debugAllowed)
    {
    TTCN_Logger::begin_event(TTCN_DEBUG);
    TTCN_Logger::log_event("PIPE test port (%s): No command executing (p_id=%d). Following ASP is ignored: ", get_name(),p_id);
    send_par.log();
    TTCN_Logger::end_event();
    }
    return;
  }

  if (main_data[p_id].disableSend) {
    sendError("ipe Test Port: PStdinBinary is not sent: current process is not started with PExecuteBackground!",p_id);
    if(debugAllowed)
    {
    TTCN_Logger::begin_event(TTCN_DEBUG);
    TTCN_Logger::log_event("PIPE test port (%s): ipe Test Port: PStdinBinary is not sent: current process is not started with PExecuteBackground! (p_id=%d). Following ASP is ignored: ", get_name(),p_id);
    send_par.log();
    TTCN_Logger::end_event();
    }
    return;
  }
  
  main_data[p_id].binaryMode = true;
  
  if(debugAllowed)
  {
  TTCN_Logger::begin_event(TTCN_DEBUG);
  TTCN_Logger::log_event("PIPE test port (%s): will now write binary data to stdin: ", get_name());
  send_par.stdin_().log();
  TTCN_Logger::end_event();
  }

  TTCN_Buffer out_buff;
  out_buff.clear();
  int write_len=put_msg(out_buff,3,send_par.stdin_().lengthof(),
    (const unsigned char*)(send_par.stdin_()),p_id);
  write(processServerDown,out_buff.get_data(),write_len);

}

void PIPEasp__PT::outgoing_send(const PIPEasp__Types::ASP__PStdinBinary& send_par) {
  log("PIPEasp__PT::outgoing_send_PStdinBinary called");

  handle_message(0,send_par);

  log("PIPEasp__PT::outgoing_send_PStdinBinary exited");
}

void PIPEasp__PT::handle_message(const int p_id, const PIPEasp__Types::ASP__PKill& send_par) {

  if(!pipe_id_valid(p_id)){
    sendError("Pipe Test Port: Pipe execution id (p_id) is not valid",p_id);
    if(debugAllowed)
    {
    TTCN_Logger::begin_event(TTCN_DEBUG);
    TTCN_Logger::log_event("PIPE test port (%s): Pipe execution id (p_id=%d) is not valid: ", get_name(),p_id);
    send_par.log();
    TTCN_Logger::end_event();
    }
    return;
  }

  if (!main_data[p_id].processExecuting) {
    sendError("Pipe Test Port: No command executing",p_id);
    if(debugAllowed)
    {
    TTCN_Logger::begin_event(TTCN_DEBUG);
    TTCN_Logger::log_event("PIPE test port (%s): No command executing (p_id=%d). Following ASP is ignored: ", get_name(),p_id);
    send_par.log();
    TTCN_Logger::end_event();
    }
    return;
  }
  
  int signo = (int)send_par.signal();
  if (signo<1 || signo>31) {
    // signo out of range;
    log("Signo out of range.");
    sendError(
      "Pipe Test port: Signal number should be "
      "between 1 and 31",p_id);
    log("PIPEasp__PT::outgoing_send_PKill exited");
    return;
  }
  // killing process
  log("Killing process with signo: %d", signo);
  unsigned char msg[8];
  msg[0]=7;
  msg[1]=0;
  msg[2]=0;
  msg[3]=0;
  msg[4]=3;
  msg[5]= (p_id >> 8) & 0xFF;
  msg[6]= p_id & 0xFF;
  msg[7]= signo;
  write(processServerDown,msg,8);
}

void PIPEasp__PT::outgoing_send(const PIPEasp__Types::ASP__PKill& send_par) {
  log("PIPEasp__PT::outgoing_send_PKill called");

  handle_message(0,send_par);

  log("PIPEasp__PT::outgoing_send_PKill exited");
}

void PIPEasp__PT::outgoing_send(const PIPEasp__Types::ASP__PLineMode& send_par) {
  log("PIPEasp__PT::outgoing_send_PLineMode called");
  lineMode = (bool)send_par.lineMode();
  log("LineMode is set to %s", (lineMode)?"TRUE":"FALSE");
  log("PIPEasp__PT::outgoing_send_PLineMode exited");
}

void PIPEasp__PT::handle_message(const int p_id, const PIPEasp__Types::ASP__PEndOfInput& send_par) {

  if(!pipe_id_valid(p_id)){
    sendError("Pipe Test Port: Pipe execution id (p_id) is not valid",p_id);
    if(debugAllowed)
    {
    TTCN_Logger::begin_event(TTCN_DEBUG);
    TTCN_Logger::log_event("PIPE test port (%s): Pipe execution id (p_id=%d) is not valid: ", get_name(),p_id);
    send_par.log();
    TTCN_Logger::end_event();
    }
    return;
  }

  if (!main_data[p_id].processExecuting) {
    sendError("Pipe Test Port: No command executing",p_id);
    if(debugAllowed)
    {
    TTCN_Logger::begin_event(TTCN_DEBUG);
    TTCN_Logger::log_event("PIPE test port (%s): No command executing (p_id=%d). Following ASP is ignored: ", get_name(),p_id);
    send_par.log();
    TTCN_Logger::end_event();
    }
    return;
  }

  unsigned char msg[7];
  msg[0]=8;
  msg[1]=0;
  msg[2]=0;
  msg[3]=0;
  msg[4]=2;
  msg[5]= (p_id >> 8) & 0xFF;
  msg[6]= p_id & 0xFF;
  write(processServerDown,msg,7);
}

void PIPEasp__PT::outgoing_send(const PIPEasp__Types::ASP__PEndOfInput& send_par) {
  log("PIPEasp__PT::outgoing_send_PEndOfInput called");

  handle_message(0,send_par);

  log("PIPEasp__PT::outgoing_send_PEndOfInput exited");
}

void PIPEasp__PT::outgoing_send(const PIPEasp__Types::ASP__Parallel__Command& send_par) {
  log("PIPEasp__PT::ASP__Parallel__Command called");

  int p_id=send_par.p__id();
  if(p_id==0){
    sendError("Pipe Test Port: Pipe execution id (p_id) is not valid",p_id);
    if(debugAllowed)
    {
    TTCN_Logger::begin_event(TTCN_DEBUG);
    TTCN_Logger::log_event("PIPE test port (%s): Pipe execution id (p_id=%d) is not valid: ", get_name(),p_id);
    send_par.log();
    TTCN_Logger::end_event();
    }
    
  } else {
    switch (send_par.command().get_selection()) {
      case PIPEasp__Types::ASP__Commands::ALT_pexecute:
        handle_message(p_id,send_par.command().pexecute());
        break;
      case PIPEasp__Types::ASP__Commands::ALT_pexecutePty:
        handle_message(p_id,send_par.command().pexecutePty());
        break;
      case PIPEasp__Types::ASP__Commands::ALT_pexecuteBinary:
        handle_message(p_id,send_par.command().pexecuteBinary());
        break;
      case PIPEasp__Types::ASP__Commands::ALT_pexecuteBinaryPty:
        handle_message(p_id,send_par.command().pexecuteBinaryPty());
        break;
      case PIPEasp__Types::ASP__Commands::ALT_pexecuteBackground:
        handle_message(p_id,send_par.command().pexecuteBackground());
        break;
      case PIPEasp__Types::ASP__Commands::ALT_pexecuteBackgroundPty:
        handle_message(p_id,send_par.command().pexecuteBackgroundPty());
        break;
      case PIPEasp__Types::ASP__Commands::ALT_pStdin:
        handle_message(p_id,send_par.command().pStdin());
        break;
      case PIPEasp__Types::ASP__Commands::ALT_pStdinBinary:
        handle_message(p_id,send_par.command().pStdinBinary());
        break;
      case PIPEasp__Types::ASP__Commands::ALT_pKill:
        handle_message(p_id,send_par.command().pKill());
        break;
      case PIPEasp__Types::ASP__Commands::ALT_pEndOfInput:
        handle_message(p_id,send_par.command().pEndOfInput());
        break;
      default:
        break;
    }
  }
  log("PIPEasp__PT::ASP__Parallel__Command exited");
}
/***********************************
* if the the child process died, gets
* the exit code and sends it to TTCN
* should be called when stdout/err are closed
************************************/
void PIPEasp__PT::handle_childDeath(const int p_id) {
  log("Child process exit status is: %d", main_data[p_id].processExitCode);
  // send code to TTCN:
  sendExitCode(p_id);
  // send result to TTCN
  sendResult(p_id);

  main_data[p_id].processExecuting = false;
  main_data[p_id].disableSend = false;
  if(!main_data[p_id].reuse && !waitforpidrelease){
    main_data[p_id].state=0;
    Free(main_data[p_id].stdout_buffer);
    main_data[p_id].stdout_buffer=NULL;
    Free(main_data[p_id].stderr_buffer);
    main_data[p_id].stderr_buffer=NULL;
    maindata_used--;
  }
}

/***************************
* Send stdout msg to TTCN
***************************/
void PIPEasp__PT::sendStdout(const int p_id) {
  if (main_data[p_id].disableSend) return;
  

  PIPEasp__Types::ASP__PStdout message_PStdout;
  PIPEasp__Types::ASP__PStdoutBinary message_PStdoutBinary;
  if (lineMode && !main_data[p_id].binaryMode) {
    // send complete lines from buffer
    const unsigned char* pos = main_data[p_id].stdout_buffer->get_read_data();
    for(unsigned int i=0; i < main_data[p_id].stdout_buffer->get_read_len() ; i++) {
      // not end of line:
      if (pos[i] != '\n') {
        continue;
      }
      
      // at end of line
      // length of data is i (+1 is for \n and is not sent)
      message_PStdout.stdout_() = CHARSTRING(i, (const char*)pos);
      
      // send message
      if(p_id){ // p_id == 0 means old interface
        PIPEasp__Types::ASP__Parrallel__Result message_PR;
        message_PR.p__id()=p_id;
        message_PR.result().pStdout()=message_PStdout;
        incoming_message(message_PR);
      } else {
        incoming_message(message_PStdout);
      }
      // remove the complete line from buffer,
      // also set i and pos to the beginning of buffer
      main_data[p_id].stdout_buffer->set_pos(i+1);
      main_data[p_id].stdout_buffer->cut();
      i = 0;
      pos = main_data[p_id].stdout_buffer->get_read_data();
    }
  } else {
    // lineMode false or binaryMode true
    if (main_data[p_id].binaryMode) {
      message_PStdoutBinary.stdout_() =
        OCTETSTRING(main_data[p_id].stdout_buffer->get_read_len(), main_data[p_id].stdout_buffer->get_read_data());
      main_data[p_id].stdout_buffer->clear();
      if(p_id){ // p_id == 0 means old interface
        PIPEasp__Types::ASP__Parrallel__Result message_PR;
        message_PR.p__id()=p_id;
        message_PR.result().pStdoutBinary()=message_PStdoutBinary;
        incoming_message(message_PR);
      } else {
        incoming_message(message_PStdoutBinary);
      }
    }
    else {
      message_PStdout.stdout_() = 
        CHARSTRING(main_data[p_id].stdout_buffer->get_read_len(), (const char*)main_data[p_id].stdout_buffer->get_read_data());
      main_data[p_id].stdout_buffer->clear();
      if(p_id){ // p_id == 0 means old interface
        PIPEasp__Types::ASP__Parrallel__Result message_PR;
        message_PR.p__id()=p_id;
        message_PR.result().pStdout()=message_PStdout;
        incoming_message(message_PR);
      } else {
        incoming_message(message_PStdout);
      }
    }
//    incoming_message(message);
  }
}


/***************************
* Send stderr msg to TTCN
***************************/
void PIPEasp__PT::sendStderr(const int p_id) {
  if (main_data[p_id].disableSend) return;

  PIPEasp__Types::ASP__PStderr message_PStderr;
  PIPEasp__Types::ASP__PStderrBinary message_PStderrBinary;

  if (lineMode && !main_data[p_id].binaryMode) {
    // send complete lines from buffer
    const unsigned char* pos = main_data[p_id].stderr_buffer->get_read_data();
    for(unsigned int i=0; i<main_data[p_id].stderr_buffer->get_read_len(); i++) {
      // not end of line:
      if (pos[i] != '\n') {
        continue;
      }
      
      // at end of line
      // length of data is i (+1 is for \n and is not sent)
      message_PStderr.stderr_() = CHARSTRING(i, (const char*)pos);
      
      // send message
      if(p_id){ // p_id == 0 means old interface
        PIPEasp__Types::ASP__Parrallel__Result message_PR;
        message_PR.p__id()=p_id;
        message_PR.result().pStderr()=message_PStderr;
        incoming_message(message_PR);
      } else {
        incoming_message(message_PStderr);
      }
      // remove the complete line from buffer,
      // also set i and pos to the beginning of buffer
      main_data[p_id].stderr_buffer->set_pos(i+1);
      main_data[p_id].stderr_buffer->cut();
      i = 0;
      pos = main_data[p_id].stderr_buffer->get_read_data();
    }
  } else {
    // lineMode false or binaryMode true
    if (main_data[p_id].binaryMode) {
      message_PStderrBinary.stderr_() =
        OCTETSTRING(main_data[p_id].stderr_buffer->get_read_len(), main_data[p_id].stderr_buffer->get_read_data());
      main_data[p_id].stderr_buffer->clear();
      if(p_id){ // p_id == 0 means old interface
        PIPEasp__Types::ASP__Parrallel__Result message_PR;
        message_PR.p__id()=p_id;
        message_PR.result().pStedrrBinary()=message_PStderrBinary;
        incoming_message(message_PR);
      } else {
        incoming_message(message_PStderr);
      }
    }
    else {
      message_PStderr.stderr_() = 
        CHARSTRING(main_data[p_id].stderr_buffer->get_read_len(), (const char*)main_data[p_id].stderr_buffer->get_read_data());
      main_data[p_id].stderr_buffer->clear();
      if(p_id){ // p_id == 0 means old interface
        PIPEasp__Types::ASP__Parrallel__Result message_PR;
        message_PR.p__id()=p_id;
        message_PR.result().pStderr()=message_PStderr;
        incoming_message(message_PR);
      } else {
        incoming_message(message_PStderr);
      }
    }
//    incoming_message(message);
  }
}


/***************************
* Send exitcode msg to TTCN
***************************/
void PIPEasp__PT::sendExitCode(const int p_id) {
  if (main_data[p_id].disableSend) return;

  log("Sending ExitCode to TTCN");
  PIPEasp__Types::ASP__PExit message_PExit;
  message_PExit.code() = main_data[p_id].processExitCode;
  if(p_id){ // p_id == 0 means old interface
    PIPEasp__Types::ASP__Parrallel__Result message_PR;
    message_PR.p__id()=p_id;
    message_PR.result().pExit()=message_PExit;
      incoming_message(message_PR);
  } else {
    incoming_message(message_PExit);
  }
}


/***************************
* Send error msg to TTCN
***************************/
void PIPEasp__PT::sendError(const char* error_msg,const int p_id) {
  PIPEasp__Types::ASP__PError message_PError;
  message_PError.errorMessage() = error_msg;
  if(p_id){ // p_id == 0 means old interface
    PIPEasp__Types::ASP__Parrallel__Result message_PR;
    message_PR.p__id()=p_id;
    message_PR.result().pError()=message_PError;
      incoming_message(message_PR);
  } else {
    incoming_message(message_PError);
  }
}


/***************************
* Send Result msg to TTCN
***************************/
void PIPEasp__PT::sendResult(const int p_id) {
  if (!main_data[p_id].disableSend) return; // do not send result if process was started by PExecuteBackground
  
  log("Sending result to TTCN...");
  PIPEasp__Types::ASP__PResult message_PResult;
  PIPEasp__Types::ASP__PResultBinary message_PResultBinary;
  if (main_data[p_id].binaryMode) {
    message_PResultBinary.stdout_() =
     OCTETSTRING(main_data[p_id].stdout_buffer->get_read_len(), main_data[p_id].stdout_buffer->get_read_data());
    message_PResultBinary.stderr_() =
     OCTETSTRING(main_data[p_id].stderr_buffer->get_read_len(), main_data[p_id].stderr_buffer->get_read_data());
    message_PResultBinary.code() = main_data[p_id].processExitCode;
    if(p_id){ // p_id == 0 means old interface
      PIPEasp__Types::ASP__Parrallel__Result message_PR;
      message_PR.p__id()=p_id;
      message_PR.result().pResultBinary()=message_PResultBinary;
      incoming_message(message_PR);
    } else {
      incoming_message(message_PResultBinary);
    }
  } else {
    int messageLen = main_data[p_id].stdout_buffer->get_read_len();
    const char* messageData = (const char*)main_data[p_id].stdout_buffer->get_read_data();
    
    if (messageLen>0 && lineMode && messageData[messageLen-1]=='\n') {
      messageLen--; // remove newline from the end
    }
    
    message_PResult.stdout_() = CHARSTRING(messageLen, messageData);

    messageLen = main_data[p_id].stderr_buffer->get_read_len();
    messageData = (const char*)main_data[p_id].stderr_buffer->get_read_data();
    
    if (messageLen>0 && lineMode && messageData[messageLen-1]=='\n') {
      messageLen--; // remove newline from the end
    }
    
    message_PResult.stderr_() = CHARSTRING(messageLen, messageData);
    message_PResult.code() = main_data[p_id].processExitCode;
    if(p_id){ // p_id == 0 means old interface
      PIPEasp__Types::ASP__Parrallel__Result message_PR;
      message_PR.p__id()=p_id;
      message_PR.result().pResult()=message_PResult;
      incoming_message(message_PR);
    } else {
      incoming_message(message_PResult);
    }
  }

  // clearing the buffers
  main_data[p_id].stdout_buffer->clear();
  main_data[p_id].stderr_buffer->clear();
  //incoming_message(message);
}


////////////////
// Log function
////////////////
void PIPEasp__PT::log(const char *fmt, ...)
{ 
  if(debugAllowed)
  {
  TTCN_Logger::begin_event(TTCN_DEBUG);
  TTCN_Logger::log_event("PIPE test port (%s): ", get_name());
  va_list ap;
  va_start(ap, fmt);
  TTCN_Logger::log_event_va_list(fmt, ap);
  va_end(ap);
  TTCN_Logger::end_event();
  }
}


void PIPEasp__PT::log_buffer(const unsigned char * buf, size_t buflen)
{
 if(debugAllowed)
 {
    TTCN_logger.log(TTCN_DEBUG,"------------------------ PIPE test port (%s) Logging process buffer -----------------------------\n", get_name());

    TTCN_logger.log(TTCN_DEBUG,"Process buffer in hexstring format:\n");
    for(size_t j=0;j<buflen;) {
	TTCN_Logger::begin_event(TTCN_DEBUG);
	for(size_t i=0;j<buflen && i<16;i++) {
	    if(i == 8) TTCN_Logger::log_event("  ");
	    TTCN_Logger::log_event("%02x ", buf[j++]);
	}
	TTCN_Logger::end_event();
    }
    
    CHARSTRING cc = CHARSTRING((int)buflen, (const char*)buf);
    TTCN_Logger::begin_event(TTCN_DEBUG);
    TTCN_Logger::log_event("Process buffer in charstring format:\n");
    cc.log();
    TTCN_Logger::end_event();

    TTCN_logger.log(TTCN_DEBUG,"------------------------ End logging -----------------------------\n");
 }
}



int PIPEasp__PT::get_len(const TTCN_Buffer& buff){
  int buff_len=buff.get_len();
  if(buff_len<5) return -1; // not enough data in buffer
  const unsigned char* data=buff.get_data();
  int msg_len=(data[1]<<24)+(data[2]<<16)+(data[3]<<8)+data[4];
//printf("get_len,%d, %d, %d,%d,%d\n",buff_len, data[0], msg_len, data[1], data[2]);
  if(buff_len<(5+msg_len)) return -1; // partial message
  return msg_len;
}

int PIPEasp__PT::put_msg(TTCN_Buffer& buff, int msg_type, int len, const unsigned char* msg, const int p_id){
  buff.put_c(msg_type);
  int whole_len=len+7;
  buff.put_c(((len+2)>>24)&0xFF);
  buff.put_c(((len+2)>>16)&0xFF);
  buff.put_c(((len+2)>>8)&0xFF);
  buff.put_c((len+2)&0xFF);
  buff.put_c((p_id>>8)&0xFF);
  buff.put_c(p_id&0xFF);
  buff.put_s(len,msg);
  return whole_len;
}

void PIPEasp__PT::processHandler(){
  
  bool executing=true;
  int nBytes;
  int r;
  fd_set select_set;
  FD_ZERO(&working_set);
  FD_ZERO(&select_set);
//printf("PIPEasp__PT::processHandler() started %d\r\n",getpid());
  FD_SET(processServerDown, &working_set);
  max_fd=processServerDown;
  while(executing){
//printf("while(executing)\r\n");
/*    
    if(processStdout!=-1){
      FD_SET(processStdout, &readfds);
      max_fd=max_fd>processStdout?max_fd:processStdout;
    }
    if(processStderr!=-1){
      FD_SET(processStderr, &readfds);
      max_fd=max_fd>processStderr?max_fd:processStderr;
    }
    if(processPty!=-1){
      FD_SET(processPty, &readfds);
      max_fd=max_fd>processPty?max_fd:processPty;
    }*/
//printf("call select, %d\r\n",max_fd);
    select_set = working_set;
  
  
  
  
    int result = select(max_fd+1,&select_set,NULL,NULL,NULL);
//printf("result = select(max_fd+1,&readfds,NULL,NULL,NULL), %d,%d,%s\r\n",result,errno, strerror(errno));
    if(result<0){

      if(errno != EINTR) {
        close(processServerDown);
        close(processServerUp);
        return;
      }
    } else {
      if(FD_ISSET(processServerDown,&select_set)){
        result--;
        nBytes = PIPE_BUF_SIZE;
        unsigned char* buffer;
        size_t end_len = nBytes;
        process_buffer.get_end(buffer, end_len);
        r = read(processServerDown,buffer,(int)nBytes);
//printf("messages read from ttcn, %d\r\n",r);
        if(r>0){
          process_buffer.increase_length(r);
          int msg_len;
          int sig_no;
          //int a;
          int p_id;
          while((msg_len=get_len(process_buffer))!=-1){
            const unsigned char* msg=process_buffer.get_data();
//printf("messages from ttcn, %d, %d,%d,%d\n", msg[0], msg_len, msg[1], msg[2]);
            switch(msg[0]){
              case 1: // kill the server
                close(processServerDown);
                close(processServerUp);
                free_thread_data();
                return;
                break;
              case 2: // execute command
                 p_id=((int)msg[5]<<8) + (int)msg[6];
                 thread_data[p_id].ptyMode = false;
                 newcmd(p_id,CHARSTRING(msg_len-2,(const char*)msg+7));
                 break;
              case 10: // execute command in pty mode
                 p_id=((int)msg[5]<<8) + (int)msg[6];
                 thread_data[p_id].ptyMode = true;
                 newcmd_pty(p_id,CHARSTRING(msg_len-2,(const char*)msg+7));
                 break;
              case 3: // stdin or pty
                  p_id=((int)msg[5]<<8) + (int)msg[6];
                  if (!thread_data[p_id].ptyMode) {
                      if(thread_data[p_id].processStdin!=-1){
                          write(thread_data[p_id].processStdin,msg+7,msg_len-2);
                      }
                  } else {
                      if(thread_data[p_id].processPty!=-1){
                          write(thread_data[p_id].processPty,msg+7,msg_len-2);
                      }
                  }
                break;
              case 8: // end of input
                  p_id=((int)msg[5]<<8) + (int)msg[6];
                  if (!thread_data[p_id].ptyMode) {
                      close(thread_data[p_id].processStdin);
                      thread_data[p_id].processStdin=-1;
                  } else {
                      shutdown(thread_data[p_id].processPty,SHUT_WR);
                  }
                break;
              case 7: // kill command
                p_id=((int)msg[5]<<8) + (int)msg[6];
                sig_no=msg[7];
                kill(thread_data[p_id].pid,sig_no);
//printf("kill(%d,%d) returned %d\r\n", processPid,sig_no,a);
                break;
              case 9: // unmap command
                for(int i=0;i<threaddata_len;i++){
                  if(thread_data[i].processStdin!=-1){ 
                    close(thread_data[i].processStdin);
                    thread_data[i].processStdin=-1;
                  }
                  if(thread_data[i].processStdout!=-1){ 
                    close(thread_data[i].processStdout);
                    thread_data[i].processStdout=-1;
                  }
                  if(thread_data[i].processStderr!=-1){ 
                    close(thread_data[i].processStderr);
                    thread_data[i].processStderr=-1;
                  }
                  if(thread_data[i].processPty!=-1){ 
                    close(thread_data[i].processPty);
                    thread_data[i].processPty=-1;
                  }
                }
                break;

              case 11: // set echo mode for pty
                pty_echo=msg[5];
                break;
              
              case 12:
                p_id=((int)msg[5]<<8) + (int)msg[6];
                if(threaddata_len<=p_id){
                  int new_size=p_id+1;
                  thread_data=(pipedata_thread *)Realloc(thread_data,new_size*sizeof(pipedata_thread));
                  for(int i= threaddata_len;i<new_size;i++){
                    init_pipedata(i);
                  }
                  threaddata_len=new_size;
                }
                init_pipedata(p_id);
                break;
              default:
                break;
            }
            process_buffer.set_pos(5+msg_len);
            process_buffer.cut();
          }
//printf("messages processing end, %d\r\n",msg_len);
        } else {
          close(processServerDown);
          close(processServerUp);
          free_thread_data();
          return;
        }
      }
      for(int p_id=0;result>0 && p_id<threaddata_len;p_id++){
        if(thread_data[p_id].processStdout!=-1 && FD_ISSET(thread_data[p_id].processStdout,&select_set)){
          result--;
          nBytes = PIPE_BUF_SIZE;
          unsigned char buffer[PIPE_BUF_SIZE];
          r = read(thread_data[p_id].processStdout,buffer,(int)nBytes);
  //printf("processStdout end, %d\r\n",r);
          if(r>0){
            TTCN_Buffer msg;
            int msg_len=put_msg(msg,4,r,buffer,p_id);
            write(processServerUp,msg.get_data(),msg_len);
          } else {
            close_and_remove_fd(thread_data[p_id].processStdout);
            thread_data[p_id].processStdout=-1;
            if(thread_data[p_id].processStderr==-1) handle_childDeath_inprocess(p_id);
  //printf("handle_childDeath_inprocess end, %d \r\n", executing);
          }
        }
        if(thread_data[p_id].processStderr!=-1 && FD_ISSET(thread_data[p_id].processStderr,&select_set)){
          result--;
          nBytes = PIPE_BUF_SIZE;
          unsigned char buffer[PIPE_BUF_SIZE];
          r = read(thread_data[p_id].processStderr,buffer,(int)nBytes);
  //printf("processStderr end, %d\r\n",r);
          if(r>0){
            TTCN_Buffer msg;
            int msg_len=put_msg(msg,5,r,buffer,p_id);
            write(processServerUp,msg.get_data(),msg_len);
          } else {
            close_and_remove_fd(thread_data[p_id].processStderr);
            thread_data[p_id].processStderr=-1;
            if(thread_data[p_id].processStdout==-1) handle_childDeath_inprocess(p_id);
//  printf("handle_childDeath_inprocess end, \r\n");
          }
        }
        if(thread_data[p_id].processPty!=-1 && FD_ISSET(thread_data[p_id].processPty,&select_set)){
          result--;
          nBytes = PIPE_BUF_SIZE;
          unsigned char buffer[PIPE_BUF_SIZE];
          r = read(thread_data[p_id].processPty,buffer,(int)nBytes);
          //printf("processPty end, %d\r\n",r);
          if(r>0){
            TTCN_Buffer msg;
            // sent as STDIN
            int msg_len=put_msg(msg,4,r,buffer,p_id);
            write(processServerUp,msg.get_data(),msg_len);
          } else {
            close_and_remove_fd(thread_data[p_id].processPty);
            thread_data[p_id].processPty=-1;
            handle_childDeath_inprocess(p_id);
//  printf("handle_childDeath_inprocess end, %d \r\n", executing);
          }
        }
      }
    }
//printf("while end, %d\r\n",executing);

  }
}

void PIPEasp__PT::newcmd(const int p_id,const char* command){
  // creating pipes for process
  int pipesStdin[2];
  int pipesStdout[2];
  int pipesStderr[2];
  int processPid;	
  			    
  if (pipe(pipesStdin) != 0) {
      return;
  }
  if (pipe(pipesStdout) != 0) {
      return;
  }
  if (pipe(pipesStderr) != 0) {
      return;
  }
  
  thread_data[p_id].processStdin = pipesStdin[1];
  thread_data[p_id].processStdout = pipesStdout[0];
  thread_data[p_id].processStderr = pipesStderr[0];

  processPid = fork();
  if (processPid < 0) {
    //
    // Error
    //

    // close the pipes
    close(pipesStdin[0]);
    close(pipesStdout[1]);
    close(pipesStderr[1]);

    close(thread_data[p_id].processStdin);
    close(thread_data[p_id].processStdout);
    close(thread_data[p_id].processStderr);

  }
  else if (processPid == 0) {

      //
      // Child process
      //

      // close the parent end of the pipes
      close(thread_data[p_id].processStdin);
      close(thread_data[p_id].processStdout);
      close(thread_data[p_id].processStderr);

      int r;
      // redirect pipeStdin to stdin
      r = dup2(pipesStdin[0], 0);
      if (r<0) {
        exit(errno);
      }

      // redirect pipeStdout to stdout
      r = dup2(pipesStdout[1], 1);
      if (r<0) {
        exit(errno);
      }

      // redirect pipeStderr to stderr
      r = dup2(pipesStderr[1], 2);
      if (r<0) {
        exit(errno);
      }

      int processExitCode = execCommand(command);
      
      // There is a problem executing the command
      // Exiting...
      
      fflush(stdout);
      fflush(stderr);
      
      //closing pipes:
      close(pipesStdin[0]);
      close(pipesStdout[1]);
      close(pipesStderr[1]);
      
      exit(processExitCode); // end of child process
  }
  else {
					
    //
    // Parent process
    //
//printf("PIPEasp__PT::newcmd(%s) started, %d\r\n",command,processPid);

    // close child end of the pipes
    close(pipesStdin[0]);
    close(pipesStdout[1]);
    close(pipesStderr[1]);
    
    FD_SET(thread_data[p_id].processStdout,&working_set);
    max_fd=max_fd>thread_data[p_id].processStdout?max_fd:thread_data[p_id].processStdout;
    FD_SET(thread_data[p_id].processStderr,&working_set);
    max_fd=max_fd>thread_data[p_id].processStderr?max_fd:thread_data[p_id].processStderr;
    
    thread_data[p_id].processExecuting = true;
    thread_data[p_id].pid=processPid;
 }

}

void PIPEasp__PT::newcmd_pty(const int p_id,const char* command){

  int processPid;	

  processPid = forkpty(&thread_data[p_id].processPty,NULL,NULL,NULL);
  if (processPid < 0) {
    //
    // Error
    //
    thread_data[p_id].processPty=-1;

  }
  else if (processPid == 0) {

      //
      // Child process
      //

      int processExitCode;
      processExitCode = execCommand(command);
      exit(processExitCode); // end of child process

  }
  else {

    //
    // Parent process
    //
if(pty_echo){
struct termio tty, oldtty;
ioctl(thread_data[p_id].processPty, TCGETA, &oldtty);
tty = oldtty;
tty.c_lflag    &= ~ECHO;
ioctl(thread_data[p_id].processPty, TCSETA, &tty);
}
    FD_SET(thread_data[p_id].processPty,&working_set);
    max_fd=max_fd>thread_data[p_id].processPty?max_fd:thread_data[p_id].processPty;

    thread_data[p_id].processExecuting = true;
    thread_data[p_id].pid=processPid;

 }

}


/********************************
* Execute the given command
* returns the exitcode of the process
*********************************/
int PIPEasp__PT::execCommand(const char* command) {
//  log("PIPEasp__PT::execCommand called");
//  log("Executing command: %s", command);

  // with this it is not possible to access the pid of the process
  //return system(command);

  int argc = 0;
  char* argv[1024];

  CHARSTRING temp = "";
  for (int i = 0; command[i] != 0; i++) {
      if (isspace(command[i])) {
	  argv[argc++] = strdup(temp);
//          log("command argument added: %s", (const char*)temp);
	  while (command[i] != '0' && isspace(command[i])) i++;
	  i--;
	  temp = "";
      } else {
	  temp = temp + CHARSTRING(1, command+i);
      }
  }

  if (temp != "") {
      argv[argc++] = strdup(temp);
//      log("command argument added: %s", (const char*)temp);
  }

  argv[argc++] = (char*)NULL;

//  log("execCommand(%s,%d)\n", argv[0], argc);
  int retval=execvp(argv[0],argv);

//  f//
  int store_errno=errno;
fprintf(stderr,"Executing command failed (retval=%d) %s (%d): %s\r\n",retval, argv[0],store_errno , strerror(store_errno));
//  fflush(stderr);
//  exit(errno);
  return store_errno;
}


void PIPEasp__PT::handle_childDeath_inprocess(const int p_id) {

  thread_data[p_id].processExitCode = 0;  // reset the exitcode

  int pid = waitpid(thread_data[p_id].pid,&thread_data[p_id].processExitCode, 0);
  //printf("processExitCode %d\n",processExitCode);
  if (pid!=thread_data[p_id].pid) {
    return;
  }

  if (!thread_data[p_id].ptyMode) {
      if(thread_data[p_id].processStdin!=-1){
          close(thread_data[p_id].processStdin);
          thread_data[p_id].processStdin=-1;
      }
  } else {
      if(thread_data[p_id].processPty!=-1){
          close_and_remove_fd(thread_data[p_id].processPty);
          thread_data[p_id].processPty=-1;
      }
  }
  // send code to TTCN:
  unsigned char msg[11];
  msg[0]=6;
  msg[1]=0;
  msg[2]=0;
  msg[3]=0;
  msg[4]=6;
  msg[5]= (p_id >> 8) & 0xFF;
  msg[6]= p_id & 0xFF;
  
  msg[7]=(thread_data[p_id].processExitCode>>24)&0xFF;
  msg[8]=(thread_data[p_id].processExitCode>>16)&0xFF;
  msg[9]=(thread_data[p_id].processExitCode>>8)&0xFF;
  msg[10]=thread_data[p_id].processExitCode&0xFF;
  
  write(processServerUp,msg,11);
//printf("write\n");

  thread_data[p_id].processExecuting = false;
  thread_data[p_id].pid=-1;
}

void PIPEasp__PT::init_pipedata(int idx){
    thread_data[idx].processExecuting=false; // true if process is executing: disable new processes
    thread_data[idx].processStdin=-1;   // fd of stdin of the process
    thread_data[idx].processStdout=-1;  // fd of stdout of the process
    thread_data[idx].processStderr=-1;  // fd of stderr of the process

    thread_data[idx].pid=-1;
    thread_data[idx].ptyMode=false;       // pty mode
    thread_data[idx].processPty=-1;     // pty of the process (in pty mode)
    thread_data[idx].processExitCode=-1;       // exit code of the process
}
void PIPEasp__PT::close_and_remove_fd(int fd){
  close(fd);
  FD_CLR(fd,&working_set);
  if(fd==max_fd){
    max_fd=processServerDown;
    for(int i=0;i<threaddata_len;i++){
      if(thread_data[i].processStdout!=-1 && thread_data[i].processStdout!=fd){
        max_fd=max_fd>thread_data[i].processStdout?max_fd:thread_data[i].processStdout;
      }
      if(thread_data[i].processStderr!=-1 && thread_data[i].processStderr!=fd){
        max_fd=max_fd>thread_data[i].processStderr?max_fd:thread_data[i].processStderr;
      }
      if(thread_data[i].processPty!=-1 && thread_data[i].processPty!=fd){
        max_fd=max_fd>thread_data[i].processPty?max_fd:thread_data[i].processPty;
      }
    }
  }
}

INTEGER f__PIPE__request__p__id(PIPEasp__PortType::PIPEasp__PT& pl__port, BOOLEAN const&pl__reuse){
  int return_value=-1;
  
  if((pl__port.maindata_used+1)>pl__port.maindata_len){
    return_value=pl__port.maindata_len;
    pl__port.maindata_len++;
    pl__port.main_data=(PIPEasp__PT::pipedata_main*)Realloc(pl__port.main_data,pl__port.maindata_len*sizeof(PIPEasp__PT::pipedata_main));
  } else {
    for(int i=0;i<pl__port.maindata_len;i++){
      if(!pl__port.main_data[i].state){
        return_value=i;
        break;
      }
    }
  }


  pl__port.maindata_used++;
  pl__port.setup_pipedata_main(return_value);
  pl__port.main_data[return_value].reuse=pl__reuse;
  pl__port.send_p_id_to_thread(return_value);
  return return_value;
}

BOOLEAN f__PIPE__release__p__id(PIPEasp__PortType::PIPEasp__PT& pl__port, INTEGER const&pl__pid){
  int p_id=(int)pl__pid;
  if( (p_id>=0) && (p_id<pl__port.maindata_len) && pl__port.main_data[p_id].state && !pl__port.main_data[p_id].processExecuting){
    pl__port.main_data[p_id].state=0;
    if(pl__port.main_data[p_id].stdout_buffer) {
      Free(pl__port.main_data[p_id].stdout_buffer);
      pl__port.main_data[p_id].stdout_buffer=NULL;
    }
    if(pl__port.main_data[p_id].stderr_buffer) {
      Free(pl__port.main_data[p_id].stderr_buffer);
      pl__port.main_data[p_id].stderr_buffer=NULL;
    }
    pl__port.maindata_used--;
    return true;
  }

  return false;
}

}//namespace
