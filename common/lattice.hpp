#ifndef LATTICE_HDR
#define LATTICE_HDR

/*!
  Lattice is used to keep all the information extracted by annotators
  (processors). Language units (tokens, words, phrases etc.)
  recognised by annotators are represented by edges (vertices
  represent points between characters). Each edge has a category and
  a attribute-value matrix (AVM). Each edge has tags which can
  be used to distinguish various levels, annotators etc.

  Lattice lifecycle:
  - construction, edge is created for each character (optimised!) [tag: raw]
  - formatter, extra edges for mark-ups and entity-likes (one edge "<"
    spanning "&lt;"), [text, mark-ups]
  - splitter - working on text level, Translatica splitter with
    function, get_text (....) [sentence]
  - tokeniser - working on text level (inside sentence?), get_edge_text [token]
  - lemmatiser - working on token level [lemma]
  - corrector - working on text level (if token without lemma),
  - parser - working ok token/lemma level, taking into account blanks
  - chooser - working on parser level
*/


class lattice {

    lattice(UnicodeString text);
    ~lattice();

    vertex_descriptor first_vertex();
    vertex_descriptor last_vertex();

    add_edge(vertex_descriptor from,
             vertex_descriptor to,
             avm,
             tags);
}

#endif

