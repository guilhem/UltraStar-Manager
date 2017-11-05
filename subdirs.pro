TEMPLATE = subdirs

SUBDIRS = audiotag cleanup lyric preparatory rename app

audiotag.subdir		= src/plugins/audiotag
cleanup.subdir		= src/plugins/cleanup
lyric.subdir		= src/plugins/lyric
preparatory.subdir	= src/plugins/preparatory
rename.subdir		= src/plugins/rename

app.file	= src/UltraStar-Manager.pro
app.depends 	= audiotag cleanup lyric preparatory rename
