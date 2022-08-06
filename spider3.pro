TEMPLATE = subdirs
SUBDIRS = boot3 main sha256 git-console gitlab-console scoop-console qt-console qml-console $$(HOME)/qt/common

boot3.depends = $$(HOME)/qt/common
main.depends = $$(HOME)/qt/common
sha256.depends = $$(HOME)/qt/common
git-console.depends = $$(HOME)/qt/common
gitlab-console.depends = $$(HOME)/qt/common
scoop-console.depends = $$(HOME)/qt/common
qt-console.depends = $$(HOME)/qt/common
qml-console.depends = $$(HOME)/qt/common
