TEMPLATE = subdirs
SUBDIRS = csb demo tests

demo.depend = csb
tests.depend = csb