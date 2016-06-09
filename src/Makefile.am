SUBDIRS = test_endpoint

CFLAGS = std=c++11 -Wall -pedantic

bin_PROGRAMS = jarvis
jarvis_SOURCES = main.cc database.cc net.cc parser.cc
#jarvis_LDADD = -lmysqlclient
jarvis_CPPFLAGS = -I/usr/include/mysql -I../include
