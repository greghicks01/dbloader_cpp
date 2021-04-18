# dbloader_cpp

Example of C++ Threading via ODBC for interoperability of databases on the same server.

## Motivation

The group I worked for decided to purchase a new server for a Solid Server RDBMS, I suggested we use the server for a Sybase Anywhere Server as well because:
* there are only 70 people who use this across the network
* we would maintain control over the server

To confirm interoperatbility I wrote a workcrew threaded model to connect via ODBC and provide 

# Development process
* Identify the way to connect to both DB's from ODBC
* Utilise installed ODBC connections by interrogating the machines Local and machine DSN entries
* Use a threaded workcrew model to provide the connection and run SQL statements to simulate users
* Log the timings and relevant data for review of the performance, errors for analysis

# What I learned doing this
* How to interrogate the registry DSN entries
* How to use critical sections to eusure logs were not mixed up
* Writing threads that pause for a time before continuing
* Writing threads the respond to a terminate request (either from reaching max time or user initiated)
* Writing SQL for the Solid and Sybase servers system tables
* Setting up a flexible number of threads (1 to 50) for each DSN
* Handing and trapping errors within a thread
* writing a singleton for log and workcrew factory threads
* OBDC connections
* Windows forms for this application
* Debugging, compiling and creating installer pacakge
