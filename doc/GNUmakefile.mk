###############################################################################
##
##  makefile for documentation creation
##
##  INTPUT FILES (SOURCE/doc)
##      doc/
##          dox/            <-- generated API docs support files
##          doxygen/        <-- generated API docs support files
##          texi/           <-- hand-written project articles
##
##  OUTPUT FILES (BUILD/doc)
##      doc/
##      doc/
##          api/
##              html/   <-- API in html format
##              xml/    <-- API in xml format
##          articles/
##              html/   <-- articles in html format
##              txt/    <-- articles in txt format
##              wiki/   <-- articles in Google Code Wiki format
##              xml/    <-- articles in xml-texinfo format
##
###############################################################################

SOURCE/ = $(top_srcdir)/
BUILD/  = $(top_builddir)/

DOC.in/  = $(SOURCE/)doc/
DOC.out/ = $(BUILD/)doc/

DOC.out.api/      = $(DOC.out/)api/
DOC.out.articles/ = $(DOC.out/)articles/

###############################################################################

MAKEINFO.flags      = -I$(DOC.in/)texi -I$(DOC.out/)texi
MAKEINFO.flags.html = --html --no-headers --no-split
MAKEINFO.flags.txt  = --plaintext --no-headers
MAKEINFO.flags.xml  = --xml --output-indent=4
MAKEINFO.cmd        = makeinfo $(MAKEINFO.flags) $(MAKEINFO.flags.$(1)) $(2) -o $(3)

XML2WIKI       = $(SOURCE/)project/xml2wiki.py
XML2WIKI.flags = --date --toc
XML2WIKI.cmd   = $(XML2WIKI) $(XML2WIKI.flags) $(1) > $(2)
XML2WIKI.deps  = $(XML2WIKI)

DOXYGEN     = doxygen
DOXYGEN.cmd = $(DOXYGEN) $(1)

###############################################################################

DOC.texi.articles = $(wildcard $(DOC.in/)texi/*.texi $(DOC.out/)texi/*.texi)

DOC.texi.includes = \
    $(DOC.out/)texi/base/project.texi \
    $(wildcard $(DOC.in/)texi/*/*.texi $(DOC.out/)texi/*/*.texi)

DOC.texi2html.out = $(DOC.texi.articles:$(DOC.in/)texi/%.texi=$(DOC.out.articles/)html/%.html)
DOC.texi2txt.out  = $(DOC.texi.articles:$(DOC.in/)texi/%.texi=$(DOC.out.articles/)txt/%.txt)
DOC.texi2xml.out  = $(DOC.texi.articles:$(DOC.in/)texi/%.texi=$(DOC.out.articles/)xml/%.xml)

DOC.xml2wiki.out  = $(DOC.texi2xml.out:$(DOC.out.articles/)xml/%.xml=$(DOC.out.articles/)wiki/%.wiki)

DOC.api.out = $(DOC.out.api/).stamp

MKDIRS += $(foreach n,html man texi txt wiki xml,$(DOC.out.articles/)$n)
MKDIRS += $(DOC.out.api/)

###############################################################################

EXTRA_DIST += $(SOURCE/)doc/GNUmakefile.mk

EXTRA_DIST += \
    $(DOC.in/)texi/base/project.texi.in \
    $(DOC.texi.articles) \
    $(DOC.texi.includes)

EXTRA_DIST += \
    $(SOURCE/)vstudio9.0/include/mp4v2/project.h \
    $(SOURCE/)vstudio9.0/mp4v2.sln \
    $(wildcard $(SOURCE/)vstudio9.0/*/*.vcproj)

###############################################################################

distclean-local: docclean

.PHONY: articles doc
articles: html txt xml wiki
doc: articles api

.PHONY: articlesclean apiclean docclean
articlesclean: htmlclean txtclean xmlclean wikiclean
docclean: articlesclean apiclean

.PHONY: html txt xml wiki doxygen dox
html: $(DOC.texi2html.out)
txt: $(DOC.texi2txt.out)
xml: $(DOC.texi2xml.out)
wiki: $(DOC.xml2wiki.out)
api: $(DOC.api.out)

.PHONY: htmlclean txtclean xmlclean wikiclean doxygenclean doxclean
htmlclean:
	rm -f $(DOC.texi2html.out)

txtclean:
	rm -f $(DOC.texi2txt.out)

xmlclean:
	rm -f $(DOC.texi2xml.out)

wikiclean:
	rm -f $(DOC.xml2wiki.out)

apiclean:
	rm -f $(DOC.api.out)
	rm -fr $(DOC.out.articles/)api/html/ $(DOC.out.articles/)api/xml/

###############################################################################

$(DOC.texi2html.out): $(DOC.texi.includes) | $(dir $(DOC.texi2html.out))
$(DOC.texi2html.out): $(DOC.out.articles/)html/%.html: $(DOC.in/)texi/%.texi
	$(call MAKEINFO.cmd,html,$<,$@)

$(DOC.texi2txt.out): $(DOC.texi.includes) | $(dir $(DOC.texi2txt.out))
$(DOC.texi2txt.out): $(DOC.out.articles/)txt/%.txt: $(DOC.in/)texi/%.texi
	$(call MAKEINFO.cmd,txt,$<,$@)

$(DOC.texi2xml.out): $(DOC.texi.includes) | $(dir $(DOC.texi2xml.out))
$(DOC.texi2xml.out): $(DOC.out.articles/)xml/%.xml: $(DOC.in/)texi/%.texi
	$(call MAKEINFO.cmd,xml,$<,$@)

$(DOC.xml2wiki.out): $(XML2WIKI.deps) | $(dir $(DOC.xml2wiki.out))
$(DOC.xml2wiki.out): $(DOC.out.articles/)wiki/%.wiki: $(DOC.out.articles/)xml/%.xml
	$(call XML2WIKI.cmd,$<,$@)

$(DOC.api.out): | $(dir $(DOC.api.out))
$(DOC.api.out): $(DOC.out/)doxygen/Doxyfile
	$(call DOXYGEN.cmd,$<)
	touch $@

###############################################################################

$(sort $(MKDIRS)):
	$(MKDIR_P) $@
