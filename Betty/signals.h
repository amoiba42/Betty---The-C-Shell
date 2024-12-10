#ifndef __SIGNALS_H__
#define __SIGNALS_H__

void handle_CtrlD();
void handle_CtrlC(int sig);
void handle_CtrlZ(int sig);

void childsignal(int sig);
void sendsignal(int pid, int signal);

#endif