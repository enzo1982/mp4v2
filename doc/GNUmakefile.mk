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
##          man/
##              man1/   <-- man-pages for utilities
##
###############################################################################

VPATH =

vpath COPYING       $(top_srcdir)
vpath INSTALL       $(top_srcdir)
vpath README        $(top_srcdir)
vpath autoaux/%     $(top_srcdir)
vpath %.cpp         $(top_srcdir)
vpath %.h           $(top_srcdir)
vpath %.m4          $(top_srcdir)
vpath %.1           $(top_srcdir)

###############################################################################

SOURCE/ = $(top_srcdir)/
BUILD/  = $(top_builddir)/

DOC.in/  = $(SOURCE/)doc/
DOC.out/ = $(BUILD/)doc/

DOC.out.api/      = $(DOC.out/)api/
DOC.out.articles/ = $(DOC.out/)articles/
DOC.out.site/     = $(DOC.out/)site/

###############################################################################

M4       = m4
M4.flags = -I$(BUILD/)project
M4.cmd   = $(M4) $(M4.flags) $(1) > $(2)
M4.deps  = $(BUILD/)project/project.m4

HELP2MAN       = help2man
HELP2MAN.flags = -m "$(PROJECT_name) Utilities" -N
HELP2MAN.cmd   = $(HELP2MAN) $(HELP2MAN.flags) ./$(1) -o $(2)

MAKEINFO.flags      = -I$(DOC.in/)texi -I$(DOC.out/)texi
MAKEINFO.flags.html = --html --no-headers --no-split
MAKEINFO.flags.txt  = --plaintext --no-headers
MAKEINFO.flags.xml  = --xml --output-indent=4
MAKEINFO.cmd        = makeinfo $(MAKEINFO.flags) $(MAKEINFO.flags.$(1)) $(2) -o $(3)

HTMLCOMBINE = $(SOURCE/)project/htmlcombine.py
HTMLCOMBINE.cmd   = $(HTMLCOMBINE) --header $(1) --footer $(2) --body $(3) > $(4)
HTMLCOMBINE.deps  = $(HTMLCOMBINE)

XML2WIKI       = $(SOURCE/)project/xml2wiki.py
XML2WIKI.flags = --date --toc
XML2WIKI.cmd   = $(XML2WIKI) $(XML2WIKI.flags) $(1) > $(2)
XML2WIKI.deps  = $(XML2WIKI)

DOXYGEN     = doxygen
DOXYGEN.cmd = $(DOXYGEN) $(1)

###############################################################################

DOC.m4.out = \
    doc/texi/base/project.texi \
    doc/doxygen/Doxyfile       \
    doc/doxygen/header.html    \
    doc/doxygen/footer.html    \
    doc/html/header.html       \
    doc/html/footer.html

DOC.man.utils = mp4art mp4dump mp4optimize mp4track
DOC.man.out   = $(DOC.man.utils:%=$(DOC.out/)man/man1/%.1)

DOC.texi.articles = $(wildcard $(DOC.in/)texi/*.texi $(DOC.out/)texi/*.texi)

DOC.texi.includes = \
    doc/texi/base/project.texi \
    $(wildcard $(DOC.in/)texi/*/*.texi $(DOC.out/)texi/*/*.texi)

DOC.texi2html.out = $(DOC.texi.articles:$(DOC.in/)texi/%.texi=$(DOC.out.articles/)html/%.html)
DOC.texi2txt.out  = $(DOC.texi.articles:$(DOC.in/)texi/%.texi=$(DOC.out.articles/)txt/%.txt)
DOC.texi2xml.out  = $(DOC.texi.articles:$(DOC.in/)texi/%.texi=$(DOC.out.articles/)xml/%.xml)

DOC.xml2wiki.out  = $(DOC.texi2xml.out:$(DOC.out.articles/)xml/%.xml=$(DOC.out.articles/)wiki/%.wiki)

DOC.api.out = $(DOC.out.api/).stamp

DOC.site.out       = $(DOC.out.site/).stamp
DOC.site.out.copy  = $(patsubst $(DOC.in/)html/%,$(DOC.out.site/)%, \
    $(filter-out %.in,$(wildcard $(DOC.in/)html/*)))
DOC.site.out.index = $(DOC.out.site/)index.html
DOC.site.out.html  = $(patsubst $(DOC.out.articles/)html/%,$(DOC.out.site/)%, \
    $(filter-out %/Documentation.html,$(DOC.texi2html.out)))

###############################################################################

MKDIRS += $(dir $(DOC.m4.out))
MKDIRS += $(DOC.out/)man/man1/
MKDIRS += $(foreach n,html man texi txt wiki xml,$(DOC.out.articles/)$n)
MKDIRS += $(DOC.out.api/)
MKDIRS += $(DOC.out.site/)

###############################################################################

EXTRA_DIST += \
    $(SOURCE/)vstudio9.0/include/mp4v2/project.h \
    $(SOURCE/)vstudio9.0/mp4v2.sln \
    $(wildcard $(SOURCE/)vstudio9.0/*/*.vcproj)

###############################################################################

dist-hook: $(DOC.texi2txt.out) $(DOC.man.out)
	rm -fr $(distdir)/doc
	$(MKDIR_P) $(distdir)/doc
	$(INSTALL_DATA) $(DOC.texi2txt.out) $(distdir)/doc/.
	$(MKDIR_P) $(distdir)/doc/man/man1
	$(INSTALL_DATA) $(DOC.man.out) $(distdir)/doc/man/man1

distclean-local: docclean

