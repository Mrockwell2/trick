#!/usr/bin/python3
import sys
import socket
from variable_server import VariableServer

# Process the command line arguments.
if ( len(sys.argv) == 2) :
    trick_varserver_port = int(sys.argv[1])
else :
    print( "Usage: vsclient <port_number>")
    sys.exit()

# Connect to the variable server.
# client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
# client_socket.connect( ("localhost", trick_varserver_port) )
# insock = client_socket.makefile("r")

# client_socket.send( b"trick.var_pause()\n" )
# client_socket.send( b"trick.var_ascii()\n" )

vs = VariableServer("localhost", trick_varserver_port)

# Get the number of tics per second (just once).
# client_socket.send( b"trick.var_add(\"trick_sys.sched.time_tic_value\")\n" )
# client_socket.send( b"trick.var_send()\n" )
# line = insock.readline()
# field = line.split("\t")
# tics_per_second = int(field[1]);
# client_socket.send( b"trick.var_clear()\n" )
tics_per_second = vs.get_value("trick_sys.sched.time_tic_value", type_=int)
print(tics_per_second)

time_tics = vs.get_value("trick_sys.sched.time_tics", type_=int)
print(time_tics)

sim_time = float(time_tics) / tics_per_second
print(sim_time)

# Get the number of time_tics, and whatever else you want to recieve periodically.
# client_socket.send( b"trick.var_add(\"trick_sys.sched.time_tics\") \n" )

# Start the flow of data from the variable server.
# client_socket.send( b"trick.var_unpause()\n" )

# Repeatedly read and process the responses from the variable server.
# while(True):
#     line = insock.readline()
#     if line == '':
#         break

#     field = line.split("\t")
#     time_tics = int(field[1]);
    
#     # Calculate sim_time 
#     sim_time = float(time_tics) / tics_per_second

#     print(f'sim_time = {sim_time}')
