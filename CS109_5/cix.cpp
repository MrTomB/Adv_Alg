/*---------------------------
Thomas Burch (tburch1@ucsc.edu)
CMPS 109 Program 5
Client/Server and Sockets
8/11/15
------------------------------*/

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
//ADDED
#include <fstream>
#include <sstream>

using namespace std;

#include <libgen.h>
#include <sys/types.h>
#include <unistd.h>

#include "protocol.h"
#include "logstream.h"
#include "sockets.h"

logstream log (cout);
struct cix_exit: public exception {};

unordered_map<string,cix_command> command_map {
   {"exit", CIX_EXIT},
   {"help", CIX_HELP},
   {"ls"  , CIX_LS  },
   //ADDED
   {"get", CIX_GET},
   {"put", CIX_PUT},
   {"rm", CIX_RM},
};

void cix_help() {
   static vector<string> help = {
      "exit         - Exit the program.  Equivalent to EOF.",
      "get filename - Copy remote file to local host.",
      "help         - Print help summary.",
      "ls           - List names of files on remote server.",
      "put filename - Copy local file to remote host.",
      "rm filename  - Remove file from remote server.",
   };
   for (const auto& line: help) cout << line << endl;
}

void cix_ls (client_socket& server) {
   cix_header header;
   header.command = CIX_LS;
   log << "sending header " << header << endl;
   send_packet (server, &header, sizeof header);
   recv_packet (server, &header, sizeof header);
   log << "received header " << header << endl;
   if (header.command != CIX_LSOUT) {
      log << "sent CIX_LS, server did not return CIX_LSOUT" << endl;
      log << "server returned " << header << endl;
   }else {
      char buffer[header.nbytes + 1];
      recv_packet (server, buffer, header.nbytes);
      log << "received " << header.nbytes << " bytes" << endl;
      buffer[header.nbytes] = '\0';
      cout << buffer;
   }
}

//ADDED----------------------------------------
void cix_get(client_socket& server, string filename){
    cix_header header;
    header.command = CIX_GET;
    if(filename.size() >= FILENAME_SIZE){
        log << "get: " << filename << 
        ": filename too large" << endl;
        return;
    }

    strcpy(header.filename, filename.c_str()); 
    header.nbytes = 0;

    //sent and received
    log << "sending header " << header << endl;
    send_packet (server, &header, sizeof header);
    recv_packet (server, &header, sizeof header);
    log << "received header " << header << endl;

    //server errors
    if(header.command != CIX_FILE){
        log << "sent CIX_GET, server did not return CIX_FILE"
         << endl;
        log << "server returned " << header << endl;
    }else{
        ofstream output_stream(filename);
        char buffer[header.nbytes + 1];
        recv_packet (server, buffer, header.nbytes);
        log << "received " << header.nbytes
         << " bytes" << endl;
        buffer[header.nbytes] = '\0';
        output_stream.write(buffer, header.nbytes);
        output_stream.close();
        cout << "Successfully saved file to local directory"
         << endl;
    }
}

void cix_put(client_socket& server, string filename) {
   cix_header header;
   header.command = CIX_PUT;
   if(filename.size() >= FILENAME_SIZE){
        log << "put: " << filename << 
        ": filename too large" << endl;
        return;
   }

   ifstream fs(filename);
   stringstream sstr;
   sstr << fs.rdbuf();
   string data = sstr.str();
   strcpy(header.filename, filename.c_str()); 
   header.nbytes = data.size();
   log << "sending header " << header << endl;
   send_packet (server, &header, sizeof header);
   send_packet (server, data.c_str(), data.size());
   recv_packet (server, &header, sizeof header);
   log << "received header " << header << endl;
   if(header.command != CIX_ACK){
      log << "sent CIX_PUT, server did not return CIX_ACK"
       << endl;
      log << "server returned " << header << endl;
   }else{
   cout << "Successfully put file onto server" << endl;
   }   
}

void cix_rm(client_socket& server, string filename){
    cix_header header;
    header.command = CIX_RM;
    if(filename.size() >= FILENAME_SIZE){
        log << "rm: " << filename 
        << ": filename too large" << endl;
        return;
    }
    strcpy(header.filename, filename.c_str()); 
    header.nbytes = 0;
    log << "sending header " << header << endl;
    send_packet (server, &header, sizeof header);
    recv_packet (server, &header, sizeof header);
    log << "received header " << header << endl;
    if(header.command != CIX_ACK){
        log << "sent CIX_RM, server did not return CIX_ACK" 
        << endl;
        log << "server returned " << header << endl;
    }else{
      cout << "Successfully unlinked file from the server"
       << endl;
    }
}

string trim(const string& str){
  size_t first = str.find_first_not_of (" \t");
   if (first == string::npos) return ""; 
   size_t last = str.find_last_not_of (" \t");
   return str.substr (first, last - first + 1);
}

bool SIGINT_throw_cix_exit {false};
void signal_handler (int signal){
  log << "signal_handler: caught " 
  << strsignal (signal) << endl;
  switch (signal){
    case SIGINT: 
    case SIGTERM: 
      SIGINT_throw_cix_exit = true; 
      break;
    default: break;
  }
}
//----------------------------------------

void usage() {
   cerr << "Usage: " << log.execname() << " [host] [port]" << endl;
   throw cix_exit();
}

int main (int argc, char** argv) {
   log.execname (basename (argv[0]));
   log << "starting" << endl;
   vector<string> args (&argv[1], &argv[argc]);
   if (args.size() > 2) usage();
   string host = get_cix_server_host (args, 0);
   in_port_t port = get_cix_server_port (args, 1);
   log << to_string (hostinfo()) << endl;
   try {
      log << "connecting to " << host << " port " << port << endl;
      client_socket server (host, port);
      log << "connected to " << to_string (server) << endl;
      for (;;) {
         string line;
         getline (cin, line);
         if (cin.eof()) throw cix_exit();
         log << "command " << line << endl;


//ADDED----------------------------------
          string cmd_key;
          string filename;
          string command = trim(line);
          //size_t end = command.size();
          size_t space = command.find(" ");
          if(space != string::npos) {
              cmd_key = command.substr(0, space);
              filename = command.substr(space + 1); // end);
          }else{
              cmd_key = command;
          }
        const auto& itor = command_map.find (cmd_key);
        cix_command cmd = itor == command_map.end()
                         ? CIX_ERROR : itor->second;
//----------------------------------
        /*
        const auto& itor = command_map.find (line);
        cix_command cmd = itor == command_map.end()
                         ? CIX_ERROR : itor->second;
        */
         switch (cmd) {
            case CIX_EXIT:
               throw cix_exit();
               break;
            case CIX_HELP:
               cix_help();
               break;
            case CIX_LS:
               cix_ls (server);
               break;
//ADDED Functions ------------------
            case CIX_GET:
              if (filename == "") {
              log << "usage: get filename" << endl;
              break;
              }
               cix_get (server, filename);
               break;
            case CIX_PUT:
                if (filename == "") {
                log << "usage: put filename" << endl;
                break;
                }
               cix_put (server, filename);
               break; 
            case CIX_RM:
              if (filename == "") {
              log << "usage: rm filename" << endl;
              break;
              }             
              cix_rm (server, filename);
              break;
// ------------------------------------
            default:
               //log << line << ": invalid command" << endl;
               log << command << ": invalid command" << endl;
               break;
         }
      }
   }catch (socket_error& error) {
      log << error.what() << endl;
   }catch (cix_exit& error) {
      log << "caught cix_exit" << endl;
   }
   log << "finishing" << endl;
   return 0;
}