###############################################################################

$(DOC.m4.out): %: %.m4 $(M4.deps) | $(dir $(DOC.m4.out))
	$(call M4.cmd,$<,$@)

###############################################################################

.PHONY: articles doc
articles: html txt xml wiki
doc: man articles api site

.PHONY: articlesclean apiclean docclean
articlesclean: htmlclean txtclean xmlclean wikiclean
docclean: manclean articlesclean apiclean siteclean

.PHONY: man html txt xml wiki api
man: $(DOC.man.out)
html: $(DOC.texi2html.out)
txt: $(DOC.texi2txt.out)
xml: $(DOC.texi2xml.out)
wiki: $(DOC.xml2wiki.out)
api: $(DOC.api.out)
site: $(DOC.site.out)

.PHONY: manclean htmlclean txtclean xmlclean wikiclean apiclean
manclean:
	rm -f $(DOC.man.out)

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
	rm -fr $(DOC.out.api/)html/ $(DOC.out.api/)xml/

siteclean:
	rm -fr $(DOC.out.site/)

###############################################################################

$(DOC.man.out): | $(dir $(DOC.man.out))
$(DOC.man.out): $(DOC.out/)man/man1/%.1: $(BUILD/)%
	$(call HELP2MAN.cmd,$<,$@)

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
$(DOC.api.out): $(DOC.in/)doxygen/banner.png
$(DOC.api.out): $(DOC.in/)doxygen/project.css
$(DOC.api.out): $(DOC.out/)doxygen/header.html
$(DOC.api.out): $(DOC.out/)doxygen/footer.html
$(DOC.api.out): $(DOC.out/)doxygen/Doxyfile
	$(call DOXYGEN.cmd,$<)
	$(INSTALL_DATA) $(DOC.in/)doxygen/banner.png $(DOC.out.api/)html/
	touch $@

###############################################################################

$(DOC.site.out): | $(DOC.out.site/)articles
$(DOC.site.out): | $(DOC.out.site/)api
$(DOC.site.out): $(DOC.site.out.copy)
$(DOC.site.out): $(DOC.site.out.index)
$(DOC.site.out): $(DOC.site.out.html)

$(DOC.out.site/)articles: | html $(DOC.out.site/)
	rm -f $@
	ln -s ../articles/html $@

$(DOC.out.site/)api: | api $(DOC.out.site/)
	rm -f $@
	ln -s ../api/html $@

$(DOC.site.out.copy): $(DOC.out.site/)%: $(DOC.in/)html/%
	$(INSTALL_DATA) $^ $(@D)/.

$(DOC.site.out.index): $(DOC.out/)html/header.html $(DOC.out/)html/footer.html
$(DOC.site.out.index): $(HTMLCOMBINE.deps)
$(DOC.site.out.index): $(DOC.out.articles/)html/Documentation.html
	$(call HTMLCOMBINE.cmd,$(word 2,$^),$(word 3,$^),$<,$@)

$(DOC.site.out.html): $(DOC.out/)html/header.html $(DOC.out/)html/footer.html
$(DOC.site.out.html): $(HTMLCOMBINE.deps)
$(DOC.site.out.html): $(DOC.out.site/)%: $(DOC.out.articles/)html/%
	$(call HTMLCOMBINE.cmd,$(word 2,$^),$(word 3,$^),$<,$@)

###############################################################################

GOOGLE.out/      = $(top_builddir)/google/
GOOGLE.repo.doc  = $(if $(filter release,$(PROJECT_repo_type)),$(PROJECT_version),trunk)

google.post: site wiki
google.post: | $(GOOGLE.out/)
google.post: google.rsync google.rm google.add google.propset

$(GOOGLE.out/):
	svn co --depth immediates $(PROJECT_repo_root) $(GOOGLE.out/)
	svn update --set-depth immediates google/wiki
	svn update --set-depth infinity google/doc/$(GOOGLE.repo.doc)

google.rsync:
	rsync -vrptPL --delete \
	    --exclude=".svn/*" --exclude=".svn" \
	    --exclude="*/.svn/*" --exclude="*/.svn" \
	    --exclude=".stamp" --exclude="*/.stamp" \
	    --exclude="*.m4" --exclude="*/*.m4" \
	    $(DOC.out.site/). $(GOOGLE.out/)doc/$(GOOGLE.repo.doc)/.
	$(INSTALL_DATA) $(DOC.out.articles/)wiki/BuildRepository.wiki $(GOOGLE.out/)wiki/.
	$(INSTALL_DATA) $(DOC.out.articles/)wiki/BuildSource.wiki $(GOOGLE.out/)wiki/.

google.rm:
	@files=`svn status $(GOOGLE.out/) | grep '^!' | awk '{ print $$2 }'`; \
	if [ -n "$$files" ]; then \
	    svn rm $$files; \
	fi

google.add:
	@files=`svn status $(GOOGLE.out/) | grep '^?' | awk '{ print $$2 }'`; \
	if [ -n "$$files" ]; then \
	    svn add $$files; \
	fi

google.propset:
	find $(GOOGLE.out/)doc/$(GOOGLE.repo.doc) -type f -a \( -name "*.html" -o -name "*.css" \) -print0 \
	    | xargs -0 svn propset svn:eol-style native
	find $(GOOGLE.out/)doc/$(GOOGLE.repo.doc) -type f -a -name "*.html" -print0 \
	    | xargs -0 svn propset svn:mime-type "text/html"

###############################################################################

$(sort $(MKDIRS)):
	$(MKDIR_P) $@
