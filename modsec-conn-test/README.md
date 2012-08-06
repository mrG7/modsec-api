Mod Security Filter
===================
To run the java application, make sure that
1. LD_LIBRARY_PATH includes the path to standalone.so
2. CLASSPATH includes ./bin
3. Provide the JVM argument java.library.path=</path/to/libmodsecurity.so>

Run as
$ java -classpath bin -Djava.library.path=</path/to/libmodsecurity.so> vulnapp.main.ProcessRequest </path/to/configfile> </path/to/eventfile>
