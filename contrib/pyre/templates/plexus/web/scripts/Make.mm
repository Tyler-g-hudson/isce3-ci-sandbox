# -*- Makefile -*-
#
# authors:
#   {project.authors}
#
# (c) {project.span} all rights reserved
#

# project settings
PROJECT = {project.name}
# the package
PACKAGE = scripts

# the location
EXPORT_WEBDIR = $(EXPORT_ROOT)/web/www/$(PROJECT)/$(PACKAGE)
# the exported items
EXPORT_WEB = *.js

# targets
all: export

export:: export-web

live: live-web-package

# end of file
