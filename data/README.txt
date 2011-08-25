Data files
==========

All files with data (e.g. rules and lexicons) should be kept in `data'
subdirectories:

* if a data file is used in only one processor, it should be stored in
  `data' subdirectory of the processor's dictionary (e.g. `data'
  subdirectory of Puddle parser is `tools/parsers/puddle/data'),

* if a data file is shared among more than one processor, it should be
  stored in `data' subdirectory of the main directory.

Language-dependent rules should be stored in the subdirectory of the
appriopriate `data' directory named after the two-letter ISO 639-1
code of the given language (use three-letter ISO 639-2 codes when no
two-letter code is applicable), e.g. Polish rules for Puddle should be
kept in `tools/parsers/puddle/data/pl'. Additional sub-subdirectories
could be introduced if needed.

Language-independent rules should be stored in the `xx' subdirectory
of the given `data' directory.

